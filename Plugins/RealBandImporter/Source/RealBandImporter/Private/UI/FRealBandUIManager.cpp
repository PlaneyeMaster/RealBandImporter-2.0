// Copyright RealEye, Inc. All Rights Reserved.

//UI
#include "UI/FRealBandUIManager.h"
#include "UI/FRealBandStyle.h"
#include "UI/FRealBandAssetLoader.h"
// Settings
#include "Settings/RealBandImportSettings.h"
//Importers
    //Datasmith
#include "DatasmithTranslator.h"
#include "DatasmithImportFactory.h"
#include "DatasmithSceneFactory.h"
#include "DatasmithImportContext.h"
#include "DatasmithStaticMeshImporter.h"
#include "DatasmithScene.h"
#include "Importer/FRealBandAssetImporter.h"
//Widgets
#include "Widgets/SWindow.h"
#include "Widgets/SWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Images/SSpinningImage.h"
#include "Layout/ChildrenBase.h"
#include "Widgets/Layout/SGridPanel.h"
#include "NodeFactory.h"
#include "Brushes/SlateBoxBrush.h"

#include "ImageUtils.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateStyle.h"
#include "Styling/StyleColors.h"

//List of Project files
#include "Interfaces/IProjectManager.h"
#include "EditorDirectories.h"
//File picker includes
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

// WebBrowser
#include "SWebBrowser.h"
#include "WebBrowserModule.h"
#include "IWebBrowserSingleton.h"
#include "IWebBrowserCookieManager.h"
#include "AssetRegistryModule.h"
#include "AssetViewUtils.h"
#include "AssetViewTypes.h"
// FBX
#include "Factories/FbxFactory.h"
#include "Factories/FbxAnimSequenceImportData.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxAssetImportData.h"
//#include "FbxImporter.h"
//FBX

//GLTF
#include "GLTFImporterModule.h"
#include "../Private/GLTFImportFactory.h"
#include "../Private/GLTFImportOptions.h"

#include "ExternalSourceModule.h"
#include "ExternalSource.h"
#include "SourceUri.h"
//
#include "ContentBrowserModule.h"
#include "ContentBrowserItem.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "SAssetView.h"

//Editor
#include "LevelEditor.h"
#include "PropertyEditor/Private/SDetailCategoryTableRow.h"

//Collections
#include "CollectionManagerTypes.h"
#include "CollectionManagerModule.h"
#include "ICollectionManager.h"
//#include "AssetManagerEditorModule.h"
//core
#include "CoreGlobals.h"

//Texture
#include "Factories/TextureFactory.h"

#include "Styling/SlateStyleMacros.h"

//Tools Menus
#include "ToolMenus.h"
#include "ToolMenuSection.h"

//#include "RealBandImportSettings.h"
//uint16 FRealBandUIManagerImpl::ActiveTypeBitset = TNumericLimits<uint16>::Max();
//#include "Cascade/Private/CascadeActions.h"


DEFINE_LOG_CATEGORY(LogManager);

#define LEVELEDITOR_MODULE_NAME TEXT("LevelEditor")
TSharedPtr<FRealBandUIManagerImpl> FRealBandUIManager::Instance;
const FName RealBandTabName = "RealBandTab";

#define LOCTEXT_NAMESPACE "RealBand"


FRealBandUIManagerImpl::FRealBandUIManagerImpl(TSharedPtr<FRealBandAssetImporter> inFRealBandAssetImporter):
	                    pFRealBandAssetImporter(inFRealBandAssetImporter)
{
	ObjUserPreference.ActiveTypeBitset = TNumericLimits<uint16>::Max();
	ObjUserPreference.ActiveTextypeBitset = TNumericLimits<uint16>::Max();
	ObjUserPreference.Format = TNumericLimits<uint8>::Max();;
}

