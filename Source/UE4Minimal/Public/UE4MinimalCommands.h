// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UE4MinimalStyle.h"

class FUE4MinimalCommands : public TCommands<FUE4MinimalCommands>
{
public:

	FUE4MinimalCommands()
		: TCommands<FUE4MinimalCommands>(TEXT("UE4Minimal"), NSLOCTEXT("Contexts", "UE4Minimal", "UE4Minimal Plugin"), NAME_None, FUE4MinimalStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};