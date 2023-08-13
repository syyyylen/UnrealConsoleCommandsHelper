// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConsoleCommandsHelperStyle.h"
#include "ConsoleCommandsHelper.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FConsoleCommandsHelperStyle::StyleInstance = nullptr;

void FConsoleCommandsHelperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FConsoleCommandsHelperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FConsoleCommandsHelperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ConsoleCommandsHelperStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FConsoleCommandsHelperStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ConsoleCommandsHelperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ConsoleCommandsHelper")->GetBaseDir() / TEXT("Resources"));

	Style->Set("ConsoleCommandsHelper.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FConsoleCommandsHelperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FConsoleCommandsHelperStyle::Get()
{
	return *StyleInstance;
}