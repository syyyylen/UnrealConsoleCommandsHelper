// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConsoleCommandsHelperCommands.h"

#define LOCTEXT_NAMESPACE "FConsoleCommandsHelperModule"

void FConsoleCommandsHelperCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ConsoleCommandsHelper", "Open ConsoleCommandsHelper Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
