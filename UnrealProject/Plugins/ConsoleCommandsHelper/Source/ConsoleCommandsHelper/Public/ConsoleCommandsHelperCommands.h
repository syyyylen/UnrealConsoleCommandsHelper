// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ConsoleCommandsHelperStyle.h"

class FConsoleCommandsHelperCommands : public TCommands<FConsoleCommandsHelperCommands>
{
public:

	FConsoleCommandsHelperCommands()
		: TCommands<FConsoleCommandsHelperCommands>(TEXT("ConsoleCommandsHelper"), NSLOCTEXT("Contexts", "ConsoleCommandsHelper", "ConsoleCommandsHelper Plugin"), NAME_None, FConsoleCommandsHelperStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
