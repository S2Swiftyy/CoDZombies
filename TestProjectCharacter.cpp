// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestProject.h"
#include "InteractableActor.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine/World.h"
#include "BuyableDoor.h"
#include "TestGameMode.h"
#include "ADogEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RifleBin.h"
#include "ShotgunBin.h"
#include "PistolBin.h"

ATestProjectCharacter::ATestProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
	FirstPersonCameraComponent->SetRelativeRotation(FRotator::ZeroRotator);
	//FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	//hopefully this makes my character move arounds
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Default(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'")
	);

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'")
	);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'")
	);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Jump(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'")
	);

	if (IMC_Default.Succeeded())
	{
		DefaultMappingContext = IMC_Default.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load IMC_Default"));
	}

	if (IA_Move.Succeeded())
	{
		MoveAction = IA_Move.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load IA_Move"));
	}

	if (IA_Look.Succeeded())
	{
		LookAction = IA_Look.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load IA_Look"));
	}

	if (IA_Jump.Succeeded())
	{
		JumpAction = IA_Jump.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load IA_Jump"));
	}

	//and hopefully this one fixes being able to look arond
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MouseLook(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_MouseLook.IMC_MouseLook'")
	);

	if (IMC_MouseLook.Succeeded())
	{
		MouseLookMappingContext = IMC_MouseLook.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load MouseLookMappingContext"));
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MouseLook(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MouseLook.IA_MouseLook'")
	);

	if (IA_MouseLook.Succeeded())
	{
		MouseLookAction = IA_MouseLook.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load IA_MouseLook"));
	}
}

void ATestProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if(DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				UE_LOG(LogTemp, Warning, TEXT("Added IMC_Default mapping context"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is null"));
			}

			if (MouseLookMappingContext)
			{
				Subsystem->AddMappingContext(MouseLookMappingContext, 1);
				UE_LOG(LogTemp, Warning, TEXT("Added MouseLookMappingContext"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("MouseLookMappingContext is null"));
			}
		}
	}
}

void ATestProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATestProjectCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATestProjectCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestProjectCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestProjectCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATestProjectCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogTestProject, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//fire
	PlayerInputComponent->BindAction("fire", IE_Pressed, this, &ATestProjectCharacter::Shoot);
	PlayerInputComponent->BindAction("fire", IE_Released, this, &ATestProjectCharacter::StopShooting);
	//buy
	PlayerInputComponent->BindAction("buy", IE_Pressed, this, &ATestProjectCharacter::buy);
	//reload
	PlayerInputComponent->BindAction("reload", IE_Pressed, this, &ATestProjectCharacter::reload);
}

void ATestProjectCharacter::Shoot()
{
	Fire();

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	int32 gun = GM->ATestGameMode::getGunType();

	if (gun == 1 || gun == 2)
	{
		//pistol and shotgun only recoil once
		//recoil for AR is handled in fire() every shot
		ApplyRecoil();
	}

	if (gun == 3) // if you have an AR
	{
		//then continue to shoot
		GetWorldTimerManager().SetTimer(fireTimerHandle, this, &ATestProjectCharacter::Fire, fireRate, true);
	}
}

void ATestProjectCharacter::StopShooting()
{
	GetWorldTimerManager().ClearTimer(fireTimerHandle);
}


void ATestProjectCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ATestProjectCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ATestProjectCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATestProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ATestProjectCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ATestProjectCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ATestProjectCharacter::Fire()
{
	FVector Start;
	FRotator rot;

	//use ammo even if you miss an enemy
	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	int32 gun = GM->ATestGameMode::getGunType();

	int32 currAmmo = GM->getAmmoCur();
	//can't shoot with no ammo
	if (currAmmo <= 0)
	{
		return;
	}

	GM->ATestGameMode::setAmmoCur(currAmmo - 1);

	//if you run out of ammo then you need to reload
	if (GM->getAmmoCur() <= 0)
	{
		//reload
		reload();
	}

	GetController()->GetPlayerViewPoint(Start, rot);

	//RIFLE CAN USE THE SAME LOGIC AS PISTOL SINCE IT'LL CHANGE THE DMG ANYWAY
	//JUST MAKE A DIFF INPUT FOR WHEN IT IS PRESSED DOWN
	//maybe
	//make the single click go away entirely and make a pressed() function that
	//if gunType == 1 || gunType == 2 then it just calls this method once
	//else it calls it repeatedly?
	

	//shows where you shoot and displays green if you hit an object and red if you don't
	//DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.0f);

	if (gun == 3)
	{
		//recoil every shot for an AR
		ApplyRecoil();
	}

	if (gun == 2)
	{
		//do shotgun spray as well as pistol / AR bullet
		int32 pellets = 10;
		float spread = 6.0f;

		for (int32 i = 0; i < pellets; i++)
		{
			float spray = FMath::DegreesToRadians(spread);
			FVector dir = FMath::VRandCone(rot.Vector(), spray);

			//shoot it in that direction
			//move most of this method into firePellets()
			firePellets(Start, dir);
		}
	}
	else
	{
		firePellets(Start, rot.Vector());
	}

}

