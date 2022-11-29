

#include "FRealBandAssetLoader.h"

//Slate
#include "Framework/Application/SlateApplication.h"
#include "SAssetView.h"
#include "SAssetSearchBox.h"
#include "Editor.h"
// Widgets
#include "Widgets/Input/SComboButton.h"

#include "FrontendFilters.h"

#include "ContentBrowserModule.h"

// Asset
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "AssetViewUtils.h"


#define LOCTEXT_NAMESPACE "RealBandAssetPreviewWidget"


FRealBandAssetLoader::~FRealBandAssetLoader()
{
	
}


void FRealBandAssetLoader::Construct(const FArguments& InArgs)
{
	OnAssetsActivated = InArgs._AssetPickerConfig.OnAssetsActivated;
	OnAssetSelected = InArgs._AssetPickerConfig.OnAssetSelected;
	OnAssetEnterPressed = InArgs._AssetPickerConfig.OnAssetEnterPressed;

	if (InArgs._AssetPickerConfig.bFocusSearchBoxWhenOpened)
	{
		RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &FRealBandAssetLoader::SetFocusPostConstruct));
	}


	TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox);
	ChildSlot
		[
			VerticalBox
		];

	FName AssetPath(InArgs._AssetPathConfig.DefaultPath);
	CurrentSourcesData = FSourcesData(InArgs._AssetPickerConfig.Filter.PackagePaths, InArgs._AssetPickerConfig.Collections);
	// Make RealBand-specific filter
	CurrentBackendFilter = InArgs._AssetPickerConfig.Filter;
	CurrentBackendFilter.PackagePaths.Reset();
	// Filters
		TArray<UClass*> FilterClassList;
	for (auto Iter = CurrentBackendFilter.ClassNames.CreateIterator(); Iter; ++Iter)
	{
		FName ClassName = (*Iter);
		UClass* FilterClass = FindObject<UClass>(ANY_PACKAGE, *ClassName.ToString());
		if (FilterClass)
		{
			FilterClassList.AddUnique(FilterClass);
		}
	}

	VerticalBox->AddSlot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(SHorizontalBox)
			// Search
		    + SHorizontalBox::Slot()
		      //.Padding(0, 1, 0, 0)
		      .FillWidth(1.0f)
		      [
				  SAssignNew(SearchBoxPtr, SAssetSearchBox)
				  .HintText(this, &FRealBandAssetLoader::GetSearchAssetsHintText)
			      //.HintText(NSLOCTEXT("ContentBrowser", "SearchBoxHint", "Search Assets"))
		          .OnTextCommitted(this, &FRealBandAssetLoader::OnSearchBoxCommitted)
				  .OnTextChanged(this, &FRealBandAssetLoader::OnSearchBoxChanged)
		          .DelayChangeNotificationsWhileTyping(true)
		          .OnKeyDownHandler(this, &FRealBandAssetLoader::HandleKeyDownFromSearchBox)
		          .OnAssetSearchBoxSuggestionFilter(this, &FRealBandAssetLoader::OnAssetSearchSuggestionFilter)
		          .OnAssetSearchBoxSuggestionChosen(this, &FRealBandAssetLoader::OnAssetSearchSuggestionChosen)
		          .Visibility(EVisibility::Visible)
		          //.PossibleSuggestions(PossibleSuggestions)
		          .MustMatchPossibleSuggestions(false)
		          
		      ]
	        + SHorizontalBox::Slot()
              .AutoWidth()
				[
					SAssignNew(FilterComboButtonPtr, SComboButton)
					.ComboButtonStyle(&FAppStyle::Get().GetWidgetStyle<FComboButtonStyle>("SimpleComboButton"))
				    .ToolTipText(LOCTEXT("AddFilterToolTip", "Add an asset filter."))
				    .HasDownArrow(false)
				    .AddMetaData<FTagMetaData>(FTagMetaData(TEXT("ContentBrowserFiltersCombo")))
				    .ButtonContent()
				     [
					     SNew(SImage)
					    .ColorAndOpacity(FSlateColor::UseForeground())
				        .Image(FAppStyle::Get().GetBrush("Icons.Filter"))
				     ]
			    ]
		  ];
	
	FrontendFilters = MakeShareable(new FAssetFilterCollectionType());

	VerticalBox->AddSlot()
		.FillHeight(1.f)
		[
			SAssignNew(AssetViewPtr, SAssetView)
			.InitialViewType(EAssetViewType::Tile)
		    .InitialSourcesData(CurrentSourcesData)
		    .InitialThumbnailSize(EThumbnailSize::Medium)
		    .OnItemsActivated(this, &FRealBandAssetLoader::HandleItemsActivated)
		    .OnSearchOptionsChanged(this, &FRealBandAssetLoader::HandleSearchSettingsChanged)
		    .InitialCategoryFilter(EContentBrowserItemCategoryFilter::IncludeAssets)
		    .InitialAssetSelection(nullptr)
		    .FilterRecursivelyWithBackendFilter(true)
		    .FrontendFilters(FrontendFilters)
		    .OnSearchOptionsChanged(this, &FRealBandAssetLoader::HandleAssetViewSearchOptionsChanged)
		    .OnItemSelectionChanged(this, &FRealBandAssetLoader::HandleItemSelectionChanged)
		    .OwningContentBrowser(nullptr)
		    .AllowThumbnailEditMode(true)
		    .FillEmptySpaceInTileView(true)
		    
		    
		       
		];

	if ( AssetViewPtr.IsValid() )
	{
		const FSourcesData SourcesData = AssetViewPtr->GetSourcesData();;
		TextFilter = MakeShareable(new FFrontendFilter_Text());
		bool bClassNamesProvided = (InArgs._AssetPickerConfig.Filter.ClassNames.Num() != 1);
		TextFilter->SetIncludeClassName(bClassNamesProvided || AssetViewPtr->IsIncludingClassNames());
		TextFilter->SetIncludeAssetPath(AssetViewPtr->IsIncludingAssetPaths());
		TextFilter->SetIncludeCollectionNames(AssetViewPtr->IsIncludingCollectionNames());
	
		
		AssetViewPtr->SetSourcesData(CurrentSourcesData);
		AssetViewPtr->SetUserSearching(true);
		// Initialize the search options
		HandleAssetViewSearchOptionsChanged();
	}

	
}



