// Fill out your copyright notice in the Description page of Project Settings.


#include "RealBandImportSettings.h"
//Widgets
#include "Widgets/SWindow.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

//Editor
#include "LevelEditor.h"
#include "PropertyEditor/Private/SDetailCategoryTableRow.h"
#if WITH_EDITOR
#include "ObjectEditorUtils.h"
#endif // WITH_EDITOR

#include <string>
using namespace std;

#define LOCTEXT_NAMESPACE "RealBandImportSettingsWidget"


URealBandImportSettings::URealBandImportSettings()
{
	
}

void URealBandImportSettings::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChangede)
{
	int test = 1;
}

void RealBandImportSettingsUI::OnPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
{
	
	const FName propName = PropertyChangedEvent.GetPropertyName();
	FName categoryName = FObjectEditorUtils::GetCategoryFName(PropertyChangedEvent.Property);
	if (propName == "bImportFBX" )
	{
		if (pRealBandSettings->bImportFBX)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::FORMAT_FBX);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::FORMAT_FBX);
		}
	}
	if (propName == "bImportGLM")
	{
		if (pRealBandSettings->bImportGLM)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::FORMAT_GLM);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::FORMAT_GLM);
		}
	}
	if (propName == "bImportOBJ")
	{
		if (pRealBandSettings->bImportOBJ)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::FORMAT_OBJ);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::FORMAT_OBJ);
		}
	}
	if (propName == "bHigh" )
	{
		if (pRealBandSettings->bHigh)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::HIGH);
					}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::HIGH);
			
		}
	}

	if (propName == "bLow" )
	{
		if (pRealBandSettings->bLow)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::LOW);
			
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::LOW);
		}
	}

	if (propName == "Path")
	{
		objUsrPreference->FolderPath = std::string(TCHAR_TO_UTF8(*(pRealBandSettings->AssetFolder.Path)));
	}

	if (propName == "b2K")
	{
		if (pRealBandSettings->b2K)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::TWO_K);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::TWO_K);
		}
	}

	if (propName == "b4K")
	{
		if (pRealBandSettings->b4K)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::FOUR_K);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::FOUR_K);
		}
	}

	if (propName == "b8K")
	{
		if (pRealBandSettings->b8K)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::EIGHT_K);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::EIGHT_K);
		}
	}

	if (propName == "bDiffuse")
	{
		if (pRealBandSettings->bDiffuse)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::DIFFUSE);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::DIFFUSE);
		}
	}

	if (propName == "bNormal")
	{
		if (pRealBandSettings->bNormal)
		{
			SetOptionBit(ECheckBoxState::Checked, SELECTOPTIONS::NORMAL);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, SELECTOPTIONS::NORMAL);
		}
	}

	if (propName == "bTexHigh")
	{
		if (pRealBandSettings->bTexHigh)
		{
			SetOptionBit(ECheckBoxState::Checked, TEXTUREOPTIONS::TEXHIGH);
		}
		else
		{
			SetOptionBit(ECheckBoxState::Unchecked, TEXTUREOPTIONS::TEXHIGH);
		}
	}

	if (propName == "bTexLow" )
	{
		if (pRealBandSettings->bTexLow)
		{
			//objUsrPreference->Format |= 1 << FORMATOPTIONS::TEXTURES;
			SetOptionBit(ECheckBoxState::Checked, TEXTUREOPTIONS::TEXLOW);
		}
		else
		{
			//objUsrPreference->Format &= 1 << FORMATOPTIONS::TEXTURES;;
			SetOptionBit(ECheckBoxState::Unchecked, TEXTUREOPTIONS::TEXLOW);
		}
	}

}

void RealBandImportSettingsUI::SetOptionBit(ECheckBoxState CheckState, SELECTOPTIONS Type)
{
	const uint16 Mask = 1 << Type;
	//const uint16 TMask = 1 << TexType;
	switch (CheckState)
	{
	case ECheckBoxState::Checked:
		objUsrPreference->ActiveTypeBitset |= Mask;
		break;
	case ECheckBoxState::Unchecked:
		objUsrPreference->ActiveTypeBitset &= ~Mask;
		break;
	default:
		objUsrPreference->ActiveTypeBitset &= ~Mask;
		break;
	}
}


void RealBandImportSettingsUI::SetOptionBit(ECheckBoxState CheckState, TEXTUREOPTIONS Type)
{
	const uint16 Mask = 1 << Type;
	//const uint16 TMask = 1 << TexType;
	switch (CheckState)
	{
	case ECheckBoxState::Checked:
		objUsrPreference->ActiveTextypeBitset |= Mask;
		break;
	case ECheckBoxState::Unchecked:
		objUsrPreference->ActiveTextypeBitset &= ~Mask;
		break;
	default:
		objUsrPreference->ActiveTextypeBitset &= ~Mask;
		break;
	}
}

