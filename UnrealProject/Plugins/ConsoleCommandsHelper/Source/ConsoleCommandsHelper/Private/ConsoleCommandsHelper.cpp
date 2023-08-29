// Copyright 2023 - Jules Vannier

#include "ConsoleCommandsHelper.h"
#include "ConsoleCommandsHelperStyle.h"
#include "ConsoleCommandsHelperCommands.h"
#include "ConsoleCommandsHelperTab.h"
#include "ToolMenus.h"

static const FName ConsoleCommandsHelperTabName("ConsoleCommandsHelper");
#define LOCTEXT_NAMESPACE "FConsoleCommandsHelperModule"

void FConsoleCommandsHelperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FConsoleCommandsHelperStyle::Initialize();
	FConsoleCommandsHelperStyle::ReloadTextures();

	FConsoleCommandsHelperCommands::Register();

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("ConsoleCommandsHelperTab"), FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs& SpawnTabArgs)
	{
		CommandsDockTab = SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SAssignNew(ConsoleCommandsHelperTab, SConsoleCommandsHelperTab)
		];

		CommandsDockTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateLambda([this](TSharedRef<SDockTab> TabToClose)
		{
			if(ConsoleCommandsHelperTab.IsValid())
			{
				ConsoleCommandsHelperTab->SaveConsoleCommands();
				ConsoleCommandsHelperTab.Reset();
			}
			
			if(CommandsDockTab.IsValid())
				CommandsDockTab.Reset();
		}));

		return CommandsDockTab.ToSharedRef();
	}))
	.SetDisplayName(FText::FromString("Console Commands"));

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FConsoleCommandsHelperCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FConsoleCommandsHelperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FConsoleCommandsHelperModule::RegisterMenus));
}

void FConsoleCommandsHelperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("ConsoleCommandsHelperTab"));
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FConsoleCommandsHelperStyle::Shutdown();
	FConsoleCommandsHelperCommands::Unregister();
}

void FConsoleCommandsHelperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName("ConsoleCommandsHelperTab"));
}

void FConsoleCommandsHelperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FConsoleCommandsHelperCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FConsoleCommandsHelperCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FConsoleCommandsHelperModule, ConsoleCommandsHelper)