void FRealBandAssetLoader::HandleAssetViewSearchOptionsChanged()
{
	TextFilter->SetIncludeClassName(AssetViewPtr->IsIncludingClassNames());
	TextFilter->SetIncludeAssetPath(AssetViewPtr->IsIncludingAssetPaths());
	TextFilter->SetIncludeCollectionNames(AssetViewPtr->IsIncludingCollectionNames());
}

EActiveTimerReturnType FRealBandAssetLoader::SetFocusPostConstruct(double InCurrentTime, float InDeltaTime)
{
	if (SearchBoxPtr.IsValid())
	{
		FWidgetPath WidgetToFocusPath;
		FSlateApplication::Get().GeneratePathToWidgetUnchecked(SearchBoxPtr.ToSharedRef(), WidgetToFocusPath);
		FSlateApplication::Get().SetKeyboardFocus(WidgetToFocusPath, EFocusCause::SetDirectly);
		WidgetToFocusPath.GetWindow()->SetWidgetToFocusOnActivate(SearchBoxPtr);

		return EActiveTimerReturnType::Stop;
	}
	
	return EActiveTimerReturnType::Continue;
}


void FRealBandAssetLoader::OnAssetSearchSuggestionFilter(const FText& SearchText, TArray<FAssetSearchBoxSuggestion>& PossibleSuggestions, FText& SuggestionHighlightText) const
{
	check(PossibleSuggestions.Num() == 0);
	FString FilterKey;
	FString FilterValue;
	ExtractAssetSearchFilterTerms(SearchText, &FilterKey, &FilterValue, nullptr);

	auto PassesValueFilter = [&FilterValue](const FString& InOther)
	{
		return FilterValue.IsEmpty() || InOther.Contains(FilterValue);
	};

	
	if (FilterKey.IsEmpty() || (FilterKey == TEXT("Collection") || FilterKey == TEXT("Tag")))
	{
		ICollectionManager& CollectionManager = FCollectionManagerModule::GetModule().Get();

		TArray<FCollectionNameType> AllCollections;
		CollectionManager.GetCollections(AllCollections);

		TArray<FName> PackagePaths;
		PackagePaths.Add(FName("/Engine/RealBand"));

		bool bAdded = false;
		TArray<FAssetData> AssetsInPackages;
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		IAssetRegistry* AssetRegistry = &AssetRegistryModule.Get();
		FARFilter Filter;
		Filter.bIncludeOnlyOnDiskAssets = false;
		Filter.PackagePaths = PackagePaths;
		TArray<FAssetData> AsstInPackages;
		AssetRegistry->GetAssets(Filter, AsstInPackages);
		const FText CollectionsCategoryName = NSLOCTEXT("ContentBrowser", "CollectionsCategoryName", "Collections");
		for (FAssetData& ObjectAsset : AsstInPackages)
		{
			FString AssetName = ObjectAsset.AssetName.GetPlainNameString();
			PossibleSuggestions.Add(FAssetSearchBoxSuggestion{ MoveTemp(AssetName), FText::FromName(MoveTemp(ObjectAsset.AssetName))
				                                             , CollectionsCategoryName });
		}
	}

	

	SuggestionHighlightText = FText::FromString(FilterValue);

}