FRealBandUIManagerImpl::~FRealBandUIManagerImpl()
{
	//FRealBandUIManager::Instance.Reset();
	//pFRealBandAssetLoader.Reset();
	pOverlay.Reset();
	pCanvas.Reset();
	//pFRealBandAssetLoader.Reset();
	ObjUserPreference.ActiveTypeBitset = TNumericLimits<uint16>::Max();
	ObjUserPreference.ActiveTextypeBitset = TNumericLimits<uint16>::Max();
	int test = 1;
}
void FRealBandUIManagerImpl::Initialize()
{
	 
	// Find all the engine installations
	TMap<FString, FString> EngineInstallations;
	FDesktopPlatformModule::Get()->EnumerateEngineInstallations(EngineInstallations);
	TSet<FString> AllFoundProjectFiles;

	for (TMap<FString, FString>::TConstIterator Iter(EngineInstallations); Iter; ++Iter)
	{
		TArray<FString> ProjectFiles;

		if (FDesktopPlatformModule::Get()->EnumerateProjectsKnownByEngine(Iter.Key(), false, ProjectFiles))
		{
			AllFoundProjectFiles.Append(MoveTemp(ProjectFiles));
		}
	}
	for (FString& ProjectFile : AllFoundProjectFiles)
	{
		FString ProjectFilename = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*ProjectFile);

		if (FPaths::FileExists(ProjectFilename))
		{
			FProjectStatus ProjectStatus;
			if (IProjectManager::Get().QueryStatusForProject(ProjectFilename, ProjectStatus))
			{
				// @todo localized project name
				const FText ProjectName = FText::FromString(ProjectStatus.Name);
				Array_Resolutions.Add(MakeShareable<FString>(new FString(ProjectStatus.Name)));
				
			}
			
		}
	}

	SetupMenuItem();
	//Create the MainWindow
	CreateWindow();

}



void FRealBandUIManager::Initialize(TSharedPtr<FRealBandAssetImporter> iFRealBandAssetImporter)
{
	
	if (!Instance.IsValid())
	{
		FRealBandStyle::Initialize();
		Instance = MakeShareable(new FRealBandUIManagerImpl(iFRealBandAssetImporter));
		FRealBandStyle::SetSVGIcon("SaveIcon", "saveIcon");
		//FRealBandStyle::SetIcon("Icon8", "IconsB");
		FRealBandStyle::SetIcon("Icon8", "Icon40x40");
	}
	
	Instance->Initialize();
}


void FRealBandUIManagerImpl::SetupMenuItem()
{
	FRealBandStyle::SetIcon("Logo", "Logo80x80");
	FRealBandStyle::SetIcon("ContextLogo", "Logo32x32");
	//FRealBandStyle::SetSVGIcon("MenuLogo", "QuixelBridgeB");
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LEVELEDITOR_MODULE_NAME);
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, nullptr, FToolBarExtensionDelegate::CreateRaw(this, &FRealBandUIManagerImpl::FillToolbar));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	// Adding Bridge entry to Quick Content menu.
	UToolMenu* AddMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.AddQuickMenu");
	FToolMenuSection& Section = AddMenu->FindOrAddSection("Content");
	Section.AddMenuEntry("OpenRealBand",
		LOCTEXT("OpenRealBand_Label", "RealBandImporter"),
		LOCTEXT("OpenRealBand_Desc", "Opens the RealBand Importer."),
		FSlateIcon(FName("RealBandImporterStyle"), "RealBand.MenuLogo"),
		FUIAction(FExecuteAction::CreateRaw(this, &FRealBandUIManagerImpl::CreateWindow), FCanExecuteAction())
	).InsertPosition = FToolMenuInsert("ImportContent", EToolMenuInsertType::After);
}

void FRealBandUIManagerImpl::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection(TEXT("RealBandImporter"));
	{
		ToolbarBuilder.AddToolBarButton(
			FUIAction(FExecuteAction::CreateRaw(this, &FRealBandUIManagerImpl::CreateWindow)),
			FName(TEXT("RealBand Importer")),
			LOCTEXT("QMSLiveLink_label", "RealBandImporter"),
			LOCTEXT("WorldProperties_ToolTipOverride", "Megascans Link"),
			FSlateIcon(FRealBandStyle::GetStyleSetName(), "RealBand.Logo"),
			EUserInterfaceActionType::Button,
			FName(TEXT("RealBandImporter"))
		);
	}
	ToolbarBuilder.EndSection();
}


