// Fill out your copyright notice in the Description page of Project Settings.

#include "TestHUD.h"
#include "Kismet/GameplayStatics.h"
#include "TestGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

ATestHUD::ATestHUD()
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> pistolIcon(TEXT("/Script/Engine.Texture2D'/Game/Materials/pistol.pistol'"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> shottyIcon(TEXT("/Script/Engine.Texture2D'/Game/Materials/shotty.shotty'"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> rifleIcon(TEXT("/Script/Engine.Texture2D'/Game/Materials/rifle.rifle'"));

    if (pistolIcon.Succeeded())
    {
        pistol = pistolIcon.Object;
    }

    if (shottyIcon.Succeeded())
    {
        shotty = shottyIcon.Object;
    }

    if (rifleIcon.Succeeded())
    {
        rifle = rifleIcon.Object;
    }
}

void ATestHUD::DrawHUD()
{
	Super::DrawHUD();
    ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));

	if (!Canvas)
	{
		return;
		//Canvas didn't load properly so don't try to draw anything
	}

    if (GM->getGameOver())
    {
        DrawRect(FLinearColor::Black, 0.0f, 0.0f, Canvas->SizeX, Canvas->SizeY);

        UFont* font = GEngine->GetLargeFont();
        FString msg = TEXT("YOU DIED!");

        float textW = 0.0f;
        float textH = 0.0f;
        GetTextSize(msg, textW, textH, font, 2.0f);

        float x = (Canvas->SizeX - textW) * 0.5f;
        float y = (Canvas->SizeY - textH) * 0.5f;

        DrawText(msg, FLinearColor::Red, x, y, font, 2.0f, false);
    }
    else
    {
        const float CenterX = Canvas->SizeX * 0.5f;
        const float CenterY = Canvas->SizeY * 0.5f;

        const FLinearColor Color = FLinearColor::White;

        // left side
        DrawLine(CenterX - CrosshairGap - CrosshairHalfSize, CenterY,
            CenterX - CrosshairGap, CenterY,
            Color, LineThickness);

        // right side
        DrawLine(CenterX + CrosshairGap, CenterY,
            CenterX + CrosshairGap + CrosshairHalfSize, CenterY,
            Color, LineThickness);

        // up
        DrawLine(CenterX, CenterY - CrosshairGap - CrosshairHalfSize,
            CenterX, CenterY - CrosshairGap,
            Color, LineThickness);

        // down
        DrawLine(CenterX, CenterY + CrosshairGap,
            CenterX, CenterY + CrosshairGap + CrosshairHalfSize,
            Color, LineThickness);

        //money text
        int32 money = GM->getMoney();
        int32 health = GM->getHealth();

        FString moneyText = FString::Printf(TEXT("MONEY: %d"), money);
        FString healthText = FString::Printf(TEXT("HEALTH: %d"), health);

        UFont* font = GEngine ? GEngine->GetSmallFont() : nullptr;
        if (!font)
        {
            return;
            //font didn't load correct
        }

        float x = 20.0f;
        float y = Canvas->SizeY - 20.0f;
        float textW = 0.0f;
        float textH = 0.0f;
        GetTextSize(moneyText, textW, textH, font, 1.0f);

        DrawText(moneyText, FLinearColor::White, x, y - textH, font, 1.0f, false);

        GetTextSize(healthText, textW, textH, font, 1.0f);
        DrawText(healthText, FLinearColor::White, x, (y - 20.0f) - textH, font, 1.0f, false);

        //ammo text
        int32 currAmmo = GM->getAmmoCur();
        int32 resAmmo = GM->getAmmoRes();

        FString ammoText = FString::Printf(TEXT("%d / %d"), currAmmo, resAmmo);

        x = Canvas->SizeX - 100.0f;
        y = Canvas->SizeY - 20.0f;
        textW = 0.0f;
        textH = 0.0f;
        GetTextSize(ammoText, textW, textH, font, 1.0f);

        DrawText(ammoText, FLinearColor::White, x, y - textH, font, 1.0f, false);

        //gun picture
        int32 gun = GM->getGunType();

        const float X = Canvas->SizeX - weaponIconPadding.X - weaponIconSize.X;
        const float Y = Canvas->SizeY - weaponIconPadding.Y - weaponIconSize.Y;

        if (gun == 1)
        {
            DrawTexture(pistol, X, Y, weaponIconSize.X, weaponIconSize.Y, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White);
        }
        else if (gun == 2)
        {
            DrawTexture(shotty, X, Y, weaponIconSize.X, weaponIconSize.Y, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White);
        }
        else if (gun == 3)
        {
            DrawTexture(rifle, X, Y, weaponIconSize.X, weaponIconSize.Y, 0.0f, 0.0f, 1.0f, 1.0f, FLinearColor::White);
        }
    }
}