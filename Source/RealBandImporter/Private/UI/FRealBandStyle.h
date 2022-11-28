// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

class FSlateStyleSet;
class ISlateStyle;
class FRealBandStyle
{
public:
	static void Initialize();
	static FName GetStyleSetName();
	static const  ISlateStyle& Get(); 
	static const FSlateBrush* GetBrush(FName PropertyName, const ANSICHAR* Specifier = NULL);
	static void SetIcon(const FString& StyleName, const FString& ResourcePath);
	static void SetSVGIcon(const FString& StyleName, const FString& ResourcePath);

private:
	static TUniquePtr<FSlateStyleSet> Create();
	static TUniquePtr<FSlateStyleSet> MSStyleInstance;
	
};
