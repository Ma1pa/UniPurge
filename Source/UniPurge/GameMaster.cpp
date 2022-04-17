// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMaster.h"

// Sets default values
AGameMaster::AGameMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = StaticMesh;
	Generator = WorldGenerator(1, Height, Width);
}

// Called when the game starts or when spawned
void AGameMaster::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Inicio Juego"));
	(&Generator)->~WorldGenerator();   //call the destructor explicitly
	new (&Generator) WorldGenerator(1, Height, Width);
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
		std::pair<int, int> Point = Generator.GetLessEntropy();
		UE_LOG(LogTemp, Warning, TEXT("Position (%d, %d)"), Point.first, Point.second);
		//We chose if we put empty or not 50%
		int option = std::rand() % 2;
		//int option = 0;
		//Check if an empty is possible
		if (option == 1 && Generator.CanEmpty(Point.first, Point.second))
		{
			Generator.CollapseOptions(Point.first, Point.second, 1);
		}
		else
		{
			if (Generator.GetPosibilities(Point.first, Point.second) <= 0) 
			{
				Generator.CollapseList(Point.first, Point.second, -1); 
				UE_LOG(LogTemp, Warning, TEXT("No options left"));
			}
			else
			{
				option = std::rand() % Generator.GetPosibilities(Point.first, Point.second);
				Generator.CollapseList(Point.first, Point.second, option);
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Block = %d"), (int)Generator.GetBlock(Point.first, Point.second));
		SpawnActor(Generator.GetBlock(Point.first, Point.second), Point.first * GridToCoordMult, Point.second * GridToCoordMult);
	}
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMaster::SpawnActor_Implementation(Block ChosenRoad, int XPosition, int YPosition) {}

