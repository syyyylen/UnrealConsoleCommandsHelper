// Copyright 2023 - Jules Vannier

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FConsoleCommandsHelperStyle
{
public:

	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();

private:
	static TSharedRef< class FSlateStyleSet > Create();
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};