// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UniPurge/Enums.h"
#include "GameFramework/Actor.h"
#include "UniPurge/WorldGenerator.h"
#include <algorithm>
#include <iterator>


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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WorldGenerator Generator;

	void StartGeneration();

	Block ChooseBlock(int X, int Y);

	std::vector<int> ReduceOptions(const bool options[4], std::vector<int> currentOptions);

	Block BuildOther(int X, int Y);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spawning")
	void SpawnActor(Block ChosenRoad, int XPosition, int YPosition);

};
