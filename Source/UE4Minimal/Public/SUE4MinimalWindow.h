// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class UE4MINIMAL_API SUE4MinimalWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUE4MinimalWindow)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
