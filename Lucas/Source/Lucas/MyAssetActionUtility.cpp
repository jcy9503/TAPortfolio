// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "LandscapeRender.h"


#pragma region RenameSelectedAssets

void UMyAssetActionUtility::RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase) const
{

	// Check if something needs to be done
	if (SearchPattern.IsEmpty() || SearchPattern.Equals(ReplacePattern, SearchCase)) 
	{
		return;
	}

	// Get the selected objects
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	// Check each Asset if it needs to be renames
	for (UObject* SelectedObject : SelectedObjects) 
	{
		if (ensure(SelectedObject)) 
		{
			FString AssetName = SelectedObject->GetName();
			if (AssetName.Contains(SearchPattern, SearchCase)) 
			{
				FString NewName = AssetName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				++Counter;
			}
		}
	}

	GiveFeedback(TEXT("Renamed"), Counter);
}

#pragma endregion

#pragma region CheckPowerOfTwo

void UMyAssetActionUtility::CheckPowerOfTwo()
{
	// Get all selected assets
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			UTexture* Texture = dynamic_cast<UTexture*>(SelectedObject);
			if (ensure(Texture))
			{
				int32 Width = static_cast<int32>(Texture->GetSurfaceWidth());
				int32 Height = static_cast<int32>(Texture->GetSurfaceHeight());
				if (!IsPowerOfTwo(Width) || !IsPowerOfTwo(Height))
				{
					PrintToScreen(SelectedObject->GetPathName() + " is not a power of 2 texture", FColor::Red);
				}
				else
				{
					++Counter;
				}
			}
			else
			{
				PrintToScreen(SelectedObject->GetPathName() + " is not a texture", FColor::Red);
			}
		}
	}

	GiveFeedback("Power of two", Counter);
}



bool UMyAssetActionUtility::IsPowerOfTwo(int32 NumberToCheck)
{
	if (NumberToCheck == 0)
	{
		return false;
	}
	return (NumberToCheck & (NumberToCheck - 1)) == 0;
}

#pragma endregion

#pragma region AddPrefixes

void UMyAssetActionUtility::AddPrefixes() const
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if(ensure(SelectedObject))
		{
			const FString* Prefix = PrefixMap.Find(SelectedObject->GetClass());
			if(ensure(Prefix) && !Prefix->Equals(""))
			{
				FString OldName = SelectedObject->GetName();
				if(!OldName.StartsWith(*Prefix))
				{
					FString NewName = *Prefix + OldName;
					UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
					++Counter;
				}
			}
			else
			{
				PrintToScreen("Couldn't find prefix for class " + SelectedObject->GetClass()->GetName(), FColor::Red);
			}
		}
	}

	GiveFeedback("Added Prefix to", Counter);
}

#pragma endregion

#pragma region ProjectOrganizer

void UMyAssetActionUtility::CleanupFolder(FString ParentFolder) const
{
	// Check if parent folder is actually in the Content folder
	if(!ParentFolder.StartsWith(TEXT("/Game")))
	{
		ParentFolder = FPaths::Combine(TEXT("/Game"), ParentFolder);
	}

	TArray<UObject*> SelectedObjectS = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for(UObject* SelectedObject : SelectedObjectS)
	{
		if(ensure(SelectedObject))
		{
			if (FString NewPath = FPaths::Combine(ParentFolder, SelectedObject->GetClass()->GetName(), SelectedObject->GetName()); UEditorAssetLibrary::RenameLoadedAsset(SelectedObject, NewPath))
			{
				++Counter;
			}
			else
			{
				PrintToScreen("Couldn't move " + SelectedObject->GetName(), FColor::Red);
			}
		}
	}

	GiveFeedback("Moved ", Counter);
}

#pragma endregion

#pragma region Duplicator

void UMyAssetActionUtility::DuplicateAsset(const uint32 NumberOfDuplicates, const bool bSave) const
{
	TArray<FAssetData> AssetDataArray = UEditorUtilityLibrary::GetSelectedAssetData();

	uint32 Counter = 0;

	for(FAssetData AssetData : AssetDataArray)
	{
		for (uint32 i = 0; i < NumberOfDuplicates; ++i)
		{
			FString NewFileName = AssetData.AssetName.ToString().AppendChar('_').Append(FString::FromInt(i));
			if(FString NewPath = FPaths::Combine(AssetData.PackagePath.ToString(), NewFileName); ensure(UEditorAssetLibrary::DuplicateAsset(AssetData.ObjectPath.ToString(), NewPath)))
			{
				++Counter;
				if(bSave)
				{
					UEditorAssetLibrary::SaveAsset(NewPath, false);
				}
			}
		}
	}
}

#pragma endregion 

#pragma region RemoveUnusedAssets

void UMyAssetActionUtility::RemoveUnusedAssets(const bool bDeleteImmediately) const
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<UObject*> UnusedObjects = TArray<UObject*>();

	for(UObject* SelectedObject : SelectedObjects)
	{
		if(ensure(SelectedObject))
		{
			// FindPackageReferencersForAsset returns an array --> check size
			if(UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedObject->GetPathName(), true).Num() <= 0)
			{
				UnusedObjects.Add(SelectedObject);
			}
		}
	}

	uint32 Counter = 0;

	for(UObject* SelectedObject : UnusedObjects)
	{
		if(bDeleteImmediately)
		{
			if(UEditorAssetLibrary::DeleteLoadedAsset(SelectedObject))
			{
				++Counter;
			}
			else
			{
				PrintToScreen("Error deleting " + SelectedObject->GetPathName(), FColor::Red);
			}
		}
		else
		{
			FString NewPath = FPaths::Combine(TEXT("/Game"), TEXT("Trash"), SelectedObject->GetName());
			if(UEditorAssetLibrary::RenameLoadedAsset(SelectedObject, NewPath))
			{
				++Counter;
			}
			else
			{
				PrintToScreen("Error moving " + SelectedObject->GetPathName(), FColor::Red);
			}
		}
	}

	GiveFeedback(bDeleteImmediately ? "Deleted:" : "Moved to Trash Folder:", Counter);
}

#pragma endregion 

#pragma region Helper

void UMyAssetActionUtility::PrintToScreen(const FString Message, const FColor Color) const
{
	if (ensure(GEngine)) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), &Message);
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, Color, Message);
	}
}

void UMyAssetActionUtility::GiveFeedback(FString Method, const uint32 Counter) const
{
	FString Message = FString("No matching files found");
	const FColor Color = Counter > 0 ? FColor::Green : FColor::Red;

	if (Counter > 0)
	{
		Message = Method.AppendChar(' ');
		Message.AppendInt(Counter);
		Message.Append(Counter == 1 ? TEXT(" file") : TEXT(" files"));
	}

	PrintToScreen(Message, Color);
}

#pragma endregion