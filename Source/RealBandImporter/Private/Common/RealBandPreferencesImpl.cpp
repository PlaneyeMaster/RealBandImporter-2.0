
#include "RealBandPreferencesImplh.h"


//RealBandPreferencesImpl::~RealBandPreferencesImpl()
//{
//
//}

void RealBandPreferencesImpl::SavePreferences(const USRPREFERENCE& iPreference)
{

}

void RealBandPreferencesImpl::LoadPreferences(USRPREFERENCE& oPreference)
{
    oPreference.ConnectPath = _userPreferences.ConnectPath;
    oPreference.FolderPath = _userPreferences.FolderPath;
    oPreference.Format = _userPreferences.Format;
    oPreference.Project = _userPreferences.Project;
    oPreference.TexLevel = _userPreferences.TexLevel;
    oPreference.TextureType = _userPreferences.TextureType;
}

void RealBandPreferencesImpl::Reset()
{
   
}