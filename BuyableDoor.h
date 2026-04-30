// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuyableDoor.generated.h"

UCLASS()
class TESTPROJECT_API ABuyableDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuyableDoor();

	void buy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;

private:
	bool bBought = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
