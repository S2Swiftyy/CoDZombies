// Fill out your copyright notice in the Description page of Project Settings.

#include "ADogEnemy.h"
#include "ZombieAIController.h"
#include "TestGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

// Sets default values
AADogEnemy::AADogEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	// Optional: rotate mesh to face forward (many Character meshes need this)
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));

	if (MannyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannyMesh.Object);
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->bReturnMaterialOnMove = true;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetCapsuleComponent());
	AttackSphere->SetRelativeLocation(FVector::ZeroVector);
	AttackSphere->SetSphereRadius(AttackRange);

	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AADogEnemy::OnAttackOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AADogEnemy::OnAttackOverlapEnd);

	health = 50;
}

// Called when the game starts or when spawned
void AADogEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AADogEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AADogEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AADogEnemy::hit(int32 dmg)
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction called on: %s"), *GetName());

	health = health - dmg;
	UE_LOG(LogTemp, Warning, TEXT("Health =  %d"), health);

	//After you interact with the current object, then it needs to disapear
	// these work but destroy() is better for targets that aren'y going to come back
	//SetActorHiddenInGame(true);
	//SetActorEnableCollision(false);
	if (health <= 0)
	{
		ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->RegisterInteraction();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is fucked"));
		}

		Destroy();
	}
}

void AADogEnemy::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerChar || OtherActor != PlayerChar)
	{
		return; // ignore zombies
	}

	TargetPlayer = PlayerChar;

	DealDamage();
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AADogEnemy::DealDamage, AttackInterval, true);
}

void AADogEnemy::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerChar || OtherActor != PlayerChar)
	{
		return;
	}

	TargetPlayer = nullptr;
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

void AADogEnemy::DealDamage()
{
	ACharacter* PlayerChar = TargetPlayer.Get();
	if (!PlayerChar)
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		return;
	}

	// Only damage if still in range
	if (!AttackSphere->IsOverlappingActor(PlayerChar))
	{
		TargetPlayer = nullptr;
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		return;
	}

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM) return;

	GM->setHealth(GM->getHealth() - 20);
}