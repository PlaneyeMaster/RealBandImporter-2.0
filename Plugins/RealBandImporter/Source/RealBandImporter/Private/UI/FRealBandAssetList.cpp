

#include "FRealBandAssetList.h"
#include "Framework/Application/SlateApplication.h"
#include "Editor.h"
#include "ImageCore.h"

//#include "Templates/WidgetTemplateImageClass.h"
#include "Components/Image.h"

FRealBandAssetList::~FRealBandAssetList()
{

}




void FRealBandAssetList::Construct(const FArguments& InArgs)
{
	//FString DefaultImage = TEXT("Dummy");
	//UObject* ImageResource = FindObject<UObject>(ANY_PACKAGE, *DefaultImage);
	


	TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox);

	ChildSlot
		[
			VerticalBox
		];

	VerticalBox->AddSlot()
		.FillHeight(1.f)
		[

	        SNew(SListView<TSharedPtr<struct FImage> >)
		    .ListItemsSource(&ImageData)
		    .SelectionMode(ESelectionMode::None)
		];

}



