// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "EluImporter.h"
#include "EOD.h"
#include "EluLibrary.h"
#include "EluMeshNodeLoader.h"
#include "RaiderzXmlUtilities.h"

#include "Misc/FileHelper.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"


UEluImporter::UEluImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEluImporter::ImportEluFile()
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
			URaiderzXmlUtilities::DataFolderPath,
			TEXT(""),
			TEXT("RaiderZ files|*.elu"),
			EFileDialogFlags::None,
			SelectedFiles
		);
	}

	if (!bSuccess || SelectedFiles.Num() == 0)
	{
		PrintWarning(TEXT("User failed to select any files!"));
	}

	const FString& EluFilePath = SelectedFiles[0];
	bool bImportSuccessful = ImportEluFile_Internal(EluFilePath);
}

bool UEluImporter::ImportEluFile_Internal(const FString& EluFilePath)
{
	TArray<uint8> BinaryData;
	bool bSuccess = FFileHelper::LoadFileToArray(BinaryData, *EluFilePath);
	if (!bSuccess)
	{
		return false;
	}

	UINT Offset = 0;
	FEluHeader EluHeader;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&EluHeader, sizeof(EluHeader), BinaryData, Offset);

	/*
	if (EluHeader.Signature != EXPORTER_SIG)
	{



		FString Message = FPaths::GetCleanFilename(FilePath) + FString(" - failed to verify elu signature. ");
		UE_LOG(LogTemp, Error, TEXT("%s"), *Message);

		Message = FString("Signature: ") + FString::FromInt(EluHeader.Signature) + FString(", Expected: ") + FString::FromInt(EXPORTER_SIG);
		UE_LOG(LogTemp, Error, TEXT("%s"), *Message);

		return false;
	}
	*/



	return true;
}