void FRealBandUIManagerImpl::CreateWindow()
{
	bool bIsVisible = false;

	/*FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = NAME_None;
	TSharedPtr<class IDetailsView> SettingsDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	URealBandImportSettings* pRealBandSettings = GetMutableDefault<URealBandImportSettings>();
	SettingsDetailsView->SetObject(pRealBandSettings);
	*/
	//TSharedPtr <RealBandImportSettingsUI> pRealBandImportSettings = SNew(RealBandImportSettingsUI);
	//SAssignNew(pRealBandImportSettings, URealBandImportSettings)

	if (!pDialogMainWindow )
	{
		// Get the AssetConfig from FRealBandAssetImporter
		FAssetPickerConfig ConfigPicker;
		pFRealBandAssetImporter->GetAssetConfig(ConfigPicker);

		SelectedProject = (*Array_Resolutions.begin());
		pDialogMainWindow = SNew(SWindow)
			.Title(FText::FromString("RealBand"))
			.ClientSize(FVector2D(1100, 800))
			.SupportsMaximize(false)
			.SupportsMinimize(false)
			.SizingRule(ESizingRule::UserSized)
			//.IsTopmostWindow(true)
			[
				SAssignNew(pOverlay, SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SBox)
				.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(pCanvas, SCanvas)
				
			+ SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			.Position(FVector2D(30.0f, 80.0f))
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Local"))
			.OnClicked(this, &FRealBandUIManagerImpl::OnLocal)

			]
		// Asset Viewer
		+ SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(830.0f, 640.0f))
			.Position(FVector2D(150.0f, 125.0f))
			[
				SAssignNew(pFRealBandAssetLoader, FRealBandAssetLoader)
			   .AssetPickerConfig(ConfigPicker)
			]
		
		///////////////////
		+ SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			.Position(FVector2D(30.0f, 180.0f))
			[
				SAssignNew(pSettings, SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Settings"))
			.OnClicked(this, &FRealBandUIManagerImpl::LaunchSettings)
			]
		 + SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			.Position(FVector2D(750.0f, 5.0f))
			[
				SAssignNew(pImport, SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Import"))
			.OnClicked(this, &FRealBandUIManagerImpl::OnImportBtnClicked)
			]
		    + SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(950.0f, 700.0f))
				.Position(FVector2D(140.0f, 45.0f))
			[
				SAssignNew(pRealBandImportSettings, RealBandImportSettingsUI)
				.ObjPreference(&ObjUserPreference)
				//pRealBandImportSettings.ToSharedRef()
				//SAssignNew(pRealBandImportSettings, URealBandImportSettings)
				//SAssignNew(pSettingsWindow, SWindow)
				//.Title(FText::FromString("Settings"))
				//.Style(&FCoreStyle::Get().GetWidgetStyle<FWindowStyle>("Window"))
				//.SizingRule(ESizingRule::Autosized)
				//.ClientSize(FVector2D(900, 600))
				//.SupportsMaximize(false)
				//.SupportsMinimize(false)
				//
				//[
				//	 SAssignNew(pSettingsCanvasBox, SCanvas)
				//	 + SCanvas::Slot()
				//      .HAlign(HAlign_Fill)
				//      .VAlign(VAlign_Fill)
    //                  .Size(FVector2D(750.0f, 600.0f))
				//      .Position(FVector2D(100.0f, 15.0f))
				//      [
				//		  SNew(SVerticalBox)
				//		  + SVerticalBox::Slot()
				//        [
				//		  SNew(SSplitter)
				//		  .Orientation(EOrientation::Orient_Vertical)
				//           + SSplitter::Slot()
				//            .Value(0.8f)
				//            [
				//	            SNew(SSplitter)
				//	            .Orientation(EOrientation::Orient_Horizontal)
				//                + SSplitter::Slot()
				//                .Value(0.5f)
				//                [
				//	                SNew(SWidgetSwitcher)
				//	                + SWidgetSwitcher::Slot()
				//                      
				//                      .HAlign(HAlign_Fill)
				//                      .VAlign(VAlign_Fill)
				//                    [
				//	                   SettingsDetailsView.ToSharedRef()
				//                    ]
				//                 ]
				//             ]
				//		]
				//		 
				//	   ]
			]
					 + SCanvas::Slot()
						 .HAlign(HAlign_Fill)
						 .VAlign(VAlign_Fill)
						 .Size(FVector2D(100.0f, 50.0f))
						 //.Position(FVector2D(690.0f, 600.0f))
						 .Position(FVector2D(450.0f, 680.0f))
						 [
							 SAssignNew(pApplyButton, SButton)
							 .HAlign(HAlign_Center)
						 .VAlign(VAlign_Center)
						 .Text(FText::FromString("Apply"))
						 .OnClicked(this, &FRealBandUIManagerImpl::ApplySettings)
						 .ToolTipText(LOCTEXT("ApplyButtonTooltip", "Apply settings"))
						 ]
				//	 + SCanvas::Slot()
				//		 .HAlign(HAlign_Fill)
				//		 .VAlign(VAlign_Fill)
				//		 .Size(FVector2D(100.0f, 50.0f))
				//		 .Position(FVector2D(490.0f, 600.0f))
				//		 [
				//			 SNew(SButton)
				//			 .HAlign(HAlign_Center)
				//		 .VAlign(VAlign_Center)
				//		 .Text(FText::FromString("Reset"))
				//		 .OnClicked(this, &FRealBandUIManagerImpl::ResetSettings)
				//		 .ToolTipText(LOCTEXT("ResetButtonTooltip", "Clear all settings"))
				//		 ]

				//  ]// Settings Window
			    
			


			]

			]

			];


		FSlateApplication::Get().AddWindow(pDialogMainWindow.ToSharedRef());

	//	pSettingsWindow->SetVisibility(EVisibility::Hidden);
		pApplyButton->SetVisibility(EVisibility::Hidden);
		pFRealBandAssetLoader->SetVisibility(EVisibility::Collapsed);
		pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
		
		pRealBandImportSettings->pSettingsWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
			{
				pImport->SetVisibility(EVisibility::Visible);
				pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
				pApplyButton->SetVisibility(EVisibility::Hidden);
			}));

	}
	else
	{
	    bIsVisible = pDialogMainWindow->IsVisible();
		
    }

	pDialogMainWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
		{
			pDialogMainWindow.Reset();
		}));

}


