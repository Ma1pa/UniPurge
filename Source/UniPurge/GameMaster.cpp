// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMaster.h"
#include <UniPurge/BaseBlock.h>


// Sets default values
AGameMaster::AGameMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = StaticMesh;
	Generator = WorldGenerator(1, Height, Width);
	generator.seed(std::rand());
	//generator.seed(1);
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
	//We generate the roads
	for (int i = 0; i < Height * Width; i++) 
	{
		std::pair<int, int> Point = Generator.GetLessEntropy();
		UE_LOG(LogTemp, Warning, TEXT("Position (%d, %d)"), Point.first, Point.second);
		//We chose if we put empty or not 50%
		std::discrete_distribution<int> var({1,0.3});
		int option = var(generator);
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
				std::vector<int> dist;
				std::vector<int> op = Generator.GetOptions(Point.first, Point.second);
				for (int j = 0; j < Generator.GetPosibilities(Point.first, Point.second); j++)
				{
					if (op[j] < 12)	dist.push_back(12);
					else if (op[j] < 16)	dist.push_back(6);
					else	dist.push_back(1);
				}
				std::discrete_distribution<int> distribution (std::begin(dist), std::end(dist));
				option = distribution(generator);
				Generator.CollapseList(Point.first, Point.second, option);
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Block = %d"), (int)Generator.GetBlock(Point.first, Point.second));
		//Old spawning implementation with blueprints
		//SpawnActor(Generator.GetBlock(Point.first, Point.second), Point.first * GridToCoordMult, Point.second * GridToCoordMult);
		GenerarActor(Generator.GetBlock(Point.first, Point.second), Point.first * GridToCoordMult, Point.second * GridToCoordMult);
	}
	//We generate the houses
	//TODO search for roads and all adjacent empties; then start a globe on them
	for (int i = 0; i < Height; i++)
	{
		for(int j = 0; j < Width; j++)
			if (Generator.GetBlock(i, j) == Block::EMPTY)
			{
				Generator.CreateHoses(i, j);
				//Old spawning implementation with blueprints
				//SpawnActor(Generator.GetBlock(i, j), i * GridToCoordMult, j * GridToCoordMult);
				GenerarActor(Generator.GetBlock(i, j), i * GridToCoordMult, j * GridToCoordMult);
		}
	}
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMaster::GenerarActor(Block ChosenRoad, int XPosition, int YPosition)
{
	const FVector Location = {StaticCast<float>(XPosition), StaticCast<float>(YPosition), 10.0};
	const FRotator Rotation = GetActorRotation();
	ABaseBlock* actor = GetWorld()->SpawnActor<ABaseBlock>(ActorToSpawn, Location, Rotation);
	actor->SetStats(ChosenRoad);
}

//Old spawning implementation with blueprints
//void AGameMaster::SpawnActor_Implementation(Block ChosenRoad, int XPosition, int YPosition) {}

