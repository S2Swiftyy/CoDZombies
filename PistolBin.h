// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolBin.generated.h"

UCLASS()
class TESTPROJECT_API APistolBin : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APistolBin();

	void buy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
