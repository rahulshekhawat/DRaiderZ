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

void UFbxImportUtilities::ImportRaiderzFbxFile()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bSuccess = false;
	TArray<FString> SelectedFiles;
	if (DesktopPlatform)
	{
		const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		bSuccess = DesktopPlatform->OpenFileDialog(
			ParentWindowWindowHandle,
			TEXT("Select FBX file to import"),
			URaiderzXmlUtilities::DarkRaiderzAssetPath,
			TEXT(""),
			TEXT("FBX files|*.fbx"),
			EFileDialogFlags::None,
			SelectedFiles
		);
	}

	if (!bSuccess || SelectedFiles.Num() == 0)
	{
		PrintWarning(TEXT("User failed to select any files!"));
		return;
	}
}

void UFbxImportUtilities::ImportRaiderzFbxFilesFromFolder()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bSuccess = false;
	FString SelectedFolderPath;
	if (DesktopPlatform)
	{
		const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		bSuccess = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowWindowHandle,
			TEXT("Select fbx models folder"),
			TEXT("F:/Game Dev/asset_dest/Model"),
			SelectedFolderPath
		);
	}

	if (!bSuccess || SelectedFolderPath == TEXT(""))
	{
		PrintWarning(TEXT("User failed to select any folder!"));
		return;
	}
}
