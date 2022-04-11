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
}

void AGameMaster::StartGeneration()
{
	for (int i = 0; i < Height * Width; i++) 
	{
		std::pair<int, int> Point = Generator.GetMostInfluenced();
		UE_LOG(LogTemp, Warning, TEXT("Position (%d, %d)"), Point.first, Point.second);
		Block objeto = ChooseBlock(Point.first, Point.second);
		SpawnActor(objeto, Point.first * GridToCoordMult, Point.second * GridToCoordMult);
		Generator.AddItem(Point.first, Point.second, objeto);
	}
}

Block AGameMaster::ChooseBlock(int X, int Y)
{
	//Take into account previous blocks
	// +Y == Este
	// -Y == Oeste
	// +X == Norte
	// -X == Sur
	//Array to know if we connect to a road in a certain position
	bool Directions[4] = { false };
	//If the element in the south has a north exit
	Directions[0] = (std::find(std::begin(SouthExits), std::end(SouthExits), (int)Generator.GetBlock(X + 1, Y)) != std::end(SouthExits));
	UE_LOG(LogTemp, Warning, TEXT("Item at north connects? %d"), Directions[0]);
	Directions[1] = (std::find(std::begin(NorthExits), std::end(NorthExits), (int)Generator.GetBlock(X - 1, Y)) != std::end(NorthExits));
	UE_LOG(LogTemp, Warning, TEXT("Item at south connects? %d"), Directions[1]);
	Directions[2] = (std::find(std::begin(WestExits), std::end(WestExits), (int)Generator.GetBlock(X, Y + 1)) != std::end(WestExits));
	UE_LOG(LogTemp, Warning, TEXT("Item at east connects? %d"), Directions[2]);
	Directions[3] = (std::find(std::begin(EastExits), std::end(EastExits), (int)Generator.GetBlock(X, Y - 1)) != std::end(EastExits));
	UE_LOG(LogTemp, Warning, TEXT("Item at west connects? %d"), Directions[3]);
	//Array to know if we connect to an empty in a certain position
	bool Empties[4] = { false };
	Empties[0] = Directions[0] == true ? false : Generator.GetBlock(X + 1, Y) == Block::EMPTY;
	Empties[1] = Directions[1] == true ? false : Generator.GetBlock(X - 1, Y) == Block::EMPTY;
	Empties[2] = Directions[2] == true ? false : Generator.GetBlock(X, Y + 1) == Block::EMPTY;
	Empties[3] = Directions[3] == true ? false : Generator.GetBlock(X, Y - 1) == Block::EMPTY;

	Block finalBlock = Block::EMPTY;

	//See if we connect to a previous road
	if (std::find(std::begin(Directions), std::end(Directions), true) != std::end(Directions))
	{
		UE_LOG(LogTemp, Warning, TEXT("Conecto con calle"));
		std::vector<int> limitador;

		limitador = ReduceOptions(Directions, limitador);
		//See if we connect to a non generated block
		if(std::find(std::begin(Empties), std::end(Empties), true) != std::end(Empties))
		{
			for (int i = 0; i < 4; i++)	Empties[i] = !Empties[i];
			limitador = ReduceOptions(Empties, limitador);
		}
		
		finalBlock = StaticCast <Block>(limitador[std::rand() % limitador.size()]);
	}
	//If we do not connect to any road
	else {
		UE_LOG(LogTemp, Warning, TEXT("No conecto con calle"));
		finalBlock = BuildOther(X, Y);
		//If we decide to randomly build a road
		if(finalBlock == Block::EMPTY)
		{
			std::vector<int> limitador;
			for (int i = 0; i < 4; i++)	Empties[i] = !Empties[i];
			limitador = ReduceOptions(Empties, limitador);
			finalBlock = StaticCast<Block>(limitador[std::rand() % limitador.size()]);
		}
	}

	
	if (finalBlock == Block::EMPTY)	finalBlock = StaticCast < Block>((std::rand()%15) + 1);
	return finalBlock;
}

std::vector<int> AGameMaster::ReduceOptions(const bool options[4], const std::vector<int> currentOptions)
{
	std::array<int, 8> initial = { };
	std::vector<int> commonElements;
	std::vector<int> finalElements;
	for (int i = 0; i < 4; i++)
	{
		if (options[i])
		{
			switch (i)
			{
			case 0:	//North
				//Only 1 possible
				initial = NorthExits;
				break;
			case 1:	//South
				//Only 2 possible
				if (initial[0] != 0)
					std::set_intersection(std::begin(initial), std::end(initial)
						, std::begin(SouthExits), std::end(SouthExits)
						, std::back_inserter(commonElements));
				else
					initial = SouthExits;
				break;
			case 2:	//East
				if (commonElements.empty()) {
					//Not third element
					if (initial[0] != 0)
						std::set_intersection(std::begin(initial), std::end(initial)
							, std::begin(EastExits), std::end(EastExits)
							, std::back_inserter(commonElements));
					else
						initial = EastExits;
				}
				else
				{
					//Third element
					std::set_intersection(std::begin(commonElements), std::end(commonElements)
						, std::begin(EastExits), std::end(EastExits)
						, std::back_inserter(finalElements));
				}
				break;
			case 3:	//West
				if (finalElements.empty()) {
					//Not forth element
					if (commonElements.empty()) {
						//Not third element
						if (initial[0] != 0)
							std::set_intersection(std::begin(initial), std::end(initial)
								, std::begin(WestExits), std::end(WestExits)
								, std::back_inserter(commonElements));
						else
							initial = WestExits;
					}
					else
					{
						//Third element
						std::set_intersection(std::begin(commonElements), std::end(commonElements)
							, std::begin(WestExits), std::end(WestExits)
							, std::back_inserter(finalElements));
					}
				}
				else
				{
					finalElements.clear();
					finalElements.push_back(15);
				}
				break;
			}
		}
	}
	if (commonElements.empty()) { commonElements.insert(commonElements.begin(), std::begin(initial), std::end(initial)); }
	if (finalElements.empty())	finalElements = commonElements;

	if(currentOptions.empty())
		return finalElements;
	else
		std::set_intersection(std::begin(currentOptions), std::end(currentOptions)
			, std::begin(finalElements), std::end(finalElements)
			, std::back_inserter(finalElements));
	return finalElements;
}

Block AGameMaster::BuildOther(int X, int Y)
{
	//Choose one option at random
	int value = Generator.GetInfluence(X, Y);
	value = (value > 100) ? 100 : value;
	UE_LOG(LogTemp, Warning, TEXT("Placing at (%d, %d). Value = %d"), X, Y, value);
	//Check if we put a road randomly
	if ( std::rand() % 100 < value || (X == 0 && Y == 0))
		return Block::EMPTY;
	return StaticCast < Block>(16);
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMaster::SpawnActor_Implementation(Block ChosenRoad, int XPosition, int YPosition) {}

