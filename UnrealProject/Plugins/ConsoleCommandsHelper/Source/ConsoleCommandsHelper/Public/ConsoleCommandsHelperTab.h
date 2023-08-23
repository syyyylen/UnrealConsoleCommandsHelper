#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FConsoleCommandData
{
	bool Enabled;
	FString Data;
};

class SConsoleCommandsHelperTab : public SCompoundWidget
{
private:
	SLATE_BEGIN_ARGS(SConsoleCommandsHelperTab) {}

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);
	void SaveConsoleCommands();

private:
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FConsoleCommandData> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void AddConsoleCommand(TSharedPtr<SWindow> PopUpWindow, FString NewData);
	void LoadTemplate(FString FilePath);
	
	TArray<TSharedPtr<FConsoleCommandData>> ConsoleCommandsData;
	TSharedPtr<SListView<TSharedPtr<FConsoleCommandData>>> ListViewWidget;
};