TSharedRef<SHorizontalBox> FRealBandUIManagerImpl::GetSelectedOptionToggle(SELECTOPTIONS type , const FText& Text)
{
	
	SAssignNew(pHBox, SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()

		[
			SAssignNew(pChkBox, SCheckBox)
			//SNew(SCheckBox)
			.IsChecked(ECheckBoxState::Checked)
		    .BorderBackgroundColor(FSlateColor(FLinearColor::Gray))
		    //.BackgroundImage()

		.OnCheckStateChanged(this, &FRealBandUIManagerImpl::OnTypeChanged,type)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.0f, 0.0f, 10.0f, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(Text)

		];

	OptionsFormat.Add(pChkBox);
	return pHBox.ToSharedRef();
	/*return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		
		[
			SNew(SCheckBox)
			.IsChecked(ECheckBoxState::Checked)
		    
		.OnCheckStateChanged(this,&FRealBandUIManagerImpl::OnTypeChanged, type)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(4.0f, 0.0f, 10.0f, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(Text)
		    
		];*/
}

void FRealBandUIManagerImpl::OnTypeChanged(ECheckBoxState CheckState, SELECTOPTIONS Type)
{
	const uint16 Mask = 1 << Type;
	//const uint16 TMask = 1 << TexType;
	switch (CheckState)
	{
	case ECheckBoxState::Checked:
		ObjUserPreference.ActiveTypeBitset |= Mask;
	//	AssetFormat = AssetFormat << Type;
		//ObjUserPreference.ActiveTypeBitset |= TMask;
		break;
	default:
		ObjUserPreference.ActiveTypeBitset &= ~Mask;
		//ObjUserPreference.ActiveTypeBitset &= ~TMask;
		break;
	}
}




void FRealBandUIManagerImpl::HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		// This will call back into this widget to refresh it
		//EditorModule->SetCurrentRegistrySource(*Item);
	}
}

