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
		UStaticMesh* MainMesh;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> Addons_1;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> Addons_2;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> Addons_3;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> Addons_4;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> Addons_5;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> Addons_6 ;

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

	void SetStats(Block block, int height, UStaticMesh* newMesh, TArray<UStaticMesh*> Elements1, TArray<UStaticMesh*> Elements2, TArray<UStaticMesh*> Elements3, TArray<UStaticMesh*> Elements4, TArray<UStaticMesh*> Elements5, TArray<UStaticMesh*> Elements6);

	void UpdateBuilding();

	void CreateBuildingElement(int side, bool rotate);

	void SetNewExits(int North, int South, int East, int West);

	void toggleFloor();

	void Remove();

private:
	Connections horizontalExits[4]{Connections::NOTHING};
	Block currentBlock;

	void GetMesh(Block selected);

	int HeightFromThis;

	ABaseBlock* blockUp;

	void SetSideElement(int side, int choice);

	void SetOneSide(int side, bool sideClosed, bool rotate);

	bool placedCenter;

};
