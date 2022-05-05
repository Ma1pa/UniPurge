// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UniPurge/UniPurgeCharacter.h>

#include "BasicNPC.generated.h"


UCLASS()
class UNIPURGE_API ABasicNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicNPC();

	UPROPERTY(EditAnywhere, Category = "Navver", BlueprintReadWrite)
		TArray<FVector> ListOfObjectives;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddWaypoint(FVector vector);

	UFUNCTION(BlueprintCallable)
		void PointReached();

	UFUNCTION(BlueprintNativeEvent)
		void EmpezarNavegar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navver")
		/*Destination the character wants to reach*/
		FVector Destination;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navver")
		AUniPurgeCharacter* jugador;

	UPROPERTY(EditAnywhere, Category = "Optimizer")
		float MaxDistance;

private:

	int currentWaypoint;

	bool disabled;

	float sumDelta;

	
};
