// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniPurge/Enums.h"

#include "BaseBlock.generated.h"



UCLASS()
class UNIPURGE_API ABaseBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBlock();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> ListOfMeshes;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		UStaticMesh* BasicFloor;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		UStaticMesh* BlockWall;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> OpenWall;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> DoorWall;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		UStaticMesh* ConectorWall;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> FloorMeshes;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> BlockMeshes;

	bool floored = false;

	UFUNCTION(blueprintCallable)
	void SetRenderDistance(int distance);

	void UpdateAll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//List of all possible elements
	UPROPERTY(BlueprintReadOnly)
	TArray<UStaticMeshComponent*> StaticElements;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStats(Block block, int height);

	void UpdateBuilding();

	void CreateBuildingElement(int side);

	void SetNewExits(int North, int South, int East, int West);

	void toggleFloor();

private:
	Connections horizontalExits[4]{Connections::NOTHING};
	Block currentBlock;

	UStaticMesh* GetMesh(Block selected);

	int HeightFromThis;

	ABaseBlock* blockUp;

	void UpdateAdditions(int element);

};
