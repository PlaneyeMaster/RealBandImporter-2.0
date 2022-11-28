// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealBandImporterStyle.h"
#include "RealBandImporter.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FRealBandImporterStyle::StyleInstance = nullptr;

void FRealBandImporterStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FRealBandImporterStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FRealBandImporterStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("RealBandImporterStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon30x30(30.0f, 30.0f);

TSharedRef< FSlateStyleSet > FRealBandImporterStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("RealBandImporterStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("RealBandImporter")->GetBaseDir() / TEXT("Resources"));

	Style->Set("RealBandImporter.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	Style->Set("RealBandImporter.SaveAction", new IMAGE_BRUSH_SVG(TEXT("saveIcon"), Icon20x20));
	Style->Set("RealBandImporter.SaveActionPNG", new IMAGE_BRUSH(TEXT("icons8"), Icon30x30));

	const FSlateBrush* pBrush = Style->GetBrush(FName("RealBandImporter.SaveActionPNG"));

	return Style;
}

void FRealBandImporterStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FRealBandImporterStyle::Get()
{
	return *StyleInstance;
}
