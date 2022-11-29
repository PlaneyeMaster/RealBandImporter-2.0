#pragma once

#include "CoreMinimal.h"

//class header
//#include "FRealBandxportClass.h"
//Slate
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "AssetData.h"
#include "IContentBrowserSingleton.h"
#include "SourcesData.h"



class SAssetView;
class SAssetSearchBox;
class FFrontendFilter_Text;
class SComboButton;
class SFilterList;
class FAssetViewItem;
struct FAssetSearchBoxSuggestion;



class FRealBandAssetLoader :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(FRealBandAssetLoader) {}

	/** A struct containing details about how the asset picker should behave */
	SLATE_ARGUMENT(FAssetPickerConfig, AssetPickerConfig)
	SLATE_ARGUMENT(FPathPickerConfig, AssetPathConfig)

	SLATE_END_ARGS()

	virtual ~FRealBandAssetLoader();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetAssetFolder(const FString&);
	/** Return the associated AssetView */
	//const TSharedPtr<SAssetView>& GetAssetView() const { return AssetViewPtr; }
	///**
	// * Update the set of input Assets to be only based on the given set of Collections
	// * (or all Assets, if the Collections list is empty)
	// */
	//virtual void UpdateAssetSourceCollections(TArray<FCollectionNameType> Collections);
	///** Handle forwarding picking events. We wrap OnAssetSelected here to prevent 'Direct' selections being identified as user actions */
	//void HandleItemSelectionChanged(const FContentBrowserItem& InSelectedItem, ESelectInfo::Type InSelectInfo);

	/** Handler for when the user double clicks, presses enter, or presses space on an asset */
	void HandleItemsActivated(TArrayView<const FContentBrowserItem> ActivatedItems, EAssetTypeActivationMethod::Type ActivationMethod);

	///** Forces a refresh */
	//void RefreshAssetView(bool bRefreshSources);

	///** @return The text to highlight on the assets  */
	//FText GetHighlightedText() const;

	/** Called by the editable text control when the user commits a text change */
	void OnSearchBoxCommitted(const FText& InSearchText, ETextCommit::Type CommitInfo);

	void OnSearchBoxChanged(const FText& InSearchText);

	void HandleItemSelectionChanged(const FContentBrowserItem& InSelectedItem, ESelectInfo::Type InSelectInfo);

	/** Focuses the search box post-construct */
	EActiveTimerReturnType SetFocusPostConstruct(double InCurrentTime, float InDeltaTime);

	FReply HandleKeyDownFromSearchBox(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);

	void HandleSearchSettingsChanged();

	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	FText OnAssetSearchSuggestionChosen(const FText& SearchText, const FString& Suggestion) const;

	void OnAssetSearchSuggestionFilter(const FText& SearchText, TArray<FAssetSearchBoxSuggestion>& PossibleSuggestions,
		                               FText& SuggestionHighlightText) const;

	void ExtractAssetSearchFilterTerms(const FText& SearchText, FString* OutFilterKey,
		FString* OutFilterValue, int32* OutSuggestionInsertionIndex) const;
	FText GetSearchAssetsHintText() const;

	void HandleAssetViewSearchOptionsChanged();
	

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

	/** Return the associated AssetView */
	const TSharedPtr<SAssetView>& GetAssetView() const { 
		return AssetViewPtr; 
	}


	void ImportSelectedAssets();
private:

	/** The list of FrontendFilters currently applied to the asset view */
	TSharedPtr<FAssetFilterCollectionType> FrontendFilters;

	/** The asset view widget */
	TSharedPtr<SAssetView> AssetViewPtr;
	/** Called to when an asset is selected or the none button is pressed */
	FOnAssetSelected OnAssetSelected;

	/** Called when enter is pressed while an asset is selected */
	FOnAssetEnterPressed OnAssetEnterPressed;

	/** Called when any number of assets are activated */
	FOnAssetsActivated OnAssetsActivated;

	/** The sources data currently used by the picker */
	FSourcesData CurrentSourcesData;

	FSetPathPickerPathsDelegate SetPathsDelegate;

	TSharedPtr< FFrontendFilter_Text > TextFilter;

	/** Need to filter the default AssetView **/
	FARFilter CurrentBackendFilter;

	TSharedPtr<SAssetSearchBox> SearchBoxPtr;

	TSharedPtr<SComboButton> FilterComboButtonPtr;

	/** The filter list */
	TSharedPtr<SFilterList> FilterListPtr;
	
	TArray<UObject*>  _ImportedObjects;

	void SetSearchBoxText(const FText& InSearchText);

	void SyncToAssets(const TArray<FAssetData>& AssetDataList);

	
};