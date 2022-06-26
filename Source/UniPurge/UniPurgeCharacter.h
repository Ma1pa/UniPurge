// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UniPurge/Trap.h"

#include "UniPurgeCharacter.generated.h"

UCLASS(config=Game)
class AUniPurgeCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	/* Camera boom positioning the camera behind the character */
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	/* Follow camera */
	class UCameraComponent* FollowCamera;

	UFUNCTION(BlueprintCallable)
	void InShift(float deltaTime);

	

public:
	AUniPurgeCharacter();

	UPROPERTY(EditAnywhere, Category = Trap)
	TSubclassOf<AActor> SelectedTrap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	/* Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	/* Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	/* Modifier applied whenever the character is running */
	float SprintModifier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	/* Modifier applied whenever the character is climbing */
	float ClimbModifier;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	/* Boolean indicating if the character is currently hiding*/
	bool IsHiding;
	UPROPERTY(VisibleAnywhere, Category=Movement)
	/* Boolean indicating if the character is running */
	bool IsRunning;

	UFUNCTION(BlueprintCallable)
	void EnterShift();

	UFUNCTION(BlueprintCallable)
	void ExitShift();

	UFUNCTION(BlueprintCallable)
	void PlaceTrap();

	UFUNCTION(BlueprintCallable, Category = Trap)
		float CostOfTrap();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Energy)
		float MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Energy)
		float CurrentEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Energy)
		float EnergyRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Energy)
		float ShiftEnterCost;

	UPROPERTY(BlueprintReadOnly)
	bool IsClimbing;

	UPROPERTY(BlueprintReadOnly)
	FVector CollisionNormal;

	UFUNCTION(BlueprintImplementableEvent)
		void EndGame(AActor* actorThatHit);


private:
	
	bool isShifting;
	
	
	FVector HeadArea;
	FVector FeetArea;
	ATrap* selectedTrap;
	void TrapToFloor();

protected:

	/* Called for forwards/backward input */
	void MoveForward(float Value);

	/* Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/* Called via input to indicate that the sprint action has started	*/
	void StartSprint();

	/* Called via input to indicate that the sprint action has ended */
	void StopSprint();

	/* Called via input to indicate that the hiding action has started */
	void StartHide();

	/* Called via input to indicate that the hiding action has ended */
	void StopHide();

	/**
	 * Called when moving while climbing. Returns direction of movement
	 * @param DirectionOfMovement : Indicates the desired movement result
	 * @param Right : Indicates if the movement is done to the right or not (Using float of movement), currently only used when movement is verified as horizontal
	 */
	FVector WallMovement(FVector DirectionOfMovement, bool Right);

	/**
	 * Called to check if there is a colliding wall at a certain position
	 * @param StartingPoint : Indicates the starting point of the wall collision check
	*/
	bool CheckWallAtPos(FVector StartingPoint);

	/**
	 * Called when reaching exterior border of a wall
	 * @param StartingPoint : Indicates the starting point of the wall collision check
	 * @param Right : Indicates if the movement is done to the right
	*/

	bool CheckWallAtAngles(FVector StartingPoint, bool Right);

	/* Called to start vaulting. Returns the desired position / movement */
	FVector VaultOverWall();

	/* Called to stop climbing */
	void StopClimbing();


	UFUNCTION(BlueprintCallable)
		void OnCollisionEnter(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ATrap* GetCurrentTrap() { return selectedTrap; }
};

