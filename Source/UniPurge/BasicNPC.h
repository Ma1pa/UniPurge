// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UniPurge/UniPurgeCharacter.h>
#include <UniPurge/Enums.h>
#include <UniPurge/GameMaster.h>

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

	void Iniciar(WorldGenerator* gen, int tile);

	void UpdatePatrol();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	bool active;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void PointReached(bool isDefined);

	UFUNCTION(BlueprintNativeEvent)
		void EmpezarNavegar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navver")
		/*Destination the character wants to reach*/
		FVector Destination;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navver")
		AUniPurgeCharacter* jugador;

	UPROPERTY(EditAnywhere, Category = "Optimizer")
		float MaxDistance;
	UPROPERTY(EditAnywhere, Category = "Navver")
	int position;

private:

	WorldGenerator * generador;

	int currentWaypoint;

	float sumDelta;

	bool important;

	

	

	

	
};
