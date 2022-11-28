// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//core
#include "CoreGlobals.h"
#include "CoreMinimal.h"
#include "..\..\Public\RealBandCommon.h"
/**
 * 
 */
class REALBAND IRealBandPreference:public TSharedFromThis<IRealBandPreference>
//class REALBAND IRealBandPreference :public IUnknown
{
public:

	/**
	 * Create Preferences 
	 *
	 * @param iPreference	User Preference Object 
	 */

	virtual void SavePreferences(const USRPREFERENCE & iPreference) = 0;

	/**
	 * Get Preferences
	 *
	 * @param oPreference	Out Preference Object 
	 */

	virtual void LoadPreferences(USRPREFERENCE& oPreference) = 0;

	/**
	 * Get Preferences
	 *
	 * @param oPreference	Out Preference Object
	 */

	virtual void Reset() = 0;


	IRealBandPreference();
	virtual ~IRealBandPreference() {};
};
