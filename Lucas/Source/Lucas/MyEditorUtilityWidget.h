// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "HoudiniAssetActor.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/Light.h"
#include "Engine/ReflectionCapture.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMeshActor.h"
#include "MyEditorUtilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class LUCAS_API UMyEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrganizeWorldOutliner")
	TMap<UClass*, FName> FolderMap = {
		{AStaticMeshActor::StaticClass(), "Static Mesh"},
		{ALight::StaticClass(), "Lighting"},
		{ASkyLight::StaticClass(), "Lighting"},
		{AReflectionCapture::StaticClass(), "Lighting"},
		{AHoudiniAssetActor::StaticClass(), "HDA"},
		{ASkeletalMeshActor::StaticClass(), "Skeletal Mesh"}
	};

	UFUNCTION(CallInEditor, BlueprintCallable)
	void OrganizeWorldOutliner();

	UFUNCTION(CallInEditor, BlueprintCallable)
	void DeleteNullSMActors();
};
