// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TestGameMode.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    ATestHUD();
	virtual void DrawHUD() override;

protected:
    //size
    UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
    float CrosshairHalfSize = 6.0f;

    //center gap
    UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
    float CrosshairGap = 2.0f;

    //thickness
    UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
    float LineThickness = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UTexture2D* pistol = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UTexture2D* shotty = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UTexture2D* rifle = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FVector2D weaponIconSize = FVector2D(96.0f,96.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FVector2D weaponIconPadding = FVector2D(20.0f,20.0f);
};
