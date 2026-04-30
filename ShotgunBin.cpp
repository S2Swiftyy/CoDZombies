// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunBin.h"
#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShotgunBin::AShotgunBin()
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
void AShotgunBin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShotgunBin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShotgunBin::buy()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to buy a shotty / shotty ammo"));

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		if (GM->getGunType() == 2)
		{
			//you have a pistol
			//you are buying ammo
			if (GM->getMoney() >= 250)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOUGHT AMMO"));

				GM->setAmmoRes((GM->getAmmoRes()) + 48);
				GM->setMoney(GM->getMoney() - 250);
			}
		}
		else
		{
			//buying a pistol
			if (GM->getMoney() >= 750)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOUGHT SHOTTY"));

				GM->setGunType(2);
				GM->setAmmoCur(6);
				GM->setAmmoRes(48);
				GM->setMoney(GM->getMoney() - 750);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is fucked"));
	}
}