// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	FTimerHandle ChaseTimer;
	void ChasePlayer();
};
