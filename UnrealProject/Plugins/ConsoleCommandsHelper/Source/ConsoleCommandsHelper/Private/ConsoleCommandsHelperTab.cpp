#include "ConsoleCommandsHelperTab.h"

#include "LevelEditorSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

void SConsoleCommandsHelperTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 20;

	TSharedPtr<FConsoleCommandData> NewCommandData = MakeShareable(new FConsoleCommandData());
	NewCommandData.Get()->Enabled = true;
	NewCommandData.Get()->Data = "stat fps";
	NewCommandData.Get()->Input = "";
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
					UE_LOG(LogTemp, Warning, TEXT("Sylen : Execute All Commands"));
					UWorld* World = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>()->GetWorld();
					for(auto CommandData : ConsoleCommandsData)
					{
						if(!CommandData->Enabled)
							continue;
						
						FString Command = CommandData->Data.Append(CommandData->Input);
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
					UE_LOG(LogTemp, Warning, TEXT("Sylen : Add Command"));
					TSharedPtr<FConsoleCommandData> NewCommandData = MakeShareable(new FConsoleCommandData());
					NewCommandData.Get()->Enabled = true;
					NewCommandData.Get()->Data = "Some Command";
					NewCommandData.Get()->Input = "1";
					ConsoleCommandsData.Add(NewCommandData);
					ListViewWidget->RequestListRefresh();
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
					UE_LOG(LogTemp, Warning, TEXT("Sylen : Save Commands"));
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
					UE_LOG(LogTemp, Warning, TEXT("Sylen : Load Commands"));
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
			.FillWidth(.05f)
			[
				SNew(SCheckBox)
				.IsChecked_Lambda([Item]
				{
					ECheckBoxState CheckBoxState;
					Item.Get()->Enabled ? CheckBoxState = ECheckBoxState::Checked : CheckBoxState = ECheckBoxState::Unchecked;
					return CheckBoxState;
				})
				.OnCheckStateChanged_Lambda([Item](ECheckBoxState CheckBoxState)
				{
					Item.Get()->Enabled = CheckBoxState == ECheckBoxState::Checked;
				})
			]

			// Command content
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(.05f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(*Item.Get()->Data))
			]

			// Input data
			// +SHorizontalBox::Slot()
			// .HAlign(HAlign_Left)
			// .VAlign(VAlign_Center)
			// .FillWidth(.05f)
			// [
			// 	SNew(SInlineEditableTextBlock)
			// 	.OnTextCommitted_Lambda([Item](const FText& CommitedText, ETextCommit::Type TyepTextCommit)
			// 	{
			// 		Item.Get()->Data = CommitedText.ToString();
			// 	})
			// ]
		];
}
