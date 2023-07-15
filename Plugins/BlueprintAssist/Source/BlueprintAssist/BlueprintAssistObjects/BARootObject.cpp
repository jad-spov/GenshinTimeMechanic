// Fill out your copyright notice in the Description page of Project Settings.

#include "BARootObject.h"

#include "BAAssetEditorHandlerObject.h"
#include "BlueprintAssistEditorFeatures.h"

void UBARootObject::Init()
{
	AssetHandler = NewObject<UBAAssetEditorHandlerObject>();
	AssetHandler->Init();

	EditorFeatures = NewObject<UBAEditorFeatures>();
	EditorFeatures->Init();
}

void UBARootObject::Tick()
{
	AssetHandler->Tick();
}

void UBARootObject::Cleanup()
{
	AssetHandler->Cleanup();
}
