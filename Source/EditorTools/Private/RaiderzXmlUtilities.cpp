// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "RaiderzXmlUtilities.h"

const FString URaiderzXmlUtilities::DataFolderPath(TEXT("E:/RaiderzAssets/datadump/Data"));
const FString URaiderzXmlUtilities::SoundXmlFilePath(TEXT("E:/RaiderzAssets/datadump/Data/Sound/sound.xml"));
const FString URaiderzXmlUtilities::NPCXmlFilePath(TEXT("E:/RaiderzAssets/datadump/Data/system/npc.xml"));
// FXmlParser can't parse original talent.xml, so it has been replaced with talent_unrealengine.xml which removed a few details that FXmlParser was unable to parse
// const FString URaiderzXmlUtilities::NPCXmlFilePath(TEXT("E:/RaiderzAssets/datadump/Data/system/talent.xml"));
const FString URaiderzXmlUtilities::TalentXmlFilePath(TEXT("E:/RaiderzAssets/talent_unrealengine.xml"));

const FString URaiderzXmlUtilities::EluAnimationXmlExt(TEXT(".elu.animation.xml"));
const FString URaiderzXmlUtilities::EluAnimationEventXmlExt(TEXT(".elu.animationevent.xml"));
const FString URaiderzXmlUtilities::EluAnimationInfoXmlExt(TEXT(".elu.animationInfo.xml"));
const FString URaiderzXmlUtilities::EluAnimationSoundEventXmlExt(TEXT(".elu.animationsoundevent.xml"));
const FString URaiderzXmlUtilities::EluXmlExt(TEXT(".elu.xml"));
const FString URaiderzXmlUtilities::SceneXmlExt(TEXT(".scene.xml"));

URaiderzXmlUtilities::URaiderzXmlUtilities(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}
