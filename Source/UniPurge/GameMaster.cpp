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
		if(Generator.GetBlock(Point.first, Point.second) != Block::EMPTY)
			GenerarActor(Generator.GetBlock(Point.first, Point.second), Point.first, Point.second);
	}
	//We generate the houses
	//TODO search for roads and all adjacent empties; then start a globe on them
	int group = 0;
	for (int i = 0; i < Height; i++)
	{
		for(int j = 0; j < Width; j++)
			//If we find a road
		if (Generator.GetBlock(i, j) > Block::EMPTY && Generator.GetBlock(i, j) < Block::BUILDING)
		{
			//We search the adjacent blocks. And if any of them is empty, generate a building and start generating a group
			if (i < Height - 1	&& Generator.GetBlock(i + 1, j) == Block::EMPTY && Generator.GetGroup(i + 1, j) == -1)	GroupHouses(i + 1, j, group++);
			if (i > 0			&& Generator.GetBlock(i - 1, j) == Block::EMPTY && Generator.GetGroup(i - 1, j) == -1)	GroupHouses(i - 1, j, group++);
			if (j < Width - 1	&& Generator.GetBlock(i, j + 1) == Block::EMPTY && Generator.GetGroup(i, j + 1) == -1)	GroupHouses(i, j + 1, group++);
			if (j > 0			&& Generator.GetBlock(i, j - 1) == Block::EMPTY && Generator.GetGroup(i, j - 1) == -1)	GroupHouses(i, j - 1, group++);
			
			//Old spawning implementation with blueprints
			//SpawnActor(Generator.GetBlock(i, j), i * GridToCoordMult, j * GridToCoordMult);	
		}
	}
	//We operate on all the houses to build the sides
	while (!Actualizar.empty())
	{
		ABaseBlock* actor = Actualizar.front();
		Actualizar.pop();
		ActualizarActor(actor, actor->GetActorTransform().GetLocation().X/ GridToCoordMult, actor->GetActorTransform().GetLocation().Y/ GridToCoordMult);
	}
}

void AGameMaster::GroupHouses(int X, int Y, int group)
{
	std::discrete_distribution<int> var({ 0.5,1,2,1,0.5 });
	int modifier = var(generator) - 2;
	//The amount of blocks we want in the group
	int groupSize = AverageGroup + modifier;
	std::stack<std::pair<int,int>> posibilidades;
	int iterator = 0;
	posibilidades.push(std::pair<int,int>{ X,Y });
	//Iterative recursion
	while (iterator++ < groupSize && !posibilidades.empty())
	{
		std::pair<int, int> actual = posibilidades.top();
		UE_LOG(LogTemp, Warning, TEXT("UnaCasita en (%d, %d)"), actual.first, actual.second);
		posibilidades.pop();
		Generator.CreateHoses(actual.first, actual.second, group);
		GenerarActor(Generator.GetBlock(actual.first, actual.second), actual.first, actual.second);
		//Check the sides
		if (actual.first < Height-1	&& Generator.GetBlock(actual.first + 1, actual.second) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first + 1, actual.second });
		if (actual.first > 0		&& Generator.GetBlock(actual.first - 1, actual.second) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first - 1, actual.second });
		if (actual.second < Width-1 && Generator.GetBlock(actual.first, actual.second + 1) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first, actual.second + 1 });
		if (actual.second > 0		&& Generator.GetBlock(actual.first, actual.second - 1) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first, actual.second - 1 });
		
	}
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMaster::GenerarActor(Block ChosenRoad, int XPosition, int YPosition)
{
	const FVector Location = {StaticCast<float>(XPosition * GridToCoordMult), StaticCast<float>(YPosition * GridToCoordMult), 10.0};
	const FRotator Rotation = GetActorRotation();
	ABaseBlock* actor = GetWorld()->SpawnActor<ABaseBlock>(ActorToSpawn, Location, Rotation);
	if(ChosenRoad == Block::BUILDING)
		Actualizar.push(actor);
	
	actor->SetStats(ChosenRoad);
}

void AGameMaster::ActualizarActor(ABaseBlock* actor, int X, int Y)
{
	int group = Generator.GetGroup(X, Y);
	actor->SetNewExits(	Generator.CompareGroup(X + 1, Y, group),
						Generator.CompareGroup(X, Y + 1, group),
						Generator.CompareGroup(X - 1, Y, group),
						Generator.CompareGroup(X, Y - 1, group));

	actor->UpdateBuilding();
}

//Old spawning implementation with blueprints
//void AGameMaster::SpawnActor_Implementation(Block ChosenRoad, int XPosition, int YPosition) {}