void ATestProjectCharacter::firePellets(const FVector& start, const FVector& direction)
{
	FVector Start;
	FRotator rot;
	
	GetController()->GetPlayerViewPoint(Start, rot);
	const FVector End = Start + (direction * 5000.0f);
	

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	UE_LOG(LogTemp, Warning, TEXT("Fire hit: %s"), *GetNameSafe(Hit.GetActor()));
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.05f, 0, 2.0f);

	//shooting effects would go here
	//recoil, sound, particles, tracer rounds

	if (!bHit)
	{
		return;
	}

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	int32 gun = GM->getGunType();
	int32 dmg = 0;
	switch (gun)
	{
	case 1: // pistol
		dmg = 25;
		break;
	case 2: //shotgun
		dmg = 12;
		break;
	case 3: // AR
		dmg = 15;
		break;
	}

	bool headshot = (Hit.BoneName == FName("head"));
	if (headshot)
	{
		dmg = 100;
	}

	if (AAZombieEnemy* Z = Cast<AAZombieEnemy>(Hit.GetActor()))
	{
		Z->hit(dmg);
		UE_LOG(LogTemp, Warning, TEXT("HitComp = %s Bone = %s"), *GetNameSafe(Hit.GetComponent()), *Hit.BoneName.ToString());
	}
	else if (AADogEnemy* D = Cast<AADogEnemy>(Hit.GetActor()))
	{
		D->hit(dmg);
	}
}

void ATestProjectCharacter::ApplyRecoil()
{
	float PitchKick = FMath::RandRange(RecoilPitchMin, RecoilPitchMax);
	float YawKick = FMath::RandRange(-RecoilYawMax, RecoilYawMax);

	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM->getGunType() == 2) //shotguns have more recoil
	{
		PitchKick = PitchKick * 4;
		YawKick = YawKick * 4;
	}

	AddControllerPitchInput(-PitchKick);
	AddControllerYawInput(YawKick);
}

void ATestProjectCharacter::reload()
{
	ATestGameMode* GM = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(this));

	int32 currAmmo = GM->getAmmoCur();
	int32 magSize;
	int32 gun = GM->getGunType();

	switch (gun)
	{
		case 1:
			magSize = 12;
			break;
		case 2:
			magSize = 6;
			break;
		case 3:
			magSize = 30;
			break;
	}

	int32 resAmmo = GM->getAmmoRes();
	int32 ammoDiff = magSize - currAmmo;

	if (resAmmo >= magSize)
	{
		GM->setAmmoCur(magSize);
		GM->setAmmoRes(resAmmo - ammoDiff);
	}
	else if (resAmmo == 0)
	{
		//reloading with zero ammo does nothing
		return;
	}
	else
	{
		if (currAmmo + resAmmo >= magSize)
		{
			GM->setAmmoCur(magSize);
			GM->setAmmoRes(resAmmo - ammoDiff);
		}
		else
		{
			GM->setAmmoCur(currAmmo + resAmmo);
			GM->setAmmoRes(0);
		}
	}
}

void ATestProjectCharacter::buy()
{
	float distance = 200.0f;

	FVector Start;
	FRotator rot;

	GetController()->GetPlayerViewPoint(Start, rot);

	const FVector End = Start + (rot.Vector() * (distance * 2));

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params); //checks if you are looking at the door
	
	ABuyableDoor* door = Cast<ABuyableDoor>(Hit.GetActor());
	APistolBin* pistol = Cast<APistolBin>(Hit.GetActor());
	AShotgunBin* shotty = Cast<AShotgunBin>(Hit.GetActor());
	ARifleBin* rifle = Cast<ARifleBin>(Hit.GetActor());

	if (!bHit)
	{
		return;
		//did not look at anything when trying to buy
	}

	if (door)
	{
		float distSq = FVector::DistSquared(GetActorLocation(), door->GetActorLocation());
		if (distSq > FMath::Square(distance))
		{
			//check if you are not in buying range
			return;
		}

		door->buy();
	}
	else if (pistol)
	{
		float distSq = FVector::DistSquared(GetActorLocation(), pistol->GetActorLocation());
		if (distSq > FMath::Square(distance))
		{
			//check if you are not in buying range
			return;
		}

		pistol->buy();
	}
	else if (shotty)
	{
		float distSq = FVector::DistSquared(GetActorLocation(), shotty->GetActorLocation());
		if (distSq > FMath::Square(distance))
		{
			//check if you are not in buying range
			return;
		}

		shotty->buy();
	}
	else if (rifle)
	{
		float distSq = FVector::DistSquared(GetActorLocation(), rifle->GetActorLocation());
		if (distSq > FMath::Square(distance))
		{
			//check if you are not in buying range
			return;
		}

		rifle->buy();
	}
}