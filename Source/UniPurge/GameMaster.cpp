// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMaster.h"

// Sets default values
AGameMaster::AGameMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = StaticMesh;
	Generator = WorldGenerator(1, Height, Width, 2);
}

// Called when the game starts or when spawned
void AGameMaster::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Inicio Juego"));
	(&Generator)->~WorldGenerator();   //call the destructor explicitly
	new (&Generator) WorldGenerator(1, Height, Width, 2);
	Super::BeginPlay();
	StartGeneration();

	//SpawnActor(Block::ROAD_N, 1 * GridToCoordMult, 1 * GridToCoordMult);
	//SpawnActor(Block::ROAD_S, 1 * GridToCoordMult, 2 * GridToCoordMult);
	//SpawnActor(Block::ROAD_E, 1 * GridToCoordMult, 3 * GridToCoordMult);
	//SpawnActor(Block::ROAD_W, 1 * GridToCoordMult, 4 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_S, 1 * GridToCoordMult, 5 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_E, 1 * GridToCoordMult, 6 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_W, 1 * GridToCoordMult, 7 * GridToCoordMult);
	//SpawnActor(Block::ROAD_S_E, 1 * GridToCoordMult, 8 * GridToCoordMult);
	//SpawnActor(Block::ROAD_S_W, 1 * GridToCoordMult, 9 * GridToCoordMult);
	//SpawnActor(Block::ROAD_E_W, 1 * GridToCoordMult, 10 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_S_E, 1 * GridToCoordMult, 11 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_S_W, 1 * GridToCoordMult, 12 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_E_W, 1 * GridToCoordMult, 13 * GridToCoordMult);
	//SpawnActor(Block::ROAD_S_E_W, 1 * GridToCoordMult, 14 * GridToCoordMult);
	//SpawnActor(Block::ROAD_N_S_E_W, 1 * GridToCoordMult, 15 * GridToCoordMult);
}

void AGameMaster::StartGeneration()
{
	for (int i = 0; i < Height * Width; i++) 
	{
		std::pair<int, int> Point = Generator.GetMostInfluenced();
		UE_LOG(LogTemp, Warning, TEXT("Position (%d, %d)"), Point.first, Point.second);
		Block objeto = StaticCast<Block>((std::rand() % 15) + 1);
		SpawnActor(objeto, Point.first * GridToCoordMult, Point.second * GridToCoordMult);
		Generator.AddItem(Point.first, Point.second, objeto);
	}
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMaster::SpawnActor_Implementation(Block ChosenRoad, int XPosition, int YPosition) {}

