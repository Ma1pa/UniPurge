// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniPurge/WorldGenerator.h"
#include "UniPurge/Enums.h"
#include "UniPurge/BaseBlock.h"
#include <random>
#include <stack>
#include <queue>

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

	UPROPERTY(EditAnywhere)
		/* Width of the desired grid */
		int Width = 10;
	UPROPERTY(EditAnywhere)
		/* Height of the desired grid */
		int Height = 10;

private:
	const int GridToCoordMult = 800;
	std::default_random_engine generator;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorToSpawn;

	const int AverageGroup = 4;

	std::queue<ABaseBlock*> Actualizar;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WorldGenerator Generator;

	void StartGeneration();

	void GroupHouses(int X, int Y, int group);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Old spawning implementation with blueprints
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spawning")
	//void SpawnActor(Block ChosenRoad, int XPosition, int YPosition);

	void GenerarActor(Block ChosenRoad, int XPosition, int YPosition);
	void ActualizarActor(ABaseBlock* actor, int X, int Y);
};
