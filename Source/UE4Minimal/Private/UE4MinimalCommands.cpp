// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4MinimalCommands.h"

#define LOCTEXT_NAMESPACE "FUE4MinimalModule"

void FUE4MinimalCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UE4Minimal", "Bring up UE4Minimal window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
