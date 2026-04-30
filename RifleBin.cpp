// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleBin.h"
#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARifleBin::ARifleBin()
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
void ARifleBin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifleBin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifleBin::buy()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to buy a rifle / rifle ammo"));

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		if (GM->getGunType() == 3)
		{
			//you have a pistol
			//you are buying ammo
			if (GM->getMoney() >= 200)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOUGHT AMMO"));

				GM->setAmmoRes((GM->getAmmoRes()) + 240);
				GM->setMoney(GM->getMoney() - 200);
			}
		}
		else
		{
			//buying a pistol
			if (GM->getMoney() >= 1000)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOUGHT RIFLE"));

				GM->setGunType(3);
				GM->setAmmoCur(30);
				GM->setAmmoRes(240);
				GM->setMoney(GM->getMoney() - 1000);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is fucked"));
	}
}