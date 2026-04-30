// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawnPoint.generated.h"

UCLASS()
class TESTPROJECT_API AZombieSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawnPoint();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere)
	class UBillboardComponent* billboard = nullptr;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 zoneID = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int32 getZoneID();

};
