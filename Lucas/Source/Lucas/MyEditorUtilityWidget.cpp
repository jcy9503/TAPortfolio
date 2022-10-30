// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEditorUtilityWidget.h"
#include "EditorLevelLibrary.h"

#pragma region OrganizeWorldOutliner

void UMyEditorUtilityWidget::OrganizeWorldOutliner()
{
	TArray<AActor*> AllActors = UEditorLevelLibrary::GetAllLevelActors();
	for(AActor* Actor : AllActors)
	{
		if(ensure(Actor))
		{
			const FName* FolderName = FolderMap.Find(Actor->GetClass());
			if(ensure(FolderName))
			{
				Actor->SetFolderPath(*FolderName);
			}
		}
	}
}

#pragma endregion

#pragma region DeleteNullSMActors

void UMyEditorUtilityWidget::DeleteNullSMActors()
{
	TArray<AActor*> AllActors = UEditorLevelLibrary::GetAllLevelActors();
	for(AActor* Actor : AllActors)
	{
		if(ensure(Actor))
		{
			AStaticMeshActor* StaticMeshActor = dynamic_cast<AStaticMeshActor*>(Actor);
			if(ensure(StaticMeshActor))
			{
				UStaticMeshComponent* StaticMeshComponent = StaticMeshActor->GetStaticMeshComponent();
				if(ensure(StaticMeshComponent) && nullptr == StaticMeshComponent->GetStaticMesh())
				{
					Actor->Destroy();
				}
			}
		}
	}
}

#pragma endregion 