void FRealBandAssetLoader::HandleSearchSettingsChanged()
{
//	bool bClassNamesProvided = (FilterListPtr.IsValid() ? FilterListPtr->GetInitialClassFilters().Num() != 1 : false);
	bool bClassNamesProvided = true;
	TextFilter->SetIncludeClassName(bClassNamesProvided || AssetViewPtr->IsIncludingClassNames());
	TextFilter->SetIncludeAssetPath(AssetViewPtr->IsIncludingAssetPaths());
	TextFilter->SetIncludeCollectionNames(AssetViewPtr->IsIncludingCollectionNames());
}

void FRealBandAssetLoader::OnSearchBoxChanged(const FText& InSearchText)
{
	SetSearchBoxText(InSearchText);
	FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	ContentBrowserModule.GetOnSearchBoxChanged().Broadcast(InSearchText, false);
}


void FRealBandAssetLoader::HandleItemSelectionChanged(const FContentBrowserItem& InSelectedItem, ESelectInfo::Type InSelectInfo)
{
	bool bClassNamesProvided = true;
	
	if (InSelectInfo != ESelectInfo::Direct)
	{
		FAssetData ItemAssetData;
		InSelectedItem.Legacy_TryGetAssetData(ItemAssetData);
		
		UObject *pAsset = ItemAssetData.GetAsset();
		_ImportedObjects.Add(pAsset);
	}
	//TArray<UObject*>  ImportedObjects;
	//ImportedObjects.Append(InSelectedItem)
	//FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	//ContentBrowserModule.Get().SyncBrowserToAssets(ImportedObjects);
}

void FRealBandAssetLoader::ImportSelectedAssets()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(_ImportedObjects);
}


FText FRealBandAssetLoader::OnAssetSearchSuggestionChosen(const FText& SearchText, const FString& Suggestion) const
{
	int32 SuggestionInsertionIndex = 0;
	ExtractAssetSearchFilterTerms(SearchText, nullptr, nullptr, &SuggestionInsertionIndex);

	FString SearchString = SearchText.ToString();
	SearchString.RemoveAt(SuggestionInsertionIndex, SearchString.Len() - SuggestionInsertionIndex, false);
	SearchString.Append(Suggestion);

	return FText::FromString(SearchString);
}


