// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMaster.h"
#include <UniPurge/BaseBlock.h>
#include <UniPurge/BasicNPC.h>



// Sets default values
AGameMaster::AGameMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = StaticMesh;
	
	
	seed = std::rand();
	generator.seed(seed);
	WorldGenerator Gen = WorldGenerator();
	Generator = &Gen;
	//generator.seed(1);
}

void AGameMaster::UpdateRender_Implementation(int NewDistance)
{
	RenderDistance = NewDistance;
}

void AGameMaster::UpdateNPCRadius(int NewRadius)
{
	RadiusOfSpawn = NewRadius;
	Generator->UpdateNPCR(NewRadius);
}

// Called when the game starts or when spawned
void AGameMaster::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Inicio Juego"));
	//(Generator)->~WorldGenerator();   //call the destructor explicitly
	Generator = new WorldGenerator(StaticCast<int>(RadiusOfSpawn), Side, this);
	
	Super::BeginPlay();
	StartGeneration();
	Generator->Side = Side;
	
	WallsToBorder();
}

void AGameMaster::StartGeneration()
{
	jugador->SetActorLocation(FVector{ StaticCast<float>(Side /2) * GridToCoordMult, StaticCast<float>(Side /2) * GridToCoordMult,250.0f });
	Generator->currentPlayerTile = Side / 2 * Side + Side / 2;
	//We generate the roads
	GenerateRoads();
	//We generate the rivers
	GenerateRivers();
	//We generate the houses
	GenerateHouses();
	//We generate NPCs in the world
	Generator->CheckPlayerPos(jugador);
	
	SpawnMainNPC();
	
}

void AGameMaster::GroupHouses(int X, int Y, int group, bool park)
{
	std::discrete_distribution<int> var({ 0.5,1,2,1,0.5 });
	int modifier = var(generator) - 2;
	//The amount of blocks we want in the group
	int groupSize = AverageGroup + modifier;
	std::stack<std::pair<int,int>> posibilidades;
	int iterator = 0;
	std::discrete_distribution<int> alt({ 1.25,2.5,5,10,20,10,5,2.5,1.5,1,0.8,0.6,0.4,0.2,0.1 });
	int altura = alt(generator);
	posibilidades.push(std::pair<int,int>{ X,Y });
	//Iterative recursion
	while (iterator++ < groupSize && !posibilidades.empty())
	{
		std::pair<int, int> actual = posibilidades.top();
		posibilidades.pop();
		Generator->CreateHoses(actual.first, actual.second, group, altura, park);
		GenerarActor(Generator->GetBlock(actual.first, actual.second), actual.first, actual.second);
		//Check the sides
		if (actual.first < Side -1	&& Generator->GetBlock(actual.first + 1, actual.second) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first + 1, actual.second });
		if (actual.first > 0		&& Generator->GetBlock(actual.first - 1, actual.second) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first - 1, actual.second });
		if (actual.second < Side -1 && Generator->GetBlock(actual.first, actual.second + 1) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first, actual.second + 1 });
		if (actual.second > 0		&& Generator->GetBlock(actual.first, actual.second - 1) == Block::EMPTY)	posibilidades.push(std::pair<int, int>{ actual.first, actual.second - 1 });
		
	}
}

