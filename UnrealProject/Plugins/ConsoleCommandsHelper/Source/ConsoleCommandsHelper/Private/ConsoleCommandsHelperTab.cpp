#include "ConsoleCommandsHelperTab.h"

#include "LevelEditorSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SScrollBox.h"

void SConsoleCommandsHelperTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 20;

	TSharedPtr<FConsoleCommandData> NewCommandData = MakeShareable(new FConsoleCommandData());
	NewCommandData->Enabled = true;
	NewCommandData->Data = "stat fps";
	NewCommandData->Input = 0.0f;
	ConsoleCommandsData.Add(NewCommandData);
	
	ChildSlot
	[
		SNew(SVerticalBox)

		// Main Title
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Console Commands Helper")))
			.Font(TitleTextFont)
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(FColor::White)
		]

		// Main buttons
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			
			+SHorizontalBox::Slot()
			.FillWidth(10.0f)
			.Padding(5.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Execute"))
				.OnClicked(FOnClicked::CreateLambda([this]
				{
					UWorld* World = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>()->GetWorld();
					for(auto CommandData : ConsoleCommandsData)
					{
						if(!CommandData->Enabled)
							continue;
						
						FString Command = CommandData->Data;
						if(CommandData->Input != 0.0f)
							Command.Append(FString::SanitizeFloat(CommandData->Input));
						UKismetSystemLibrary::ExecuteConsoleCommand(World, *Command);
					}
					
					return FReply::Handled();
				}))
			]
			
			+SHorizontalBox::Slot()
			.FillWidth(10.0f)
			.Padding(5.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Add"))
				.OnClicked(FOnClicked::CreateLambda([this]
				{
					TSharedPtr<SConsoleCommandsHelperTab> ThisShared = SharedThis(this);
					FString NewData = "";

					TSharedPtr<SWindow> PopUpWindow =
									SNew(SWindow)
									.Title(FText::FromString("Add Command"))
									.SizingRule(ESizingRule::Autosized)
									.Content()
									[
										SNew(SVerticalBox)
										
										+SVerticalBox::Slot()
										[
											SNew(SEditableTextBox)
											.HintText(FText::FromString("New command"))
											.OnTextChanged_Lambda([&NewData](const FText& NewText)
											{
												NewData = NewText.ToString();
											})
											.OnTextCommitted_Lambda([&PopUpWindow, &NewData, this](const FText& NewText, ETextCommit::Type CommitType)
											{
												NewData = NewText.ToString();
												if(CommitType == ETextCommit::Type::OnEnter)
													AddConsoleCommand(PopUpWindow, NewData);
											})
										]
										
										+SVerticalBox::Slot()
										.AutoHeight()
										.VAlign(VAlign_Center)
										[
											SNew(SButton)
											.Text(FText::FromString("Validate"))
											.HAlign(HAlign_Center)
											.OnClicked_Lambda([&PopUpWindow, &NewData, this]
											{
												AddConsoleCommand(PopUpWindow, NewData);
												return FReply::Handled();
											})
										]
									];
					
					FSlateApplication::Get().AddModalWindow(PopUpWindow.ToSharedRef(), ThisShared.ToSharedRef(), false);

					return FReply::Handled();
				}))
			]

			+SHorizontalBox::Slot()
			.FillWidth(10.0f)
			.Padding(5.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Save"))
				.OnClicked(FOnClicked::CreateLambda([]
				{
					return FReply::Handled();
				}))
			]

			+SHorizontalBox::Slot()
			.FillWidth(10.0f)
			.Padding(5.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Load"))
				.OnClicked(FOnClicked::CreateLambda([]
				{
					return FReply::Handled();
				}))
			]
		]

		// Commands List
		+SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox)

			+SScrollBox::Slot()
			[
				SAssignNew(ListViewWidget, SListView<TSharedPtr<FConsoleCommandData>>)
				.ItemHeight(24.0f)
				.ListItemsSource(&ConsoleCommandsData)
				.OnGenerateRow(this, &SConsoleCommandsHelperTab::OnGenerateRowForList)
			]
		]
	];
}

TSharedRef<ITableRow> SConsoleCommandsHelperTab::OnGenerateRowForList(TSharedPtr<FConsoleCommandData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		.Padding(FMargin(5.f))
		[
			SNew(SHorizontalBox)

			// Checkbox 
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(.1f)
			[
				SNew(SCheckBox)
				.IsChecked_Lambda([Item]
				{
					ECheckBoxState CheckBoxState;
					Item->Enabled ? CheckBoxState = ECheckBoxState::Checked : CheckBoxState = ECheckBoxState::Unchecked;
					return CheckBoxState;
				})
				.OnCheckStateChanged_Lambda([Item](ECheckBoxState CheckBoxState)
				{
					Item->Enabled = CheckBoxState == ECheckBoxState::Checked;
				})
			]

			// Command content
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(.45f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(*Item->Data))
			]

			// Command Input Value
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(0.15)
			[
				SNew(SSpinBox<float>)
				.EnableSlider(false)
				.Value(0.0f)
				.OnValueChanged_Lambda([Item, this](float Value)
				{
					Item->Input = Value;
			 		ListViewWidget->RequestListRefresh();
				})
			]
			
			// Remove the command from the list
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(0.25f)
			[
				SNew(SButton)
				.Text(FText::FromString("Remove"))
				.OnClicked(FOnClicked::CreateLambda([Item, this]
				{
					if(ConsoleCommandsData.Contains(Item))
						ConsoleCommandsData.Remove(Item);

					ListViewWidget->RequestListRefresh();
					return FReply::Handled();
				}))
			]
		];
}

void SConsoleCommandsHelperTab::AddConsoleCommand(TSharedPtr<SWindow> PopUpWindow, FString NewData)
{
	if(!NewData.IsEmpty())
	{
		TSharedPtr<FConsoleCommandData> NewCommandData = MakeShareable(new FConsoleCommandData());
		NewCommandData->Data = NewData;
		NewCommandData->Input = 0.0f;
		NewCommandData->Enabled = true;
													
		ConsoleCommandsData.Add(NewCommandData);
		ListViewWidget->RequestListRefresh();
	}
													
	FSlateApplication::Get().RequestDestroyWindow(PopUpWindow.ToSharedRef());
}
