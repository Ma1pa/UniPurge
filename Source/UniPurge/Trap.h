// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class UNIPURGE_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();
	UPROPERTY(EditAnywhere, Category = "Models", BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Models")
		int currentNumber;

	UPROPERTY(EditAnywhere, Category = "Models", BlueprintReadWrite)
	TArray<UStaticMesh*> ListOfMeshes;

	UPROPERTY(EditAnywhere, Category = "Models", BlueprintReadWrite)
		TArray<float> ListOfCosts;

	UPROPERTY(EditAnywhere, Category = "Models", BlueprintReadWrite)
		TArray<float> ListOfScale;
	UPROPERTY(EditAnywhere, Category = "Models", BlueprintReadWrite)
		TArray<float> ListOfDuration;

	void PlaceTrap();

	UPROPERTY(EditAnywhere, Category = "Trap", BlueprintReadWrite)
		float MaxTime;

	float TimeLeft;

	float GetCurrentCost();

	UFUNCTION(BlueprintCallable, Category = "Trap")
	void NextTrap();
	UFUNCTION(BlueprintCallable, Category = "Trap")
	void PreviousTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void LoadModel();
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isShift;

	FORCEINLINE void SetShift(bool onOrOff) { isShift = onOrOff; SettingShift(onOrOff); }

	UFUNCTION(BlueprintImplementableEvent)
		void SettingShift(bool onOrOff);

};