void AGameMaster::GenerateRoads()
{
	for (int i = 0; i < Side * Side; i++)
	{
		std::pair<int, int> Point = Generator->GetLessEntropy();
		UE_LOG(LogTemp, Warning, TEXT("Position (%d, %d)"), Point.first, Point.second);
		//We chose if we put empty or not 50%
		std::discrete_distribution<int> var({ 1,0.3 });
		int option = var(generator);
		//int option = 0;
		//Check if an empty is possible
		if (option == 1 && Generator->CanEmpty(Point.first, Point.second))
		{
			Generator->CollapseOptions(Point.first, Point.second, 1);
		}
		else
		{
			if (Generator->GetPosibilities(Point.first, Point.second) <= 0)
			{
				Generator->CollapseList(Point.first, Point.second, -1);
				UE_LOG(LogTemp, Warning, TEXT("No options left"));
			}
			else
			{
				std::vector<int> dist;
				std::vector<int> op = Generator->GetOptions(Point.first, Point.second);
				for (int j = 0; j < Generator->GetPosibilities(Point.first, Point.second); j++)
				{
					//We assign weights according to the exits
					if (op[j] < 12)	dist.push_back(12);
					else if (op[j] < 16)	dist.push_back(6);
					else	dist.push_back(1);
				}
				std::discrete_distribution<int> distribution(std::begin(dist), std::end(dist));
				option = distribution(generator);
				Generator->CollapseList(Point.first, Point.second, option);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Block = %d"), (int)Generator->GetBlock(Point.first, Point.second));
		//Old spawning implementation with blueprints
		//SpawnActor(Generator.GetBlock(Point.first, Point.second), Point.first * GridToCoordMult, Point.second * GridToCoordMult);
		if (Generator->GetBlock(Point.first, Point.second) != Block::EMPTY)
			GenerarActor(Generator->GetBlock(Point.first, Point.second), Point.first, Point.second);

		//Generate places to place NPCs and their waypoints
		int X = (int)(i / Side) * GridToCoordMult;
		int Y = (int)(i % Side) * GridToCoordMult;
		Generator->StorePosition(i, FVector{ StaticCast<float>(X), StaticCast<float>(Y), 250.0f });
		FVector puntos[4] = {
					{StaticCast<float>(X),StaticCast<float>(Y),250.0f},
					{StaticCast<float>(X + 4 * GridToCoordMult),StaticCast<float>(Y),250.0f},
					{StaticCast<float>(X),StaticCast<float>(Y + 4 * GridToCoordMult),250.0f},
					{StaticCast<float>(X - 4 * GridToCoordMult),StaticCast<float>(Y),250.0f} };
		Generator->SetWaypoints((int)(i / Side), i % Side, puntos);
	}
}

void AGameMaster::GenerateRivers()
{
	//TODO
}

void AGameMaster::GenerateHouses()
{
	int group = 0;
	for (int i = 0; i < Side; i++)
	{
		for (int j = 0; j < Side; j++)
			//If we find a road
			if (Generator->GetBlock(i, j) > Block::EMPTY && Generator->GetBlock(i, j) < Block::BUILDING)
			{
				std::discrete_distribution<int> ProbParque({ 2,0.5 });
				//We search the adjacent blocks. And if any of them is empty, generate a building and start generating a group
				if (i < Side - 1 && Generator->GetBlock(i + 1, j) == Block::EMPTY && Generator->GetGroup(i + 1, j) == -1)	GroupHouses(i + 1, j, group++, ProbParque(generator) == 1);
				if (i > 0 && Generator->GetBlock(i - 1, j) == Block::EMPTY && Generator->GetGroup(i - 1, j) == -1)	GroupHouses(i - 1, j, group++, ProbParque(generator) == 1);
				if (j < Side - 1 && Generator->GetBlock(i, j + 1) == Block::EMPTY && Generator->GetGroup(i, j + 1) == -1)	GroupHouses(i, j + 1, group++, ProbParque(generator) == 1);
				if (j > 0 && Generator->GetBlock(i, j - 1) == Block::EMPTY && Generator->GetGroup(i, j - 1) == -1)	GroupHouses(i, j - 1, group++, ProbParque(generator) == 1);
			}
	}
	//We operate on all the houses to build the sides
	while (!Actualizar.empty())
	{
		ABaseBlock* actor = Actualizar.front();
		Actualizar.pop();
		ActualizarActor(actor, actor->GetActorTransform().GetLocation().X / GridToCoordMult, actor->GetActorTransform().GetLocation().Y / GridToCoordMult);
	}
}

void AGameMaster::SpawnMainNPC()
{
	//We Spawn the main NPC in the world
	const FRotator Rotation = GetActorRotation();
	//Position random of the outer circle, either side
	FVector Posicion = { StaticCast<float>((std::rand() % (Side / 3)) + ((std::rand() % 2) * (4 * (Side / 6))) * GridToCoordMult), StaticCast<float>((std::rand() % (Side / 3)) + ((std::rand() % 2) * (4 * (Side / 6))) * GridToCoordMult),250.0f };
	ABasicNPC* NPC = GetWorld()->SpawnActor<ABasicNPC>(MainNPC, Posicion, Rotation);
	NPC->Iniciar(Generator, -1);
	NPC->jugador = jugador;
	NPC->ListOfObjectives.Add(FVector(Posicion));
	NPC->ListOfObjectives.Add(FVector(0.0f, 0.0f, 250.0f));
	NPC->ListOfObjectives.Add(FVector(std::rand() % (Side / 2) * GridToCoordMult, std::rand() % (Side / 2) * GridToCoordMult, 250.0f));
	NPC->ListOfObjectives.Add(FVector(Side * GridToCoordMult - 200, Side * GridToCoordMult - 200, 250.0f));
	NPC->ListOfObjectives.Add(FVector((std::rand() % (Side / 2)) * 2 * GridToCoordMult, (std::rand() % (Side / 2)) * 2 * GridToCoordMult, 250.0f));
	NPC->PointReached(false);
	NPCsActivos.Add(NPC);
}

void AGameMaster::AddTrap(ATrap* trap)
{
	TrampasPuestas.Add(&(*trap));
}

void AGameMaster::ExitShift()
{
	WallsToBorder();
	if (TrampasPuestas.Num() > 0)
	{
		for (int i = 0; i < TrampasPuestas.Num(); i++)
		{
			TrampasPuestas[i]->SetShift(false);
		}
	}
}

void AGameMaster::EnterShift(int radius)
{
	WallsToCoord(radius);
	ReloadAdditions();
	TrampasPuestas.Remove(nullptr);
	if (TrampasPuestas.Num() > 0)
	{	
		for (int i = 0; i < TrampasPuestas.Num(); i++)
		{
			TrampasPuestas[i]->SetShift(true);
		}
	}
}

void AGameMaster::RemoveTrap(ATrap* trap)
{
	TrampasPuestas.Remove(trap);
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerIsShifting)
	{
		Generator->CheckPlayerPos(jugador);
		CheckFirstTrap();
	}
}

void AGameMaster::CheckFirstTrap()
{
	if(TrampasPuestas.Num() > 0 && TrampasPuestas[0]->TimeLeft <= 0.0f)
	{
		ATrap* trampa = TrampasPuestas[0];
		TrampasPuestas.Remove(trampa);
		trampa->Destroy();
		CheckFirstTrap();
	}
}

void AGameMaster::GenerarActor(Block ChosenRoad, int XPosition, int YPosition)
{
	const FVector Location = {StaticCast<float>(XPosition * GridToCoordMult), StaticCast<float>(YPosition * GridToCoordMult), 10.0};
	const FRotator Rotation = GetActorRotation();
	ABaseBlock* actor = GetWorld()->SpawnActor<ABaseBlock>(ActorToSpawn, Location, Rotation);
	std::discrete_distribution<int> alt({ 1,1.5 });
	Generator->AddAgent(XPosition, YPosition, actor, alt(generator) != 0);
	if(ChosenRoad == Block::BUILDING)
		Actualizar.push(actor);
	else
		actor->SetStats(ChosenRoad, Generator->GetHeight(XPosition, YPosition));
}

void AGameMaster::ActualizarActor(ABaseBlock* actor, int X, int Y)
{
	
	int group = Generator->GetGroup(X, Y);
	actor->toggleFloor();
	actor->SetNewExits(	Generator->CompareGroup(X + 1, Y, group),
						Generator->CompareGroup(X, Y + 1, group),
						Generator->CompareGroup(X - 1, Y, group),
						Generator->CompareGroup(X, Y - 1, group));
	actor->SetStats(Generator->GetBlock(X, Y), Generator->GetHeight(X, Y));
	
	actor->UpdateBuilding();
}

void AGameMaster::SpawnDirection(int position)
{
	GenerarNPC(position, Generator->RetrievePosition(position));
}

void AGameMaster::GenerarNPC(int puntoSpawn, FVector Posicion)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawneando NPC"));
	const FRotator Rotation = GetActorRotation();
	ABasicNPC* actor = GetWorld()->SpawnActor<ABasicNPC>(NPCToSpawn, Posicion, Rotation);
	actor->Iniciar(Generator, puntoSpawn);
	actor->jugador = jugador;
	actor->UpdatePatrol();
	NPCsActivos.Add(actor);
}

