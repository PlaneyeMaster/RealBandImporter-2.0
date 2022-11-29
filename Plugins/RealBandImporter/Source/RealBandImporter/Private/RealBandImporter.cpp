// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealBandImporter.h"
#include "RealBandImporterStyle.h"
#include "RealBandImporterCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "UI/FRealBandUIManager.h"

#include "Importer/FRealBandAssetImporter.h"

static const FName RealBandImporterTabName("RealBandImporter");

#define LOCTEXT_NAMESPACE "FRealBandImporterModule"

void FRealBandImporterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (GIsEditor && !IsRunningCommandlet())
	{
//		FRealBandUIManager::Initialize();
	}
	
	FRealBandImporterStyle::Initialize();
	FRealBandImporterStyle::ReloadTextures();

	FRealBandImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRealBandImporterCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FRealBandImporterModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRealBandImporterModule::RegisterMenus));
}

void FRealBandImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FRealBandAsstImp.Reset();

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRealBandImporterStyle::Shutdown();

	FRealBandImporterCommands::Unregister();
}

void FRealBandImporterModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FRealBandImporterModule::PluginButtonClicked()")),
							FText::FromString(TEXT("RealBandImporter.cpp"))
					   );
	//FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	//TSharedPtr<FRealBandAssetImporter> FRealBandAsstImp = MakeShareable(new FRealBandAssetImporter);
	FRealBandAsstImp = MakeShareable(new FRealBandAssetImporter);
	FRealBandAsstImp->Init();
	FRealBandUIManager::Initialize(FRealBandAsstImp);
	
}

void FRealBandImporterModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRealBandImporterCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRealBandImporterCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRealBandImporterModule, RealBandImporter)