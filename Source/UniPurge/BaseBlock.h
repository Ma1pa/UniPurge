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

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Blocks", BlueprintReadWrite)
		TArray<UStaticMesh*> ListOfMeshes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//List of all avaiable meshes


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStats(Block block);

private:
	Block currentBlock;

	UStaticMesh* GetMesh(Block selected);

};