void AGameMaster::RemoveActor(ABasicNPC* actor)
{
	NPCsActivos.Remove(actor);
}

void AGameMaster::WallsToBorder()
{
	for (int i = 0; i < WorldWalls.Num(); i++)
	{
		switch (i)
		{
		case 0:
			WorldWalls[i]->SetActorLocation(FVector{ StaticCast<float>(Side * GridToCoordMult),0,0 });
			break;
		case 1:
			WorldWalls[i]->SetActorLocation(FVector{ 0,0,0 });
			break;
		case 2:
			WorldWalls[i]->SetActorLocation(FVector{ 0,StaticCast<float>(Side * GridToCoordMult),0 });
			break;
		case 3:
			WorldWalls[i]->SetActorLocation(FVector{ 0,0,0 });
			break;
		}
	}
}

void AGameMaster::WallsToCoord( int radius)
{
	int X = Generator->currentPlayerTile / Side;
	int Y = Generator->currentPlayerTile % Side;
	for (int i = 0; i < WorldWalls.Num(); i++)
	{
		switch (i)
		{
		case 0:
			WorldWalls[i]->SetActorLocation(FVector{ std::min(StaticCast<float>((X + radius) * GridToCoordMult), StaticCast<float>(Side * GridToCoordMult)), StaticCast<float>(Y * GridToCoordMult), 0 });
			break;
		case 1:
			WorldWalls[i]->SetActorLocation(FVector{ std::max(StaticCast<float>((X - radius) * GridToCoordMult), 0.0f), StaticCast<float>(Y * GridToCoordMult), 0 });
			break;
		case 2:
			WorldWalls[i]->SetActorLocation(FVector{ StaticCast<float>(X * GridToCoordMult), std::min(StaticCast<float>((Y + radius) * GridToCoordMult), StaticCast<float>(Side * GridToCoordMult)), 0 });
			break;
		case 3:
			WorldWalls[i]->SetActorLocation(FVector{ StaticCast<float>(X * GridToCoordMult), std::max(StaticCast<float>((Y - radius) * GridToCoordMult), 0.0f), 0 });
			break;
		}
	}
}

void AGameMaster::ReloadAdditions()
{
	Generator->UpdateAdditions();
}
