// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "TestProjectCharacter.h"
#include "ZombieSpawnPoint.h"
#include "TestHUD.h"
#include "AZombieEnemy.h"

ATestGameMode::ATestGameMode()
{
	DefaultPawnClass = ATestProjectCharacter::StaticClass();
	HUDClass = ATestHUD::StaticClass();
}

void ATestGameMode::StartPlay()
{
	Super::StartPlay();

	CacheSpawnPoints();

	if (!TargetClass)
	{
		TargetClass = AAZombieEnemy::StaticClass();
	}

	if (!DogClass)
	{
		DogClass = AADogEnemy::StaticClass();
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("ROUND %d"), round));
	GetWorldTimerManager().SetTimer(spawnTimer, this, &ATestGameMode::Spawn, SpawnInterval, true);
}

void ATestGameMode::RegisterInteraction()
{
	Interactions++; //will later be used for counting if every zombie is dead per round
	money += 10;

	if (Interactions >= (maxAlive + dogsSpawned) && doneSpawning)
	{
		round++;
		//set new maxAlive for next round
		//maybe add 25 enemies every round?
		maxAlive += 25;
		Interactions = 0; //essentialy a round kill count
		doneSpawning = false;
		dogsSpawned = 0;
		spawned = 0;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("ROUND %d"), round));
	}
}

void ATestGameMode::buyDoor()
{
	money = money - (1000 + (1000 * doorsBought)); //each door cost 1000 more than the last, door 1 cost $1000
	doorsBought++;
}

void ATestGameMode::EndGame()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("GAME OVER!"));
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
		}, 360.0f, false);
}

int32 ATestGameMode::getMoney()
{
	return money;
}

void ATestGameMode::setMoney(int32 x)
{
	money = x;
}

int32 ATestGameMode::getDoorsBought()
{
	return doorsBought;
}

void ATestGameMode::CacheSpawnPoints()
{
	spawnPoints.Reset();

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(this, AZombieSpawnPoint::StaticClass(), Found);

	for (AActor* A : Found)
	{
		if (AZombieSpawnPoint* SP = Cast<AZombieSpawnPoint>(A))
		{
			spawnPoints.Add(SP);
		}
	}

	//go ahead and cache the zone 0 spawn points as valid
	ValidSpawnPoints();
}

void ATestGameMode::ValidSpawnPoints()
{
	validSpawns.Reset();

	int32 zone = doorsBought; //zones are 0-index so they align with doorsBought

	for (AZombieSpawnPoint* A : spawnPoints)
	{
		if (!A)
		{
			//make sure it doesn't add null input to the valid spawns
			continue;
		}
		
		if (A->getZoneID() <= zone)
		{
			validSpawns.Add(A);
		}
	}
}

void ATestGameMode::Spawn()
{
	//UE_LOG(LogTemp, Warning, TEXT("TRYING TO SPAWN"));
	
	//peace of mind checking
	if (spawnPoints.Num() == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Returned for spawnPoints"));
		return;
	}

	if (spawned >= maxAlive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Returned for spawned >= maxAlive"));
		return;
	}

	if (!TargetClass)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Returned for !TargetClass"));
		return;
	}

	if (doneSpawning)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Returned for doneSpawning"));
		return;
	}

	//chooses which spawnpoint to spawn at
	int32 index = FMath::RandRange(0, validSpawns.Num() - 1);
	FTransform SpawnXform = validSpawns[index]->GetActorTransform();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAZombieEnemy* Spawned = GetWorld()->SpawnActor<AAZombieEnemy>(TargetClass, SpawnXform, Params);

	if (Spawned)
	{
		spawned++;
		UE_LOG(LogTemp, Warning, TEXT("Spawned #%d"), spawned);
		if (round % 10 == 0)
		{
			//spawn some dogs too but not as many as normal zombies
			//1 dog every 10 zombies
			if (spawned % 10 == 0)
			{
				if (dogsSpawned < round) //dogs spawn every 10 rounds and only spawn dogs equal to round num
				{
					index = FMath::RandRange(0, validSpawns.Num() - 1);
					SpawnXform = validSpawns[index]->GetActorTransform();
					
					AADogEnemy* SpawnedDog = GetWorld()->SpawnActor<AADogEnemy>(DogClass, SpawnXform, Params);
					if (SpawnedDog)
					{
						UE_LOG(LogTemp, Warning, TEXT("DOG SPAWNED"));
						dogsSpawned++;
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DIDN'T SPAWN"));
	}

	if (spawned >= maxAlive)
	{
		//done spawning enemies for this round
		//this should spawn all enemies then stop spawning until they are all dead
		doneSpawning = true;
	}
}

int32 ATestGameMode::getAmmoCur()
{
	return ammoCur;
}

int32 ATestGameMode::getAmmoRes()
{
	return ammoRes;
}

void ATestGameMode::setAmmoCur(int32 x)
{
	ammoCur = x;
}

void ATestGameMode::setAmmoRes(int32 x)
{
	ammoRes = x;
}

void ATestGameMode::setGunType(int32 x)
{
	gunType = x;
}

int32 ATestGameMode::getGunType()
{
	return gunType;
}

int32 ATestGameMode::getHealth()
{
	return health;
}

void ATestGameMode::setHealth(int32 x)
{
	health = x;

	if (health <= 0)
	{
		EndGame();
		gameOver = true;
	}
}

bool ATestGameMode::getGameOver()
{
	return gameOver;
}