FReply FRealBandUIManagerImpl::OnLocal()
{
	//pSettingsBox->SetVisibility(EVisibility::Collapsed);
	pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
	pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
	pImport->SetVisibility(EVisibility::Visible);
	pApplyButton->SetVisibility(EVisibility::Hidden);
	return FReply::Handled();
}

FReply FRealBandUIManagerImpl::LaunchSettings()
{
	
//	pSettingsWindow->SetVisibility(EVisibility::Visible);
	//pSettingsBox->SetVisibility(EVisibility::Visible);
	pImport->SetVisibility(EVisibility::Collapsed);
	pFRealBandAssetLoader->SetVisibility(EVisibility::Collapsed);
	pRealBandImportSettings->SetVisibility(EVisibility::Visible);
	pApplyButton->SetVisibility(EVisibility::Visible);

	//pAssetFolderPath->SetText(FText::FromString("C:\\Assets\\RealBand_bunddle_Asset\\RealBand_bunddle_Asset"));
	return FReply::Handled();
}


FReply FRealBandUIManagerImpl::OnImportBtnClicked()
{
	// Get the selected asset and add it to content browser
	pImport->SetVisibility(EVisibility::Visible);
	const TSharedPtr<SAssetView>& pAssetView = pFRealBandAssetLoader->GetAssetView();

	TArray<FContentBrowserItem> AssetsSelected = pAssetView->GetSelectedItems();

	TArray<FName> AssetList;
	for (FContentBrowserItem Item : AssetsSelected)
	{
		if (Item.IsValid())
		{
			FName AssetPath = Item.GetItemName();
			AssetList.Add(AssetPath);
			//PendingSyncItems.SelectedVirtualPaths.Add(Item->GetItem().GetVirtualPath());
		}
	}

	
	pFRealBandAssetImporter->ImportSelectedAssets(AssetList, ObjUserPreference);
	
	return FReply::Handled();
}


FReply FRealBandUIManagerImpl::ApplySettings()
{
	if(pFRealBandAssetLoader->GetVisibility() == EVisibility::Visible)
		return FReply::Handled();

	
	//bool isActive = false;
	
	//const uint16 Mask = 1 << SELECTOPTIONS::FORMAT_GLM;
	//isActive = ActiveTypeBitset & Mask;
	pApplyButton->SetVisibility(EVisibility::Hidden);
	pRealBandImportSettings->SetVisibility(EVisibility::Hidden);
	//pSettingsBox->SetVisibility(EVisibility::Collapsed);
	pImport->SetVisibility(EVisibility::Visible);
	//FText AssetFolderPath = pAssetFolderPath->GetText();
	//FString fPath = AssetFolderPath.ToString();
	//FText AssetFolderPath = FText::FromString(ObjUserPreference.FolderPath.c_str());
	FText AssetFolderPath = FText::FromString("C:\\Assets\\RealBand_bunddle_Asset\\RealBand_bunddle_Asset");
	pFRealBandAssetImporter->CreateTexturesFromAssets(AssetFolderPath);
	pDialogMainWindow->BringToFront();
	return FReply::Handled();
}

FReply FRealBandUIManagerImpl::ResetSettings()
{

	pAssetPath->ClearContent();
//	pAssetFolderPath->ClearContent();
	pAssetFolderPath->SetText(FText::FromString(""));
	
	for (TSharedPtr<SCheckBox> ptrChkBox : OptionsFormat)
	{
		ptrChkBox->SetIsChecked(ECheckBoxState::Unchecked);
	}
	
	ObjUserPreference.ActiveTypeBitset &= ~(0xFF);
	ObjUserPreference.ActiveTextypeBitset &= ~(0xFF);

	return FReply::Handled();
}


