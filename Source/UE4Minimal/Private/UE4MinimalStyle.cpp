// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4MinimalStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"

TSharedPtr< FSlateStyleSet > FUE4MinimalStyle::StyleInstance = NULL;


#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush(*(themePath + RelativePath + ".png"), __VA_ARGS__)
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(*(themePath + RelativePath + ".png"), __VA_ARGS__)
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush(*(themePath + RelativePath + ".png"), __VA_ARGS__)
#define DEFAULT_FONT( RelativePath, ... )FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

void FUE4MinimalStyle::Initialize()
{
    FString themePath = FPaths::ProjectPluginsDir() + "UE4Minimal/Theme/";
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
        //FSlateStyleSet* EditorStyles = (FSlateStyleSet*)&FEditorStyle::Get();
        //static FSlateBrush PurpleBrush = *FEditorStyle::GetBrush("Graph.Panel.SolidBackground");
        //FLinearColor purple(1.0f, 0.0f, 1.0f);
        //PurpleBrush.TintColor = FSlateColor();
        //const ISlateStyle& coreStyles = FCoreStyle::Get();
        //*(FSlateBrush*)coreStyles.GetBrush("ToolBar.Background") = PurpleBrush;
        

        {
            FString JsonString;
            const FString JsonFilePath(FPaths::ProjectPluginsDir() + "UE4Minimal/UE4MinimalOverrides.json");
            FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
            TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
            TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
            UE_LOG(LogTemp, Display, TEXT("jsonPath: %s"), *JsonFilePath);

            if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
                for(TSharedPtr<FJsonValue> overrideFile : JsonObject->GetArrayField("ImageOverride")) {
                    const TMap<FString, TSharedPtr<FJsonValue>>::ElementType& overrideField = *overrideFile->AsObject()->Values.begin();
                    UE_LOG(LogTemp, Display, TEXT("overrideField: %s"), *overrideField.Key);
                    
                    TSharedPtr<FJsonValue> slateCathegories(overrideField.Value);
                    for (TSharedPtr<FJsonValue> sCath : slateCathegories->AsArray()) {
                        FString type = "";
                        FString member = "";
                        FString slatePath = "";
                        {
                            FString full(sCath->AsString());
                            if (full.Split(":", &type, &slatePath)) {
                                UE_LOG(LogTemp, Display, TEXT("type: %s"), *type);
                            }
                        }
                        {
                            FString full(sCath->AsString());
                            if (full.Split("/", &slatePath, &member)) {
                                UE_LOG(LogTemp, Display, TEXT("member: %s"), *member);
                            }
                        }
                        if (member.IsEmpty() && type.IsEmpty()) {
                            slatePath = sCath->AsString();
                        } else {
                            slatePath.Split(":", &type, &slatePath);
                        }
                        UE_LOG(LogTemp, Display, TEXT("slatePath: %s"), *slatePath);
                        FUE4MinimalStyle::overrideCoreWidgetBrush(themePath, type, slatePath, member, overrideField.Key);
                        UE_LOG(LogTemp, Display, TEXT("FULL: %s, type: %s, member: %s, slatePath %s"), *sCath->AsString(), *type, *member, *slatePath);
                    }
                }
            } else {
                UE_LOG(LogTemp, Error, TEXT("json isn't valid"));
            }
        }
	}
}

template <typename InObj>
void overrideBrushProperty(InObj& obj, FString themePath, FString type, FString slateCathegory, FString member, FString newPath) {
    UE_LOG(LogTemp, Display, TEXT("Struct cpp name: %s"), *obj.StaticStruct()->GetStructCPPName());
    for (TFieldIterator<UProperty> propIt(obj.StaticStruct()); propIt; ++propIt) {
        UProperty* Property = *propIt;
        if (Property->GetNameCPP() == member) {
            FSlateBrush* originalBrush = Property->ContainerPtrToValuePtr<FSlateBrush>((void*)&obj, 0);
            *originalBrush = BOX_BRUSH(newPath, originalBrush->GetMargin());
            originalBrush->GetImageSize();
            break;
        }
    }
}

