// Copyright 2023 - Jules Vannier

#include "ConsoleCommandsHelperCommands.h"

#define LOCTEXT_NAMESPACE "FConsoleCommandsHelperModule"

void FConsoleCommandsHelperCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ConsoleCommandsHelper", "Open ConsoleCommandsHelper Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