void FRealBandUIManagerImpl::UpdateCollections(const FName & CollectionName,const FName & PackageDir)
{
	TArray<FName> PackagePaths;
	PackagePaths.Add(PackageDir);

	bool bAdded = false;
	TArray<FAssetData> AssetsInPackages;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry* AssetRegistry = &AssetRegistryModule.Get();
	FARFilter Filter;
	Filter.bIncludeOnlyOnDiskAssets = false;
	Filter.PackagePaths = PackagePaths;
	
	TArray<FAssetData> AsstInPackages;
	AssetRegistry->GetAssets(Filter, AsstInPackages);
	TSet<FName> ObjectPathsToAddToCollection;
	bool bCollect = false;
	for (const FAssetData& AsstData : AsstInPackages)
	{
		ObjectPathsToAddToCollection.Add(AsstData.ObjectPath);
	}
	if (ObjectPathsToAddToCollection.Num() > 0)
	{
		bCollect = FCollectionManagerModule::GetModule().Get().AddToCollection(CollectionName, ECollectionShareType::CST_Local, ObjectPathsToAddToCollection.Array());
		if (bCollect)
		{
			if (!FCollectionManagerModule::GetModule().Get().SaveCollection(CollectionName, ECollectionShareType::CST_Local))
			{
				UE_LOG(LogManager, Display, TEXT("Failed to Save Collection"));
			}
		}
	}
	if (!FCollectionManagerModule::GetModule().Get().UpdateCollection(CollectionName, ECollectionShareType::CST_Local))
	{
		UE_LOG(LogManager, Display, TEXT("Failed to Update Collection"));
	}
}


FReply FRealBandUIManagerImpl::LaunchOpenFileDialog()
{

	
	//		void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (pDialogMainWindow)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				FString FolderPath;
				FString DefaultPath("C:\\Temp");
				// set a default path
				FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_EXPORT, DefaultPath);
				DesktopPlatform->OpenDirectoryDialog(pDialogMainWindow->GetNativeWindow()->GetOSWindowHandle(),
					                                 DefaultPath,
					                                 DefaultPath, 
					                                 FolderPath);
				FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_EXPORT, FolderPath);
				if (pAssetPath)
				{
					pAssetPath->SetText(FText::FromString(FolderPath));
					pAssetFolderPath->SetText(FText::FromString(FolderPath));
				}

				
	//			DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
	//	}
	//}

	return FReply::Handled();
}


void FRealBandUIManagerImpl::OnFolderPathChanged(const FText& ChangedText)
{
	pAssetFolderPath->SetText(ChangedText);
	//return FReply::Handled();
}

TSharedRef<SDockTab> FRealBandUIManagerImpl::CreatRealBandTab(const FSpawnTabArgs& Args)
{
	FWebBrowserInitSettings browserInitSettings = FWebBrowserInitSettings();
	IWebBrowserModule::Get().CustomInitialize(browserInitSettings);
	TSharedPtr<SWebBrowser> PluginWebBrowser;
//	WindowSettings.InitialURL = FinalUrl;
	WindowSettings.BrowserFrameRate = 60;

	if (IWebBrowserModule::IsAvailable() && IWebBrowserModule::Get().IsWebModuleAvailable())
	{
		IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
		Browser = WebBrowserSingleton->CreateBrowserWindow(WindowSettings);
		PluginWebBrowser = SAssignNew(WebBrowserWidget, SWebBrowser, Browser)
			.ShowAddressBar(false)
			.ShowControls(false);
	}

	//
	return SAssignNew(LocalBrowserDock, SDockTab);
}


FReply FRealBandUIManagerImpl::SavePreferences(TWeakPtr< SMenuAnchor > iPref)
{
	return FReply::Handled();
}

TStrongObjectPtr<UDatasmithGLTFImportOptions>& FRealBandUIManagerImpl::GetOrCreateGLTFImportOptions()
{
	if (!ImportOptions.IsValid() && IsInGameThread())
	{
	//	ImportOptions = Datasmith::MakeOptions<UDatasmithGLTFImportOptions>();
	}
	return ImportOptions;
}

#undef LOCTEXT_NAMESPACE