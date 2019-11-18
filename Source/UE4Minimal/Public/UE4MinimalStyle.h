// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**  */
class FUE4MinimalStyle
{
public:
	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	static const ISlateStyle& Get();

	static FName GetStyleSetName();

    static void overrideCoreWidgetBrush(FString ThemePath, FString Type, FString slateCathegory, FString member, FString newPath);
private:

	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};
