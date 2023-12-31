// Copyright 2023 - Jules Vannier

#pragma once

#include "CoreMinimal.h"
#include "ConsoleCommandsHelperTab.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FConsoleCommandsHelperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

private:

	void RegisterMenus();

	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SDockTab> CommandsDockTab;
	TSharedPtr<SConsoleCommandsHelperTab> ConsoleCommandsHelperTab;
};
