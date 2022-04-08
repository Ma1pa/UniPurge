// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniPurge/WorldGenerator.h"
#include "UniPurge/Enums.h"

#include "GameMaster.generated.h"


UCLASS()
class UNIPURGE_API AGameMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameMaster();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

private:
	const int GridToCoordMult = 800;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WorldGenerator Generator;

	void StartGeneration();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spawning")
	void SpawnActor(Block ChosenRoad, int XPosition, int YPosition);

};
