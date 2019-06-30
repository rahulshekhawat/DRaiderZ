// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "EditorFunctionLibrary.h"
#include "EOD.h"

#include "Misc/Paths.h"
#include "AssetRegistryModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"
#include "Misc/ScopedSlowTask.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompilerModule.h"
#include "PackageTools.h"

UEditorFunctionLibrary::UEditorFunctionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEditorFunctionLibrary::CreateBlueprint(UClass* ParentClass)
{
	bool bResult = FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass);
	if (bResult)
	{
		IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");

		UClass* BlueprintClass = nullptr;
		UClass* BlueprintGeneratedClass = nullptr;
		KismetCompilerModule.GetBlueprintTypesForClass(ParentClass, BlueprintClass, BlueprintGeneratedClass);

		//~ @note Current just test code. Need to replace it with proper package name/path.
		FString PackageName = FString("/Game/RaiderZ/Zunk/TestPackage");
		bool bPackageExists = FPackageName::DoesPackageExist(PackageName);

		if (!bPackageExists)
		{
			// If package doesn't exist, it's safe to create new package
			PackageName = PackageTools::SanitizePackageName(PackageName);
			UPackage* Package = CreatePackage(nullptr, *PackageName);
			Package->FullyLoad();


			UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(ParentClass, Package, FName("BP_Wow_ok"), EBlueprintType::BPTYPE_Normal, BlueprintClass, BlueprintGeneratedClass);
			if (Blueprint)
			{
				Blueprint->MarkPackageDirty();
			}
		}
	}
}

bool UEditorFunctionLibrary::IsHumanPlayerMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr)
	{
		return false;
	}

	FString FullMeshName = Mesh->GetFName().ToString();
	if (FullMeshName.StartsWith(TEXT("SK_hf")) || FullMeshName.StartsWith(TEXT("SK_hm")))
	{
		return true;
	}

	return false;
}

FString UEditorFunctionLibrary::GetRaiderZMeshName(USkeletalMesh* Mesh)
{
	if (Mesh)
	{
		FString FullMeshName = Mesh->GetFName().ToString();
		if (FullMeshName.StartsWith(TEXT("SK_hf")))
		{
			return TEXT("hf");
		}
		else if (FullMeshName.StartsWith(TEXT("SK_hm")))
		{
			return TEXT("hm");
		}
		else
		{
			return FullMeshName.RightChop(3);
		}
	}
	return TEXT("");
}

TArray<FAssetData> UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(USkeletalMesh* SkeletalMesh)
{
	if (SkeletalMesh == nullptr)
	{
		return TArray<FAssetData>();
	}

	FSoftObjectPath SkeletonSoftPath(SkeletalMesh->Skeleton);
	if (!SkeletonSoftPath.IsValid())
	{
		return TArray<FAssetData>();
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AllAnimations;
	AssetRegistryModule.Get().GetAssetsByClass(FName("AnimSequenceBase"), AllAnimations, true);

	int32 AssetNum = AllAnimations.Num();
	TArray<FAssetData> MeshAnimations;

	FString TaskMessage = TEXT("Finding animations for mesh '") + SkeletalMesh->GetFName().ToString() + TEXT("'");
	FScopedSlowTask FindAnimationTask(AssetNum, FText::FromString(TaskMessage));
	FindAnimationTask.MakeDialog();

	for (int i = 0; i < AssetNum; i++)
	{
		FAssetData AssetData = AllAnimations[i];
		FAssetDataTagMapSharedView::FFindTagResult TagResult = AssetData.TagsAndValues.FindTag(TEXT("Skeleton"));
		const FString& Result = TagResult.GetValue();
		FSoftObjectPath ResultPath(Result);

		if (ResultPath == SkeletonSoftPath)
		{
			MeshAnimations.Add(AssetData);
		}

		FindAnimationTask.EnterProgressFrame();
	}

	return MeshAnimations;
}

TArray<FAssetData> UEditorFunctionLibrary::GetAllSoundAssets()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> SoundAssets;
	AssetRegistryModule.Get().GetAssetsByClass(FName("SoundBase"), SoundAssets, true);
	return SoundAssets;
}
