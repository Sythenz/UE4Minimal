// Fill out your copyright notice in the Description page of Project Settings.


#include "SUE4MinimalWindow.h"
#include "SlateOptMacros.h"
#include "STextBlock.h"

#define LOCTEXT_NAMESPACE "SUE4MinimalWindow"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SUE4MinimalWindow::Construct(const FArguments& InArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("SUE4MinimalWindow::Construct")),
		FText::FromString(TEXT("UE4MinimalWindow.cpp"))
	);

	
	ChildSlot
	[
		// Populate the widget
		SNew(STextBlock)
		.Text(WidgetText)
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
