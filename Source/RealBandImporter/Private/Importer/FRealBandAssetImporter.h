#pragma once

#include "CoreMinimal.h"

#include "IContentBrowserSingleton.h"
#include "RealBandCommon.h"

#include <set>
class FAssetViewItem;

DECLARE_LOG_CATEGORY_EXTERN(LogCustom, Log, All);



class FRealBandAssetImporter : FNoncopyable
{
private:
	FAssetPickerConfig ConfigPicker;
	int AssetCount;
	void UpdateCollections(const FName& CollectionName, const FName& PackageDir);
	void GetFilesForImport(const std::string & iFolderPath, const char* iFinalRegExp, std::set<std::string>& oFilesForImport);
	void ImportTextures(const std::set<std::string> &);
public:
	FRealBandAssetImporter();
	void GetAssetConfig(FAssetPickerConfig& oConfigPicker);
	void Init();
	void ImportFbx(const TArray<FString>&, bool bObj = false);
	void ImportGlm(const TArray<FString>&);
	void CreateTexturesFromAssets(const FText &);
	void GetAssetsCount(int oAssetCount) { oAssetCount = AssetCount; }
	void ImportSelectedAssets(const TArray<FName> &, const USRPREFERENCE & );
	void ApplyUserPreferences(const USRPREFERENCE &);
	//void ImportSelectedAssets(const TArray<TSharedPtr<FAssetViewItem&>> & iAssetList) const;
};