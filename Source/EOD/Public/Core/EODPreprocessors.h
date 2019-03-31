// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#define COLLISION_COMBAT ECC_GameTraceChannel1

#define TEST_CODE_ENABLED 1
#define DEBUG_SHAPES_ENABLED 0
#define MESSAGE_LOGGING_ENABLED 1

#define EOD_TEST_CODE_ENABLED 1
#define EOD_MESSAGE_LOGGING_ENABLED 1
#define EOD_SCREEN_MESSAGES_ENABLED 1
#define EOD_CONSOLE_MESSAGES_ENABLED 1
#define EOD_DRAWING_DEBUG_SHAPES_ENABLED 0


#define SURFACETYPE_FLESH       SurfaceType1
#define SURFACETYPE_METAL       SurfaceType2
#define SURFACETYPE_STONE       SurfaceType3
#define SURFACETYPE_UNDEAD      SurfaceType4
#define SURFACETYPE_WOOD        SurfaceType5
#define SURFACETYPE_WATER       SurfaceType6
#define SURFACETYPE_SNOW        SurfaceType7
#define SURFACETYPE_SAND        SurfaceType8
#define SURFACETYPE_FOLIAGE     SurfaceType9
#define SURFACETYPE_DIRT        SurfaceType10
#define SURFACETYPE_CONCRETE    SurfaceType11


#include "Kismet/KismetSystemLibrary.h"


inline void PrintToScreen(UObject* WorldContextObject, const FString& Message, float Duration = 2.0f)
{
#if EOD_MESSAGE_LOGGING_ENABLED
#if EOD_SCREEN_MESSAGES_ENABLED
	UKismetSystemLibrary::PrintString(WorldContextObject, Message, true, false, FLinearColor((0.0), (0.66), (1.0)), Duration);
#endif // EOD_SCREEN_MESSAGES_ENABLED
#endif // EOD_MESSAGE_LOGGING_ENABLED
}

inline void PrintToConsole(UObject* WorldContextObject, const FString& Message, float Duration = 2.0f)
{
#if EOD_MESSAGE_LOGGING_ENABLED
#if EOD_CONSOLE_MESSAGES_ENABLED
	UKismetSystemLibrary::PrintString(WorldContextObject, Message, false, true, FLinearColor((0.0), (0.66), (1.0)), Duration);
#endif // EOD_CONSOLE_MESSAGES_ENABLED
#endif // EOD_MESSAGE_LOGGING_ENABLED
}


inline void PrintEverywhere(UObject* WorldContextObject, const FString& Message, float Duration = 2.0f)
{
#if EOD_MESSAGE_LOGGING_ENABLED
	bool bLogScreen = false, bLogConsole = false;
#if EOD_SCREEN_MESSAGES_ENABLED
	bLogScreen = true;
#endif // EOD_SCREEN_MESSAGES_ENABLED
#if EOD_CONSOLE_MESSAGES_ENABLED
	bLogConsole = true;
#endif // EOD_CONSOLE_MESSAGES_ENABLED
	UKismetSystemLibrary::PrintString(WorldContextObject, Message, bLogScreen, bLogConsole, FLinearColor((0.0), (0.66), (1.0)), Duration);
#endif // EOD_MESSAGE_LOGGING_ENABLED
}

template <typename T>
inline FString EnumToString(const FString& EnumName, const T Value, const FString DefaultString)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (Enum)
	{
		return Enum->GetNameStringByValue((int64)Value);
	}
	return DefaultString;
}
