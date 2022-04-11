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
	// +X == Oeste
	// -X == Este
	// +Y == Norte
	// -Y == Sur
	bool Directions[4] = { false };
	//If the element in the south has a north exit
	Directions[1] = (std::find(std::begin(NorthExits), std::end(NorthExits), Generator.GetBlock(X, Y - 1)) != std::end(NorthExits));
	Directions[0] = (std::find(std::begin(SouthExits), std::end(SouthExits), Generator.GetBlock(X, Y + 1)) != std::end(SouthExits));
	Directions[3] = (std::find(std::begin(EastExits), std::end(EastExits), Generator.GetBlock(X + 1, Y)) != std::end(EastExits));
	Directions[2] = (std::find(std::begin(WestExits), std::end(WestExits), Generator.GetBlock(X - 1, Y)) != std::end(WestExits));

	Block finalBlock = Block::EMPTY;

	if (std::find(std::begin(Directions), std::end(Directions), false))
		finalBlock = BuildOther(X, Y);

	if (finalBlock == Block::EMPTY) {
		std::array<Block, 8> initial = { Block::EMPTY };
		std::array<Block, 8> final = { Block::EMPTY };
		std::vector<Block> commonElements;
		std::vector<Block> finalElements;
		for (int i = 0; i < 4; i++)
		{
			if (Directions[i])
			{
				switch (i)
				{
				case 0:
					//Only 1 possible
					initial = NorthExits;
					break;
				case 1:
					//Only 2 possible
					if (initial[0] != Block::EMPTY)
						std::set_intersection(std::begin(initial), std::end(initial)
							, std::begin(SouthExits), std::end(SouthExits)
							, std::back_inserter(commonElements));
					else
						initial = SouthExits;
					break;
				case 2:
					if (commonElements.empty()) {
						//Not third element
						if (initial[0] != Block::EMPTY)
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
				case 3:
					if (finalElements.empty()) {
						//Not forth element
						if (commonElements.empty()) {
							//Not third element
							if (initial[0] != Block::EMPTY)
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
					else return Block::ROAD_N_S_E_W;
					break;
				}
			}
		}
		if (commonElements.empty())	commonElements.insert(commonElements.begin(), std::begin(initial), std::end(initial));
		if (finalElements.empty() || finalElements.size() == 0)	finalElements = commonElements;
		finalBlock = finalElements[std::rand() % finalElements.size()];
	}
	return finalBlock;
}

Block AGameMaster::BuildOther(int X, int Y)
{
	//Choose one option at random
	int value = Generator.GetInfluence(X, Y);
	value = (value > 100) ? 100 : value;
	UE_LOG(LogTemp, Warning, TEXT("Placing at (%d, %d). Value = %d"), X, Y, value);
	if (std::rand() % 100 >= value || (X == 0 && Y == 0))
		return Block::EMPTY;
	return StaticCast < Block>(16);
}

// Called every frame
void AGameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMaster::SpawnActor_Implementation(Block ChosenRoad, int XPosition, int YPosition) {}

