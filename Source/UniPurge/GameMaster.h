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
#include <UniPurge/UniPurgeCharacter.h>

#include "GameMaster.generated.h"

class UNIPURGE_API WorldGenerator;

struct Tile
{
	/* Location of the tile in the array representing the 2D grid*/
	int Location;
	/* Current possible blocks in the tile. Its length is the entropy */
	std::vector<int> posibilities;
	/* Block currently build in the location*/
	Block block;
	/* Group of the tile */
	int group;
	/* Height of the block */
	int height;
	/* Agent of the tile */
	ABaseBlock* agent;
	/* The assigned checkpoints of the route */
	FVector waypoints[4];
	/* The stored position of the agent when leaving the zone*/
	FVector agentPos;
	/* If the NPC is Spawned */
	bool spawned = false;
};

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
		/* Side of the square of the desired grid */
		int Side = 10;


	UPROPERTY(EditAnywhere, Category = "Jugador")
		AUniPurgeCharacter* jugador;

	int RadiusOfSpawn = 4800;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> NPCToSpawn;

	const int AverageGroup = 4;

	std::queue<ABaseBlock*> Actualizar;
	int seed;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	WorldGenerator* Generator;

	void StartGeneration();

	void GroupHouses(int X, int Y, int group, bool park);

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Old spawning implementation with blueprints
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Spawning")
	//void SpawnActor(Block ChosenRoad, int XPosition, int YPosition);

	void GenerarActor(Block ChosenRoad, int XPosition, int YPosition);
	void ActualizarActor(ABaseBlock* actor, int X, int Y);

	void SpawnDirection(int position);

	void GenerarNPC(int puntoSpawn, FVector Posicion);
};



/* Class used to generate the world in which the game takes place */
class UNIPURGE_API WorldGenerator
{
public:

	WorldGenerator(int S, int H, AGameMaster* mast);
	WorldGenerator();
	~WorldGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GenerationOptions)
		/* Side of the world generated (2D) */
		int Side;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GenerationOptions)
		/* Seed to use in the random generation */
		int Seed;

	/* Function called to recieve the tile with the least entropy
	* @return Int pair indicating the 2D position of the tile in the graph
	*/
	std::pair<int, int> GetLessEntropy();

	/* Funtion called to recieve the number of the tile in the 2D position
	* @param X : The X position in the array
	* @param Y : The Y position in the array
	*/
	Block GetBlock(int X, int Y);

	/* Function called to know if an empty can be placed in the 2D position
	* @param X : The X position in the array
	* @param Y : The Y position in the array
	*/
	bool CanEmpty(int X, int Y);

	/* Function called to get the length of the vector with all the posibilities
	* @param X : The X position in the array
	* @param Y : The Y position in the array
	*/
	int GetPosibilities(int X, int Y);

	void AddAgent(int X, int Y, ABaseBlock* bloque);

	/* Function called whenever a block is chosen. It collapses other options in the tile and simplifies the others
	* @param X : The X position in the array where the block will be placed
	* @param Y : The Y position in the array where the block will be placed
	* @param chosen : The chosen block for the selected position
	*/
	void CollapseOptions(int X, int Y, int chosen);

	/* Function called whenever a block is chosen. It collapses other options in the tile and simplifies the others
	* @param X : The X position in the array where the block will be placed
	* @param Y : The Y position in the array where the block will be placed
	* @param chosen : The chosen block for the selected position
	*/
	void CollapseList(int X, int Y, int chosen);

	std::vector<int> GetOptions(int X, int Y);

	int CompareGroup(int X, int Y, int group);

	int GetGroup(int X, int Y);

	int GetHeight(int X, int Y);

	void CreateHoses(int X, int Y, int group, int h, bool park);

	void GetOppositeTile(int ogTile, FVector posPlayer);

	void SetWaypoints(int X, int Y, FVector Waypoints[4]);

	FVector* GetWaypoints(int position);

	void StorePosition(int position, FVector pos);

	FVector RetrievePosition(int position);

	bool IsFarAway(int position, FVector JPos);

	int currentPlayerTile;
	Direction playerLastMovement;

protected:
	/* Tile map used to guide generation */
	std::vector<Tile> TileMap;

	/* Function called recursively or by "CollapseOptions". It is used to remove all the adjacent blocks to the removed block
	* @param X : The X position in the array where the options will be collapsed
	* @param Y : The Y position in the array where the options will be collapsed
	* @param MovementFromRemoved : The direction from the previous tile where the funciton was called
	* @param collides : If the road from Removed collides with the tile
	*/
	void RecursivePropagation(int X, int Y, Direction MovementFromRemoved, bool collides, bool isEmpty);

	bool CheckSpots(int X, int Y);

	void CallSides(int X, int Y, int block);

private:

	bool RoadInDirection(Block road, Direction direction);
	AGameMaster* master;



	/**
	* Function used to attain the Array position of a 2D position
	* @param Columna : The X Position ^
	* @param Fila : The Y Position ->
	* @return Number indicating the location in the list
	*/
	int get_1d(int X, int Y);
	/**
	* Function used to attain the 2D position from the Array
	* @param Point : The position in the array
	* @return Pair indicating the X and Y positions of the location
	*/
	std::pair<int, int> get_2d(int Pos);
};