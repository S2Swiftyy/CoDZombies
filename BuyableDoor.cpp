// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyableDoor.h"
#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuyableDoor::ABuyableDoor()
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
void ABuyableDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuyableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuyableDoor::buy()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to buy a door"));
	
	if (bBought)
	{
		return;
	}

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		if (GM->getMoney() >= (1000 + (1000 * GM->getDoorsBought())))
		{
			GM->buyDoor();
			GM->ValidSpawnPoints();
			UE_LOG(LogTemp, Warning, TEXT("BOUGHT DOOR"));
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Mesh->SetCanEverAffectNavigation(false);
			Destroy();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is fucked"));
	}
}

