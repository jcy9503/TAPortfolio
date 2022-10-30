// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "AssetActionUtility.h"
#include "../../Plugins/Runtime/HoudiniEngineForUnreal/Source/HoudiniEngineRuntime/Public/HoudiniAsset.h"
#include "Materials/MaterialInstance.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "Animation/MorphTarget.h"
#include "UMGEditor/Public/WidgetBlueprint.h"
#include "MyAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class LUCAS_API UMyAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	/**
	* Rename the selected assets
	* @param SearchPattern - the pattern to search for
	* @param ReplacePattern - the pattern to replace with
	* @param SearchCase - should the case be ignored?
	**/
	UFUNCTION(CallInEditor)
	void RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase) const;

	/**
	* Checks if textures are power of two size.
	**/
	UFUNCTION(CallInEditor)
	void CheckPowerOfTwo();

	/**
	 * Checks if assets names properly with class named prefixes and fix these.
	 **/
	UFUNCTION(CallInEditor)
	void AddPrefixes() const;

	/**
	 * Organize selected assets to class folders in parent location user typed in.
	 **/
	UFUNCTION(CallInEditor)
	void CleanupFolder(FString ParentFolder = FString("/Game")) const;

	/**
	 * Duplicate selected assets with number-tagged
	 **/
	UFUNCTION(CallInEditor)
	void DuplicateAsset(uint32 NumberOfDuplicates = 1, bool bSave = true) const;

	/**
	 * Deletes unused assets
	 * bDeleteImmediately - Delete them permanently?
	 **/
	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets(bool bDeleteImmediately) const;

private:

	const TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialFunction::StaticClass(), TEXT("MF_")},
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("S_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UWidgetBlueprint::StaticClass(), TEXT("WBP_")},
		{UMorphTarget::StaticClass(), TEXT("MP_")},
		{USkeletalMesh::StaticClass(), TEXT("SK_")},
		{UTextureRenderTarget::StaticClass(), TEXT("RT_")},
		{UHoudiniAsset::StaticClass(), TEXT("HDA_")},
		{UMediaPlayer::StaticClass(), TEXT("MP_")}
	};

	bool IsPowerOfTwo(int32 NumberToCheck);
	
	/**
	* Prints a message to the screen
	* @param Message - the message to print
	* @param Color - the color to use for the message
	**/
	void PrintToScreen(const FString Message, const FColor Color) const;

	/**
	* Give Feedback to the user
	* @param Method - the method that was used
	* @param Counter - how many objects were affected
	**/
	void GiveFeedback(FString Method, const uint32 Counter) const;
	
};