void FRealBandAssetLoader::ExtractAssetSearchFilterTerms(const FText& SearchText, FString* OutFilterKey, 
	                                                     FString* OutFilterValue, int32* OutSuggestionInsertionIndex) const
{
	const FString SearchString = SearchText.ToString();

	if (OutFilterKey)
	{
		OutFilterKey->Reset();
	}
	if (OutFilterValue)
	{
		OutFilterValue->Reset();
	}
	if (OutSuggestionInsertionIndex)
	{
		*OutSuggestionInsertionIndex = SearchString.Len();
	}

	// Build the search filter terms so that we can inspect the tokens
	FTextFilterExpressionEvaluator LocalFilter(ETextFilterExpressionEvaluatorMode::Complex);
	LocalFilter.SetFilterText(SearchText);

	// Inspect the tokens to see what the last part of the search term was
	// If it was a key->value pair then we'll use that to control what kinds of results we show
	// For anything else we just use the text from the last token as our filter term to allow incremental auto-complete
	const TArray<FExpressionToken>& FilterTokens = LocalFilter.GetFilterExpressionTokens();
	if (FilterTokens.Num() > 0)
	{
		const FExpressionToken& LastToken = FilterTokens.Last();

		// If the last token is a text token, then consider it as a value and walk back to see if we also have a key
		if (LastToken.Node.Cast<TextFilterExpressionParser::FTextToken>())
		{
			if (OutFilterValue)
			{
				*OutFilterValue = LastToken.Context.GetString();
			}
			if (OutSuggestionInsertionIndex)
			{
				*OutSuggestionInsertionIndex = FMath::Min(*OutSuggestionInsertionIndex, LastToken.Context.GetCharacterIndex());
			}

			if (FilterTokens.IsValidIndex(FilterTokens.Num() - 2))
			{
				const FExpressionToken& ComparisonToken = FilterTokens[FilterTokens.Num() - 2];
				if (ComparisonToken.Node.Cast<TextFilterExpressionParser::FEqual>())
				{
					if (FilterTokens.IsValidIndex(FilterTokens.Num() - 3))
					{
						const FExpressionToken& KeyToken = FilterTokens[FilterTokens.Num() - 3];
						if (KeyToken.Node.Cast<TextFilterExpressionParser::FTextToken>())
						{
							if (OutFilterKey)
							{
								*OutFilterKey = KeyToken.Context.GetString();
							}
							if (OutSuggestionInsertionIndex)
							{
								*OutSuggestionInsertionIndex = FMath::Min(*OutSuggestionInsertionIndex, KeyToken.Context.GetCharacterIndex());
							}
						}
					}
				}
			}
		}

		// If the last token is a comparison operator, then walk back and see if we have a key
		else if (LastToken.Node.Cast<TextFilterExpressionParser::FEqual>())
		{
			if (FilterTokens.IsValidIndex(FilterTokens.Num() - 2))
			{
				const FExpressionToken& KeyToken = FilterTokens[FilterTokens.Num() - 2];
				if (KeyToken.Node.Cast<TextFilterExpressionParser::FTextToken>())
				{
					if (OutFilterKey)
					{
						*OutFilterKey = KeyToken.Context.GetString();
					}
					if (OutSuggestionInsertionIndex)
					{
						*OutSuggestionInsertionIndex = FMath::Min(*OutSuggestionInsertionIndex, KeyToken.Context.GetCharacterIndex());
					}
				}
			}
		}
	}
}




// Begin of SWidgetInterface
void FRealBandAssetLoader::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) 
{
	int test = 1;
	//AssetViewPtr->RequestQuickFrontendListRefresh();
}

FText FRealBandAssetLoader::GetSearchAssetsHintText() const
{
	/*if (PathViewPtr.IsValid())
	{
		TArray<FContentBrowserItem> Paths = PathViewPtr->GetSelectedFolderItems();
		if (Paths.Num() > 0)
		{
			FString SearchHint = NSLOCTEXT("ContentBrowser", "SearchBoxPartialHint", "Search").ToString();
			SearchHint += TEXT(" ");
			for (int32 i = 0; i < Paths.Num(); i++)
			{
				SearchHint += Paths[i].GetDisplayName().ToString();

				if (i + 1 < Paths.Num())
				{
					SearchHint += ", ";
				}
			}

			return FText::FromString(SearchHint);
		}
	}*/

	return NSLOCTEXT("ContentBrowser", "SearchBoxHint", "Search Assets");
}