void RealBandImportSettingsUI::Construct(const FArguments& InArgs)
{
	objUsrPreference = InArgs._ObjPreference;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	pRealBandSettings = GetMutableDefault<URealBandImportSettings>();
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = true;
	//DetailsViewArgs.NotifyHook = this;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ViewIdentifier = NAME_None;
	SettingsDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	//pRealBandSettings = GetMutableDefault<URealBandImportSettings>();
	SettingsDetailsView->SetObject(pRealBandSettings);
	SettingsDetailsView->OnFinishedChangingProperties().AddSP(this, &RealBandImportSettingsUI::OnFinishedChangingProperties);
	//SettingsDetailsView->SetOnObjectArrayChanged(FOnObjectArrayChanged::CreateSP(this, &RealBandImportSettingsUI::OnPropertyViewObjectArrayChanged));
	//pRealBandSettings->NotifyPostChange(EPropertyChangeType::ValueSet);

	

	

	ChildSlot
		[

			SAssignNew(pSettingsWindow, SWindow)
			.Title(FText::FromString("Settings"))
		.Style(&FCoreStyle::Get().GetWidgetStyle<FWindowStyle>("Window"))
		.SizingRule(ESizingRule::Autosized)
		.ClientSize(FVector2D(900, 600))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
				 SAssignNew(pSettingsCanvasBox, SCanvas)
					 + SCanvas::Slot()
				      .HAlign(HAlign_Fill)
				      .VAlign(VAlign_Fill)
	                  .Size(FVector2D(750.0f, 600.0f))
				      .Position(FVector2D(100.0f, 15.0f))
				      [
						  SNew(SVerticalBox)
						  + SVerticalBox::Slot()
				        [
						  SNew(SSplitter)
						  .Orientation(EOrientation::Orient_Vertical)
				           + SSplitter::Slot()
				            .Value(0.8f)
				            [
					            SNew(SSplitter)
					            .Orientation(EOrientation::Orient_Horizontal)
				                + SSplitter::Slot()
				                .Value(0.5f)
				                [
					                SNew(SWidgetSwitcher)
					                + SWidgetSwitcher::Slot()
				                      
				                      .HAlign(HAlign_Fill)
				                      .VAlign(VAlign_Fill)
				                    [
					                   SettingsDetailsView.ToSharedRef()
				                    ]
				                 ]
				             ]
						]
						 
					   ]
					/*	 + SCanvas::Slot()
				 		 .HAlign(HAlign_Fill)
				 		 .VAlign(VAlign_Fill)
				 		 .Size(FVector2D(100.0f, 50.0f))
				 		 .Position(FVector2D(690.0f, 600.0f))
				 		 [
				 			 SNew(SButton)
				 			 .HAlign(HAlign_Center)
				 		 .VAlign(VAlign_Center)
				 		 .Text(FText::FromString("Apply"))
				 		 .OnClicked(this, &RealBandImportSettingsUI::ApplySettings)
				 		 .ToolTipText(LOCTEXT("ApplyButtonTooltip", "Apply settings"))
				 		 ]*/
				 	 + SCanvas::Slot()
				 		 .HAlign(HAlign_Fill)
				 		 .VAlign(VAlign_Fill)
				 		 .Size(FVector2D(100.0f, 50.0f))
				 		 .Position(FVector2D(490.0f, 600.0f))
				 		 [
				 			 SNew(SButton)
				 			 .HAlign(HAlign_Center)
				 		 .VAlign(VAlign_Center)
				 		 .Text(FText::FromString("Reset"))
				 		 .OnClicked(this, &RealBandImportSettingsUI::ResetSettings)
				 		 .ToolTipText(LOCTEXT("ResetButtonTooltip", "Clear all settings"))
				 		 ]
		]
		];

	FCoreUObjectDelegates::FOnObjectPropertyChanged::FDelegate OnPropertyChangedDelegate = FCoreUObjectDelegates::FOnObjectPropertyChanged::FDelegate::CreateRaw(this, &RealBandImportSettingsUI::OnPropertyChanged);
	OnPropertyChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.Add(OnPropertyChangedDelegate);

	// Inital turn all the options ON
	pRealBandSettings->bImportFBX = true;
	pRealBandSettings->bImportGLM = true;
	pRealBandSettings->bImportOBJ = true;
	pRealBandSettings->bHigh = true;
	pRealBandSettings->bLow = true;
	pRealBandSettings->bDiffuse = true;
	pRealBandSettings->bNormal = true;
	pRealBandSettings->b2K = true;
	pRealBandSettings->b4K = true;
	pRealBandSettings->b8K = true;
	pRealBandSettings->bTexHigh = true;
	pRealBandSettings->bTexLow = true;


	//pSettingsWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
	//	{
	//		pSettingsWindow.Reset();
	//		SetVisibility(EVisibility::Collapsed);
	//	}));
}


void RealBandImportSettingsUI::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		/*FGraphPanelSelectionSet CurrentSelection = GetSelectedNodes();
		if (CurrentSelection.Num() == 1)
		{
			for (FGraphPanelSelectionSet::TConstIterator It(CurrentSelection); It; ++It)
			{
				UEnvironmentQueryGraphNode_Test* TestNode = Cast<UEnvironmentQueryGraphNode_Test>(*It);
				UEnvironmentQueryGraphNode_Option* ParentNode = TestNode ? Cast<UEnvironmentQueryGraphNode_Option>(TestNode->ParentNode) : nullptr;

				if (ParentNode)
				{
					ParentNode->CalculateWeights();
					break;
				}
			}
		}*/
	}
}

FReply RealBandImportSettingsUI::ApplySettings()
{
	 
	 return FReply::Handled();
}

FReply RealBandImportSettingsUI::ResetSettings()
{

	pRealBandSettings->bImportFBX = false;
	pRealBandSettings->bImportGLM = false;
	pRealBandSettings->bImportOBJ = false;
	pRealBandSettings->bHigh = false;
	pRealBandSettings->bLow = false;
	pRealBandSettings->bDiffuse = false;
	pRealBandSettings->bNormal = false;
	pRealBandSettings->b2K = false;
	pRealBandSettings->b4K = false;
	pRealBandSettings->b8K = false;
	pRealBandSettings->bTexHigh = false;
	pRealBandSettings->bTexLow = false;
	pRealBandSettings->AssetFolder.Path = TEXT("");
	pRealBandSettings->connectPath = FString(TEXT(""));
	objUsrPreference->ActiveTextypeBitset &= ~(0xFFFF);
	objUsrPreference->ActiveTypeBitset &= ~(0xFFFF);

	return FReply::Handled();
}




#undef LOCTEXT_NAMESPACE