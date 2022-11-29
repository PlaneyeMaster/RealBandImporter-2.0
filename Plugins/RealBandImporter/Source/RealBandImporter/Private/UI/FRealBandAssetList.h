#pragma once

#include "CoreMinimal.h"

//class header
//#include "FRealBandxportClass.h"
//Slate
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SListView.h"

typedef struct FImage FIMAGE;
//class SListView;
typedef SListView< TSharedPtr<FIMAGE> > ImageListType;

class FRealBandAssetList :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(FRealBandAssetList) {}

	
	SLATE_END_ARGS()

	virtual ~FRealBandAssetList();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	

private:

	TArray<TSharedPtr<FIMAGE>> ImageData;
	/** The List view widget */
	ImageListType ListViewPtr;
	
};