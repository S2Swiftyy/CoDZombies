// Fill out your copyright notice in the Description page of Project Settings.


#include "DogAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ADogAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorldTimerManager().SetTimer(ChaseTimer, this, &ADogAIController::ChasePlayer, 0.25f, true);
}

void ADogAIController::ChasePlayer()
{
	APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);

	if (!player)
	{
		//something wrong
		return;
	}

	MoveToActor(player, 120.0f);
}
