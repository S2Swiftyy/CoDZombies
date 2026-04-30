// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DogAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ADogAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	FTimerHandle ChaseTimer;
	void ChasePlayer();
};
