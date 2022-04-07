// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AutomatedTestsHUD.generated.h"

UCLASS()
class AAutomatedTestsHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAutomatedTestsHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

