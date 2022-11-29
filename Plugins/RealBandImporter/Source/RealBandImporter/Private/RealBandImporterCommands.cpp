// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealBandImporterCommands.h"

#define LOCTEXT_NAMESPACE "FRealBandImporterModule"

void FRealBandImporterCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "RealBandImporter", "Execute RealBandImporter action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
