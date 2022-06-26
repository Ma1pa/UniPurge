// Copyright Epic Games, Inc. All Rights Reserved.

#include "UniPurgeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "DrawDebugHelpers.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, String)

//////////////////////////////////////////////////////////////////////////
// AUniPurgeCharacter

AUniPurgeCharacter::AUniPurgeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// set our defauls modifier values
	SprintModifier = 2.0f;
	ClimbModifier = 0.3f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	

	//Other Variables
	IsRunning = false;
	IsClimbing = false;
	CollisionNormal = FVector::ZeroVector;
	HeadArea = FVector(0, 0, 70);
	FeetArea = FVector(0, 0, -100);
	selectedTrap = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUniPurgeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//TODO Remap Jump function to work while in a wall
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AUniPurgeCharacter::StartSprint);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AUniPurgeCharacter::StopSprint);
	PlayerInputComponent->BindAction("Hide", IE_Pressed, this, &AUniPurgeCharacter::StartHide);
	PlayerInputComponent->BindAction("Hide", IE_Released, this, &AUniPurgeCharacter::StopHide);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUniPurgeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUniPurgeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUniPurgeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUniPurgeCharacter::LookUpAtRate);

}

void AUniPurgeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUniPurgeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUniPurgeCharacter::MoveForward(float Value)
{
	//We reduce the Value according to the sprintModifier to limit the walking speed to the default one
	float ModifiedValue = Value / SprintModifier;
	//TODO: If IsHiding -> Multiply Value by some amount (0.x)
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (IsClimbing)
			Direction = WallMovement(FVector::UpVector, false);

		//Calculate modifications to speed
		if (IsRunning)	ModifiedValue *= SprintModifier;
		//if (IsHiding)	ModifiedValue *= HidingModifier;

		AddMovementInput(Direction, ModifiedValue);

	}
}

void AUniPurgeCharacter::MoveRight(float Value)
{
	//We reduce the Value according to the sprintModifier to limit the walking speed to the default one
	float ModifiedValue = Value / SprintModifier;
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (IsClimbing)
		{
			Direction = WallMovement(-FVector::RightVector, ModifiedValue >= 0);
			if(!IsRunning)
				ModifiedValue *= ClimbModifier;
		}

		//Calculate modifications to speed
		if (IsRunning)	ModifiedValue *= SprintModifier;
		//if (IsHiding)	ModifiedValue *= HidingModifier;

		// add movement in that direction
		AddMovementInput(Direction, ModifiedValue);
	}
}

void AUniPurgeCharacter::StartSprint()
{
	IsRunning = true;
	// TODO: Change animation to sprinting animations
	// TODO: Check for water movement
}

void AUniPurgeCharacter::StopSprint()
{
	IsRunning = false;
	// TODO: Change animation to walking animations
	// TODO: Check for water movement
}

void AUniPurgeCharacter::StartHide()
{
	Crouch();
	IsHiding = true;
	// TODO: Check for "Hiding Spots"
	// TODO: If no "Hiding Spots"; change animation to crouching animations
}

void AUniPurgeCharacter::StopHide()
{
	UnCrouch();
	IsHiding = false;
	// TODO: Change animation to walking animations
}


FVector AUniPurgeCharacter::WallMovement(FVector DirectionOfMovement, bool Right)
{
	//Check if theres a wall at head height
	if (!CheckWallAtPos(HeadArea))
	{
		PrintString("Parte de arriba descolgada.");
		//Check if there is a wall at feet height
		if (!CheckWallAtPos(FeetArea))
		{
			PrintString("Cuerpo flotando");
			//TODO Turn on the outside of the wall
			//We check all degrees until something is found 

			if (!CheckWallAtAngles(FeetArea, Right))
				StopClimbing();
			
		}
		VaultOverWall();
	}
	return FRotationMatrix(CollisionNormal.Rotation()).TransformVector(DirectionOfMovement);
}

bool AUniPurgeCharacter::CheckWallAtPos(FVector StartingPoint)
{
	FHitResult Hit;
	FRotator Rot = CollisionNormal.Rotation();

	FVector Start = GetActorLocation() + StartingPoint;
	FVector End = Start + Rot.Vector() * -150;

	FCollisionQueryParams TraceParams;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	return false;
}

