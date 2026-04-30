// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnPoint.h"
#include "Components/BillboardComponent.h"

// Sets default values
AZombieSpawnPoint::AZombieSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(billboard);
}

// Called when the game starts or when spawned
void AZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 AZombieSpawnPoint::getZoneID()
{
	return zoneID;
}

// Called every frame
void AZombieSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}