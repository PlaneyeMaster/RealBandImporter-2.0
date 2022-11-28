// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class FRealBandAssetImporter;
#define MS_MODULE_NAME TEXT("RealBandImporter")

class FRealBandImporterModule : public IModuleInterface
{
public:

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded(MS_MODULE_NAME);
	}

	static inline FRealBandImporterModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FRealBandImporterModule>(MS_MODULE_NAME);
	}


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<FRealBandAssetImporter> FRealBandAsstImp;
};
