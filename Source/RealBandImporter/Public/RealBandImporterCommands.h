// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RealBandImporterStyle.h"

class FRealBandImporterCommands : public TCommands<FRealBandImporterCommands>
{
public:

	FRealBandImporterCommands()
		: TCommands<FRealBandImporterCommands>(TEXT("RealBandImporter"), NSLOCTEXT("Contexts", "RealBandImporter", "RealBandImporter Plugin"), NAME_None, FRealBandImporterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
