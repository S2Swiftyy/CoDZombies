// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InteractableActor.h"
#include "AZombieEnemy.h"
#include "ADogEnemy.h"
#include "TestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RegisterInteraction();
	void buyDoor();
	ATestGameMode();
	int32 getMoney();
	int32 getDoorsBought();
	int32 getAmmoCur();
	int32 getAmmoRes();
	void setAmmoCur(int32);
	void setAmmoRes(int32);

private:
	int32 Interactions = 0;
	void EndGame();
	int32 money = 500;
	int32 doorsBought = 0;
	int32 ammoRes = 96;
	int32 ammoCur = 12;
	int32 health = 100;
	bool gameOver = false;
	
protected:
	virtual void StartPlay() override;

private:
	//the class that I want to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AAZombieEnemy> TargetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AADogEnemy> DogClass;

	//how often to spawn
	//for now I am just spawning on a timer and not setting up waves
	//but this can be used on how often a zombie will spawn
	//untill the correct number of zombies is reached for each wave
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float SpawnInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 maxAlive = 10;

	int32 spawned = 0;
	int32 dogsSpawned = 0;

	FTimerHandle spawnTimer;

	void CacheSpawnPoints();
	void Spawn();

	TArray<class AZombieSpawnPoint*> spawnPoints;
	TArray<class AZombieSpawnPoint*> validSpawns;

	int32 round = 1;
	bool doneSpawning = false;
	int32 gunType = 1;

public:
	
	void ValidSpawnPoints(); //called when a new door is bought
	void setGunType(int32);
	int32 getGunType();
	void setMoney(int32);
	int32 getHealth();
	void setHealth(int32);
	bool getGameOver();
};