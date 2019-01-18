// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"


/** Module interface for this game's loading screens */
class IEODLoadingScreenModule : public IModuleInterface
{
public:
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen() = 0;
};
