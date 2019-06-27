// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "FbxImportUtilities.h"
#include "EOD.h"
#include "RaiderzXmlUtilities.h"

#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"

UFbxImportUtilities::UFbxImportUtilities(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFbxImportUtilities::ImportFbxFile()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bOpened = false;
	TArray<FString> SelectedFiles;
	FString OpenFilePath;
	if (DesktopPlatform)
	{
		const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		bOpened = DesktopPlatform->OpenFileDialog(
			ParentWindowWindowHandle,
			TEXT("Select FBX file to import"),
			URaiderzXmlUtilities::DarkRaiderzAssetPath,
			TEXT(""),
			TEXT("FBX files|*.fbx"),
			EFileDialogFlags::None,
			SelectedFiles
		);
	}

	if (bOpened)
	{
		for (const FString& File : SelectedFiles)
		{
			PrintLog(File);
		}
	}
	else
	{
		PrintWarning(TEXT("User failed to select any files!"));
	}
}

void UFbxImportUtilities::ImportFbxFilesFromPath(const FString& ImportPath)
{
}