// if there is a way to do this using UE4 introspection, please replace this ugly code
// all classes comming from CoreStyle.h
void overrideCoreWidget(FString themePath, FString type, FString slateCathegory, FString member, FString newPath) {
    const ISlateStyle& coreStyles = FCoreStyle::Get();
    if (type == "FCheckBoxStyle") {
        return overrideBrushProperty((FCheckBoxStyle&)coreStyles.GetWidgetStyle<FCheckBoxStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FTextBlockStyle") {
        return overrideBrushProperty((FTextBlockStyle&)coreStyles.GetWidgetStyle<FTextBlockStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FButtonStyle") {
        return overrideBrushProperty((FButtonStyle&)coreStyles.GetWidgetStyle<FButtonStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FComboButtonStyle") {
        return overrideBrushProperty((FComboButtonStyle&)coreStyles.GetWidgetStyle<FComboButtonStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FComboBoxStyle") {
        return overrideBrushProperty((FComboBoxStyle&)coreStyles.GetWidgetStyle<FComboBoxStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FHyperlinkStyle") {
        return overrideBrushProperty((FHyperlinkStyle&)coreStyles.GetWidgetStyle<FHyperlinkStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FEditableTextStyle") {
        return overrideBrushProperty((FEditableTextStyle&)coreStyles.GetWidgetStyle<FEditableTextStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FScrollBarStyle") {
        return overrideBrushProperty((FScrollBarStyle&)coreStyles.GetWidgetStyle<FScrollBarStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FEditableTextBoxStyle") {
        return overrideBrushProperty((FEditableTextBoxStyle&)coreStyles.GetWidgetStyle<FEditableTextBoxStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FInlineEditableTextBlockStyle") {
        return overrideBrushProperty((FInlineEditableTextBlockStyle&)coreStyles.GetWidgetStyle<FInlineEditableTextBlockStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FProgressBarStyle") {
        return overrideBrushProperty((FProgressBarStyle&)coreStyles.GetWidgetStyle<FProgressBarStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FExpandableAreaStyle") {
        return overrideBrushProperty((FExpandableAreaStyle&)coreStyles.GetWidgetStyle<FExpandableAreaStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FSearchBoxStyle") {
        return overrideBrushProperty((FSearchBoxStyle&)coreStyles.GetWidgetStyle<FSearchBoxStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FSliderStyle") {
        return overrideBrushProperty((FSliderStyle&)coreStyles.GetWidgetStyle<FSliderStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FVolumeControlStyle") {
        return overrideBrushProperty((FVolumeControlStyle&)coreStyles.GetWidgetStyle<FVolumeControlStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FInlineTextImageStyle") {
        return overrideBrushProperty((FInlineTextImageStyle&)coreStyles.GetWidgetStyle<FInlineTextImageStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FSpinBoxStyle") {
        return overrideBrushProperty((FSpinBoxStyle&)coreStyles.GetWidgetStyle<FSpinBoxStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FSplitterStyle") {
        return overrideBrushProperty((FSplitterStyle&)coreStyles.GetWidgetStyle<FSplitterStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FTableRowStyle") {
        return overrideBrushProperty((FTableRowStyle&)coreStyles.GetWidgetStyle<FTableRowStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FTableColumnHeaderStyle") {
        return overrideBrushProperty((FTableColumnHeaderStyle&)coreStyles.GetWidgetStyle<FTableColumnHeaderStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FHeaderRowStyle") {
        return overrideBrushProperty((FHeaderRowStyle&)coreStyles.GetWidgetStyle<FHeaderRowStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FDockTabStyle") {
        return overrideBrushProperty((FDockTabStyle&)coreStyles.GetWidgetStyle<FDockTabStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FScrollBoxStyle") {
        return overrideBrushProperty((FScrollBoxStyle&)coreStyles.GetWidgetStyle<FScrollBoxStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FScrollBorderStyle") {
        return overrideBrushProperty((FScrollBorderStyle&)coreStyles.GetWidgetStyle<FScrollBorderStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else if (type == "FWindowStyle") {
        return overrideBrushProperty((FWindowStyle&)coreStyles.GetWidgetStyle<FWindowStyle>(*slateCathegory), themePath, type, slateCathegory, member, newPath);
    } else {
        // insert some UE4_LOG warning here
    }
}

void FUE4MinimalStyle::overrideCoreWidgetBrush(FString themePath, FString type, FString slateCathegory, FString member, FString newPath) {
    overrideCoreWidget(themePath, type, slateCathegory, member, newPath);
}

void FUE4MinimalStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FUE4MinimalStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UE4MinimalStyle"));
	return StyleSetName;
}

TSharedRef< FSlateStyleSet > FUE4MinimalStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("UE4MinimalStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("UE4Minimal")->GetBaseDir() / TEXT("Resources"));

	Style->Set("UE4Minimal.OpenPluginWindow", new FSlateImageBrush(Style->RootToContentDir(TEXT("ButtonIcon_40x"), TEXT(".png")), FVector2D(40.0f, 40.0f)));

	return Style;
}

void FUE4MinimalStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FUE4MinimalStyle::Get()
{
	return *StyleInstance;
}
