#pragma once

#include "IRealBandPreference.h"

class RealBandPreferencesImpl : public IRealBandPreference
{

public:
	//~RealBandPreferencesImpl();
	void SavePreferences(const USRPREFERENCE& iPreference) override;
	void LoadPreferences(USRPREFERENCE& oPreference) override;
	void Reset() override;
private:
	USRPREFERENCE _userPreferences;
};
