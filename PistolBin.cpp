// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolBin.h"
#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APistolBin::APistolBin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void APistolBin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolBin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APistolBin::buy()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to buy a pistol / pistol ammo"));

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		if (GM->getGunType() == 1)
		{
			//you have a pistol
			//you are buying ammo
			if (GM->getMoney() >= 150)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOUGHT AMMO"));

				GM->setAmmoRes((GM->getAmmoRes()) + 96);
				GM->setMoney(GM->getMoney() - 150);
			}
		}
		else
		{
			//buying a pistol
			if (GM->getMoney() >= 500)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOUGHT PISTOL"));

				GM->setGunType(1);
				GM->setAmmoCur(12);
				GM->setAmmoRes(96);
				GM->setMoney(GM->getMoney() - 500);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is fucked"));
	}
}