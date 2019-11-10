// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4Minimal.h"
#include "UE4MinimalStyle.h"
#include "UE4MinimalCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SUE4MinimalWindow.h"

static const FName UE4MinimalTabName("UE4Minimal");

#define LOCTEXT_NAMESPACE "FUE4MinimalModule"

void FUE4MinimalModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FUE4MinimalStyle::Initialize();
	FUE4MinimalStyle::ReloadTextures();

	FUE4MinimalCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUE4MinimalCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FUE4MinimalModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUE4MinimalModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	//{
	//	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	//	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUE4MinimalModule::AddToolbarExtension));
	//	
	//	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	//}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UE4MinimalTabName, FOnSpawnTab::CreateRaw(this, &FUE4MinimalModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FUE4MinimalTabTitle", "UE4Minimal"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FUE4MinimalModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUE4MinimalStyle::Shutdown();

	FUE4MinimalCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UE4MinimalTabName);
}

TSharedRef<SDockTab> FUE4MinimalModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SUE4MinimalWindow)
			]
		];
}

void FUE4MinimalModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(UE4MinimalTabName);
}

void FUE4MinimalModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUE4MinimalCommands::Get().OpenPluginWindow);
}

void FUE4MinimalModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUE4MinimalCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE4MinimalModule, UE4Minimal)