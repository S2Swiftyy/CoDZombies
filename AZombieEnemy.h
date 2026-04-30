// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "AZombieEnemy.generated.h"

UCLASS()
class TESTPROJECT_API AAZombieEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAZombieEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void hit(int32);

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USphereComponent* AttackSphere = nullptr;

	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRange = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackInterval = 1.0f; // damage once per second


	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void DealDamage();

private:
	int32 health;

	TWeakObjectPtr<ACharacter> TargetPlayer;
};