bool AUniPurgeCharacter::CheckWallAtAngles(FVector StartingPoint, bool Right)
{
	if (IsRunning)
		return false;
	FHitResult Hit;
	FRotator Rot = CollisionNormal.Rotation();
	float Yaw = 90;
	float InitialYaw = Rot.Yaw;

	FVector Start = GetActorLocation() + StartingPoint + Rot.Vector() * -10;
	FVector End = Start + Rot.Vector() * -60;
	FCollisionQueryParams TraceParams;
	

	while (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams) && Yaw > -90)
	{
		Yaw--;
		if (Right)
			Rot.Yaw = InitialYaw + Yaw;
		else
			Rot.Yaw = InitialYaw - Yaw;
		End = Start + Rot.Vector() * -100;
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f);
		
	}
	if (Yaw > -90)
	{ 
		CollisionNormal = Hit.Normal;
		return true;
	}
		
	else
		return false;
}

FVector AUniPurgeCharacter::VaultOverWall()
{
	return FVector::ZeroVector;
}

void AUniPurgeCharacter::OnCollisionEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector ObjectNormal, const FHitResult& Hit)
{
	EndGame(OtherActor);

	if( GetCharacterMovement()->MovementMode != MOVE_Walking && ( abs(ObjectNormal.X) > abs(ObjectNormal.Z) || abs(ObjectNormal.Y) > abs(ObjectNormal.Z)) &&  !strstr(TCHAR_TO_ANSI(*OtherActor->GetName()), "WorldWall"))
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		CollisionNormal = ObjectNormal;
		IsClimbing = true;
		PrintString(FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));
		SetActorRotation(FQuat::MakeFromEuler(FVector(-CollisionNormal.X, -CollisionNormal.Y, -CollisionNormal.Z)));

	}
	else if(IsClimbing)
	{
		StopClimbing();
	}
	else if (strstr(TCHAR_TO_ANSI(*OtherActor->GetName()), "WaterBorder"))
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Swimming);
	}
	
}

void AUniPurgeCharacter::StopClimbing()
{ 
	IsClimbing = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorRotation(FQuat::Identity);
}

void AUniPurgeCharacter::EnterShift()
{
		isShifting = true;
}

void AUniPurgeCharacter::ExitShift()
{
	isShifting = false;
	if (selectedTrap != nullptr)
	{
		selectedTrap->Destroy();
		selectedTrap = nullptr;
	}
}

void AUniPurgeCharacter::InShift(float deltaTime)
{
	if (isShifting)
	{
		FHitResult Hit;
		FRotator Rot = GetViewRotation();

		FVector Start = GetActorLocation() + HeadArea;
		FVector End = Start + Rot.Vector() * 10000;

		FCollisionQueryParams TraceParams;
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams))
		{
			UE_LOG(LogTemp, Warning, TEXT("Choco"));
			if (selectedTrap == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("ColocoTrampa"));
				FVector posicion = { Hit.Location.X, Hit.Location.Y, Hit.Location.Z };
				selectedTrap = GetWorld()->SpawnActor<ATrap>(SelectedTrap, posicion, GetActorRotation());
			}
			else if(Hit.Actor != selectedTrap)
			{
				selectedTrap->SetActorLocation(GetActorLocation());
				FRotator rotator = GetViewRotation();
				rotator.Pitch = 0;
				rotator.Roll = 0;
				selectedTrap->SetActorRelativeRotation(rotator);
				selectedTrap->SetActorLocation(Hit.Location, true);
				TrapToFloor();
				
			}
		}
	}
	else
	{
		if (CurrentEnergy < MaxEnergy)
			CurrentEnergy += EnergyRecovery * deltaTime;
		else
			CurrentEnergy = MaxEnergy;
	}
}

void AUniPurgeCharacter::TrapToFloor()
{
	FHitResult Hit;
	FRotator Rot = {-90,0,0};

	FVector Start = selectedTrap->GetActorLocation();
	FVector End = Start + Rot.Vector() * 10000;

	FCollisionQueryParams TraceParams;
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams))
	{
		selectedTrap->SetActorLocation(Hit.Location, true);
	}
}

void AUniPurgeCharacter::PlaceTrap()
{
	selectedTrap->PlaceTrap();
	selectedTrap = nullptr;
}

float AUniPurgeCharacter::CostOfTrap()
{
	return selectedTrap->GetCurrentCost();
}