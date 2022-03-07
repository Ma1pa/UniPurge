// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

public:
	AUniPurgeCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	/* Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	/* Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	float BaseLookUpRate;

	/* Boolean indicating if the character is currently hiding*/
	bool IsHiding;

private:
	/* Boolean indicating if the character is running */
	bool IsRunning;

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

	/**
	* Called via input to change between world states and animations.
	* @param Intensity	This is the intensity of the animation. Only at 1 or -1 does it change states
	*/
	void DimensionShift(float Intensity);

	/* Called via input to indicate that the sprint action has started	*/
	void StartSprint();

	/* Called via input to indicate that the sprint action has ended */
	void StopSprint();

	/* Called via input to indicate that the hiding action has started */
	void StartHide();

	/* Called via input to indicate that the hiding action has ended */
	void StopHide();

	/* Called via input to indicate to toggle the Menu	*/
	void Menu();

	// TODO: Add Wall Climbing
	// TODO: Add Swimming

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// TODO: Add return of IsHiding (Not know if INLINE or not)
};