void FRealBandAssetLoader::OnSearchBoxCommitted(const FText& InSearchText, ETextCommit::Type CommitInfo)
{
	SetSearchBoxText(InSearchText);
	//AssetViewPtr->AdjustActiveSelection(-1);

	
	//if (CommitInfo == ETextCommit::OnEnter)
	//{
	//	TArray<FContentBrowserItem> SelectionSet = AssetViewPtr->GetSelectedFileItems();
	//	SelectionSet = AssetViewPtr->GetSelectedItems();
	//	
	//	TArray<FAssetData> SelectionAssetSet = AssetViewPtr->GetSelectedAssets();
	//	//SelectionSet = AssetViewPtr->GetSelectedViewItems();
	//	if (SelectionSet.Num() == 0)
	//	{
	//		AssetViewPtr->AdjustActiveSelection(1);
	//		//SelectionSet = AssetViewPtr->GetSele();
	//	}
	//	HandleItemsActivated(SelectionSet, EAssetTypeActivationMethod::Opened);
	//}
	
}

void FRealBandAssetLoader::SetSearchBoxText(const FText& InSearchText)
{
	// Has anything changed? (need to test case as the operators are case-sensitive)
	if (!InSearchText.ToString().Equals(TextFilter->GetRawFilterText().ToString(), ESearchCase::CaseSensitive))
	{
		TextFilter->SetRawFilterText(InSearchText);
		if (InSearchText.IsEmpty())
		{
			FrontendFilters->Remove(TextFilter);
			AssetViewPtr->SetUserSearching(false);
		}
		else
		{
			FrontendFilters->Add(TextFilter);
			AssetViewPtr->SetUserSearching(true);
		}
	}
}

void FRealBandAssetLoader::HandleItemsActivated(TArrayView<const FContentBrowserItem> ActivatedItems, EAssetTypeActivationMethod::Type ActivationMethod)
{
	FContentBrowserItem FirstActivatedFolder;

	TArray<FAssetData> ActivatedAssets;
	for (const FContentBrowserItem& ActivatedItem : ActivatedItems)
	{
		if (ActivatedItem.IsFile())
		{
			FAssetData ItemAssetData;
			if (ActivatedItem.Legacy_TryGetAssetData(ItemAssetData))
			{
				ActivatedAssets.Add(MoveTemp(ItemAssetData));
			}
		}

		if (ActivatedItem.IsFolder() && !FirstActivatedFolder.IsValid())
		{
			FirstActivatedFolder = ActivatedItem;
		}
	}

	if (ActivatedAssets.Num() == 0)
	{
		return;
	}

	if (ActivationMethod == EAssetTypeActivationMethod::Opened)
	{
		OnAssetEnterPressed.ExecuteIfBound(ActivatedAssets);
	}

	OnAssetsActivated.ExecuteIfBound(ActivatedAssets, ActivationMethod);
}

FReply FRealBandAssetLoader::HandleKeyDownFromSearchBox(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	// Hide the filter list
	if (FilterComboButtonPtr.IsValid())
	{
		FilterComboButtonPtr->SetIsOpen(false);
	}

	// Up and down move thru the filtered list
	int32 SelectionDelta = 0;

	if (InKeyEvent.GetKey() == EKeys::Up)
	{
		SelectionDelta = -1;
	}
	else if (InKeyEvent.GetKey() == EKeys::Down)
	{
		SelectionDelta = +1;
	}

	if (SelectionDelta != 0)
	{
		AssetViewPtr->AdjustActiveSelection(SelectionDelta);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}


FReply FRealBandAssetLoader::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

void FRealBandAssetLoader::SyncToAssets(const TArray<FAssetData>& AssetDataList)
{
	AssetViewPtr->SyncToLegacy(AssetDataList, TArray<FString>());
}

#undef LOCTEXT_NAMESPACE