// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerator.h"
#include "GameMaster.h"

WorldGenerator::WorldGenerator(){};
WorldGenerator::WorldGenerator(int RR, int H, AGameMaster* mast)
{
	//TODO Obtener todos los bojetos que se pueden generar
	master = mast;
	Side = H;
	NPCRadius = RR;
	currentPlayerTile = 0;
	isUpdating = false;
	for (int i = 0; i < Side * Side; i++)
	{
		TileMap.push_back(Tile());
		TileMap[i].Location = i;
		TileMap[i].posibilities.assign(AllRoads, AllRoads + 14);
		//TileMap[i].posibilities.assign(AllSimpleRoads, AllSimpleRoads + 7);
		//for (int j = 0; j < 16; j++)
		//	TileMap[i].posibilities.push_back(j+1);
		TileMap[i].block = Block::NOTHING;
		TileMap[i].group = -1;
		TileMap[i].height = 0;
	}
	//NPCRadius = 4800;
	RenderRadius = 32000;
	
}

WorldGenerator::~WorldGenerator()
{
}

std::pair<int, int> WorldGenerator::GetLessEntropy()
{
	//Brute force method. 
	//TODO Fix for better system
	int bestPosition = 0;
	for (int i = 1; i < TileMap.size(); i++)
	{

		//The position selected has to be Nothing and have the minimal size or be a straight river
		if ((TileMap[i].block == Block::NOTHING &&
			TileMap[i].posibilities.size() < TileMap[bestPosition].posibilities.size()) || 
			(TileMap[bestPosition].block != Block::NOTHING))
			bestPosition = i;
	}
	UE_LOG(LogTemp, Warning, TEXT("Best Position = %d Block = %d"), bestPosition,(int)TileMap[bestPosition].block);
	return get_2d(bestPosition);
}

void WorldGenerator::CollapseList(int X, int Y, int chosen)
{
	if (chosen == -1)
	{
		//There are no options left; so we search for any of the no exit routes
		//North
		if (X < (Side - 1) && RoadInDirection(TileMap[get_1d(X + 1, Y)].block, Direction::SOUTH))	CollapseOptions(X, Y, 2);
		//South
		else if (X > 0 && RoadInDirection(TileMap[get_1d(X - 1, Y)].block, Direction::NORTH))	CollapseOptions(X, Y, 3);
		//East
		else if (Y < (Side - 1) && RoadInDirection(TileMap[get_1d(X, Y + 1)].block, Direction::WEST))	CollapseOptions(X, Y, 4);
		//West
		else if (Y > 0 && RoadInDirection(TileMap[get_1d(X, Y - 1)].block, Direction::EAST))	CollapseOptions(X, Y, 5);
		else CollapseOptions(X, Y, 1);
	}
	else
		CollapseOptions(X, Y, TileMap[get_1d(X, Y)].posibilities[chosen]);
}

std::vector<int> WorldGenerator::GetOptions(int X, int Y)
{
	return TileMap[get_1d(X, Y)].posibilities;
}

int WorldGenerator::CompareGroup(int X, int Y, int group)
{
	//The point is not in the tilemap
	if (X < 0 || Y < 0 || X >= Side || Y >= Side || (TileMap[get_1d(X, Y)].block > Block::ROAD_N_S_E_W && TileMap[get_1d(X, Y)].block < Block::BUILDING))	return 0;
	//The point is in the tilemap
	if (TileMap[get_1d(X, Y)].block != Block::BUILDING && TileMap[get_1d(X, Y)].block > Block::EMPTY)	return 1;
	if (TileMap[get_1d(X, Y)].group == group)	return 2;
	return 3;
}

int WorldGenerator::GetGroup(int X, int Y)
{
	if (X < 0 || Y < 0 || get_1d(X, Y) > std::size(TileMap))	return -1;
	return TileMap[get_1d(X, Y)].group;
}

int WorldGenerator::GetHeight(int X, int Y)
{
	return TileMap[get_1d(X, Y)].height;
}

void WorldGenerator::CreateHoses(int X, int Y, int group, int h, bool park)
{
	if(park)	TileMap[get_1d(X, Y)].block = Block::PARK;
	else	TileMap[get_1d(X, Y)].block = Block::BUILDING;
	TileMap[get_1d(X, Y)].group = group;
	TileMap[get_1d(X, Y)].height = h;
}

void WorldGenerator::CollapseOptions(int X, int Y, int chosen)
{
	Block selected = StaticCast<Block>(chosen);
	TileMap[get_1d(X, Y)].block = selected;
	//Clean all other possible options and remove such options from the adjacent tiles
	TileMap[get_1d(X, Y)].posibilities = std::vector<int>{ chosen };
	
	CallSides(X, Y, chosen);

}

void WorldGenerator::RecursivePropagation(int X, int Y, Direction MovementFromRemoved, bool collides, bool isEmpty)
{
	if (TileMap[get_1d(X, Y)].block != Block::NOTHING)	return;
	std::vector<int> options;
	//if the block collides another block but it cannot connect; make it empty
	//We firstly check if the block connects to another road in another side
	if (!collides && !isEmpty && !CheckSpots(X,Y))
	{
		options.push_back(1);
		TileMap[get_1d(X, Y)].posibilities = options;
		//We do another recursion since we no longer can connect to the adjacent blocks
		CallSides(X, Y, 1);
		return;
	}
	//We get the intersection between all current posibilities and all posibilities that connect
	switch (MovementFromRemoved)
	{
		case Direction::NORTH:
			std::set_intersection(std::begin(AllSouth), std::end(AllSouth), TileMap[get_1d(X, Y)].posibilities.begin(), TileMap[get_1d(X, Y)].posibilities.end(), back_inserter(options));
			break;
		case Direction::SOUTH:
			std::set_intersection(std::begin(AllNorth), std::end(AllNorth), TileMap[get_1d(X, Y)].posibilities.begin(), TileMap[get_1d(X, Y)].posibilities.end(), back_inserter(options));
			break;
		case Direction::EAST:
			std::set_intersection(std::begin(AllWest), std::end(AllWest), TileMap[get_1d(X, Y)].posibilities.begin(), TileMap[get_1d(X, Y)].posibilities.end(), back_inserter(options));
			break;
		case Direction::WEST:
			std::set_intersection(std::begin(AllEast), std::end(AllEast), TileMap[get_1d(X, Y)].posibilities.begin(), TileMap[get_1d(X, Y)].posibilities.end(), back_inserter(options));
			break;
	}

	//If there is a road facing this place, we only keep the options that connect
	if (collides) 
	{
		TileMap[get_1d(X, Y)].posibilities = options;
	}
	//If there is no road facing this place, we remove all the options that connect to such road
	else
	{
		std::vector<int> rest;
		//We remove those posibilities from the current ones
		std::set_symmetric_difference(	TileMap[get_1d(X, Y)].posibilities.begin(), TileMap[get_1d(X, Y)].posibilities.end(),
										options.begin(), options.end(), std::back_inserter(rest));
		TileMap[get_1d(X, Y)].posibilities = rest;
	}

	//We do not require another recursion since we can still connect to unknown blocks
	//CallSides(X, Y, 0);
}

bool WorldGenerator::CheckSpots(int X, int Y)
{
	if (X < (Side - 1) && RoadInDirection(TileMap[get_1d(X + 1, Y)].block, Direction::SOUTH))	return true;
	if (X > 0 &&			RoadInDirection(TileMap[get_1d(X-1, Y)].block, Direction::NORTH))	return true;
	if (Y < (Side - 1) &&	RoadInDirection(TileMap[get_1d(X, Y+1)].block, Direction::WEST))	return true;
	if (Y > 0 &&			RoadInDirection(TileMap[get_1d(X, Y-1)].block, Direction::EAST))	return true;
	return false;
}

void WorldGenerator::CallSides(int X, int Y, int block)
{
	//Call to north
	if (X < (Side - 1))	RecursivePropagation(X + 1, Y, Direction::NORTH, RoadInDirection(StaticCast<Block>(block), Direction::NORTH), block == 1);
	//Call to south
	if (X > 0)	RecursivePropagation(X - 1, Y, Direction::SOUTH, RoadInDirection(StaticCast<Block>(block), Direction::SOUTH), block == 1);
	//Call to east
	if (Y < (Side - 1))	RecursivePropagation(X, Y + 1, Direction::EAST, RoadInDirection(StaticCast<Block>(block), Direction::EAST), block == 1);
	//Call to west
	if (Y > 0)	RecursivePropagation(X, Y - 1, Direction::WEST, RoadInDirection(StaticCast<Block>(block), Direction::WEST), block == 1);
}

bool WorldGenerator::RoadInDirection(Block road, Direction direction)
{
	switch (direction) 
	{
	case Direction::NORTH:
		return (std::find(std::begin(AllNorth), std::end(AllNorth), (int)road) != std::end(AllNorth));
		break;
	case Direction::SOUTH:
		return (std::find(std::begin(AllSouth), std::end(AllSouth), (int)road) != std::end(AllSouth));
		break;
	case Direction::EAST:
		return (std::find(std::begin(AllEast), std::end(AllEast), (int)road) != std::end(AllEast));
		break;
	case Direction::WEST:
		return (std::find(std::begin(AllWest), std::end(AllWest), (int)road) != std::end(AllWest));
		break;
	default:
		return false;
		break;
	}
}


int WorldGenerator::get_1d(int X, int Y)	{ return Y + Side * X; }

std::pair<int, int> WorldGenerator::get_2d( int Pos){
	return { (int)(Pos / Side), (int)(Pos % Side)}; }

Block WorldGenerator::GetBlock(int X, int Y)
{
	return TileMap[get_1d(X, Y)].block;
}

bool WorldGenerator::CanEmpty(int X, int Y)
{
	return TileMap[get_1d(X, Y)].posibilities[0] == 1;
}

int WorldGenerator::GetPosibilities(int X, int Y)
{
	return TileMap[get_1d(X, Y)].posibilities.size();
}

void WorldGenerator::AddAgent(int X, int Y, ABaseBlock* bloque, bool hasNPC)
{
	TileMap[get_1d(X, Y)].agent = bloque;
	TileMap[get_1d(X, Y)].hasNPC = hasNPC;
}

void WorldGenerator::SetWaypoints(int X, int Y, FVector Waypoints[4])
{
	if (X < 0 || Y < 0)	return;
	TileMap[get_1d(X, Y)].waypoints[0] = Waypoints[0];
	TileMap[get_1d(X, Y)].waypoints[1] = Waypoints[1];
	TileMap[get_1d(X, Y)].waypoints[2] = Waypoints[2];
	TileMap[get_1d(X, Y)].waypoints[3] = Waypoints[3];
}

FVector* WorldGenerator::GetWaypoints(int position)
{
	if (position < 0)	return nullptr;
	return TileMap[position].waypoints;
}

void WorldGenerator::StorePosition(int position, FVector pos)
{
	if (position < 0)	return;
	TileMap[position].agentPos = pos;
}

FVector WorldGenerator::RetrievePosition(int position)
{
	if (position < 0)	return FVector{ -800,-800,250 };
	return TileMap[position].agentPos;
}

bool WorldGenerator::IsFarAway(int position, FVector JPos)
{
	//We get which tile the player is on
	std::pair<int, int> playerTile = { StaticCast<int>((JPos.X + (GridToCoordMult/2)) / GridToCoordMult), StaticCast<int>((JPos.Y + (GridToCoordMult/2)) / GridToCoordMult) };
	std::pair<int, int> NPCTile = get_2d(position);
	return abs(sqrt(pow(playerTile.first - NPCTile.first, 2) + pow(playerTile.second - NPCTile.second, 2))) > StaticCast <int>(NPCRadius);

}

void WorldGenerator::EliminarActor(ABasicNPC* actor, int tile)
{
	master->RemoveActor(actor);
	TileMap[tile].spawned = false;
}

void WorldGenerator::CheckPlayerPos(AUniPurgeCharacter* jugador)
{
	//We check if the player changed tile.
	//If so, we change the tile and spawn NPCs

	std::pair<int, int> playerTile = { StaticCast<int>((jugador->GetActorLocation().X + 400) / 800), StaticCast<int>((jugador->GetActorLocation().Y + 400) / 800)};
	int jug = get_1d(playerTile.first, playerTile.second);
	if (jug != currentPlayerTile)
	{
		//The player changed the tile they are on
		ChangedTile(currentPlayerTile, jug);
	}
}

void WorldGenerator::UpdateNPCR(int NewRadius)
{
	NPCRadius = NewRadius;
}

void WorldGenerator::LoadUpdates()
{
	initialUpdate = std::rand() % TileMap.size();
	currentUpdate = initialUpdate;
	isUpdating = true;
}

void WorldGenerator::ChangedTile(int OldTile, int NewTile)
{
	currentPlayerTile = NewTile;
	std::pair<int, int> pos = get_2d(NewTile);

	//We check for the sphere around the player and spawn any non spawned NPCs
	for (int X = -NPCRadius; X <= NPCRadius; X++)
	{
		for (int Y = -NPCRadius; Y <= NPCRadius; Y++)
		{
			if (TileMap[get_1d(X + pos.first, Y + pos.second)].hasNPC && !TileMap[get_1d(X + pos.first, Y + pos.second)].spawned)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawn in = %d"), get_1d(X + pos.first, Y + pos.second));
				TileMap[get_1d(X + pos.first, Y + pos.second)].spawned = true;
				master->SpawnDirection(get_1d(X + pos.first, Y + pos.second));
				//Spawn in the various heights of a building, with a variable change depending on height
				if(TileMap[get_1d(X + pos.first, Y + pos.second)].height > 0 && TileMap[get_1d(X + pos.first, Y + pos.second)].block == Block::BUILDING)
				{
					int altura = TileMap[get_1d(X + pos.first, Y + pos.second)].height + 1;
					int MaxAltura = altura;
					while (altura > 0)
					{
						std::discrete_distribution<int> ProbSpawn({ 1,StaticCast<double>(altura/MaxAltura) });
						if (ProbSpawn(generator))
						{
							FVector localiz = TileMap[get_1d(X + pos.first, Y + pos.second)].agent->GetActorLocation();
							localiz = { localiz.X, localiz.Y, StaticCast<float>(260 + 300 * altura) };
							master->GenerarNPC(get_1d(X + pos.first, Y + pos.second), localiz);
						}
						altura--;
					}
				}
			}
		}
	}
}

void WorldGenerator::UpdateAdditions(int amount)
{
	//Call a function in all tiles
	for (int i = 0; i < amount; i++)
	{
		TileMap[currentUpdate].agent->UpdateAll();
		currentUpdate++;
		if (currentUpdate >= TileMap.size()-1)
			currentUpdate = 0;
		if (initialUpdate == currentUpdate)
		{
			isUpdating = false;
			return;
		}
	}
}

void WorldGenerator::InitDestroy()
{
	initialDestroy = std::rand() % TileMap.size();
	currentDestroy = initialDestroy;
}

bool WorldGenerator::DestroyWorld(int amount)
{
		
	//Call a function in all tiles
	for (int i = 0; i < amount; i++)
	{
		if(TileMap[currentDestroy].agent != nullptr)
			TileMap[currentDestroy].agent->Remove();
		currentDestroy++;
		if (currentDestroy >= TileMap.size() - 1)
			currentDestroy = 0;
		if (initialDestroy == currentDestroy)
		{
			return true;
		}
	}
	return false;
}

void WorldGenerator::RemoveConnections(int X, int Y)
{
	std::vector<int> options;
	std::vector<int> rest;
	//Norte
	if (X + 1 < Side)
	{
		options.clear();
		rest.clear();
		std::set_intersection(std::begin(AllSouth), std::end(AllSouth), TileMap[get_1d(X + 1, Y)].posibilities.begin(), TileMap[get_1d(X + 1, Y)].posibilities.end(), back_inserter(options));

		//We remove those posibilities from the current ones
		std::set_symmetric_difference(TileMap[get_1d(X + 1, Y)].posibilities.begin(), TileMap[get_1d(X + 1, Y)].posibilities.end(),
			options.begin(), options.end(), std::back_inserter(rest));
		TileMap[get_1d(X + 1, Y)].posibilities = rest;
	}
	//Sur
	if (X - 1 >= 0)
	{
		options.clear();
		rest.clear();
		std::set_intersection(std::begin(AllNorth), std::end(AllNorth), TileMap[get_1d(X - 1, Y)].posibilities.begin(), TileMap[get_1d(X - 1, Y)].posibilities.end(), back_inserter(options));

		//We remove those posibilities from the current ones
		std::set_symmetric_difference(TileMap[get_1d(X - 1, Y)].posibilities.begin(), TileMap[get_1d(X - 1, Y)].posibilities.end(),
			options.begin(), options.end(), std::back_inserter(rest));
		TileMap[get_1d(X - 1, Y)].posibilities = rest;
	}
	//Este
	if (X + 1 < Side)
	{
		options.clear();
		rest.clear();
		std::set_intersection(std::begin(AllWest), std::end(AllWest), TileMap[get_1d(X, Y + 1)].posibilities.begin(), TileMap[get_1d(X, Y + 1)].posibilities.end(), back_inserter(options));

		//We remove those posibilities from the current ones
		std::set_symmetric_difference(TileMap[get_1d(X, Y + 1)].posibilities.begin(), TileMap[get_1d(X, Y + 1)].posibilities.end(),
			options.begin(), options.end(), std::back_inserter(rest));
		TileMap[get_1d(X, Y + 1)].posibilities = rest;
	}
	//Oeste
	if (X - 1 >= 0)
	{
		options.clear();
		rest.clear();
		std::set_intersection(std::begin(AllEast), std::end(AllEast), TileMap[get_1d(X, Y - 1)].posibilities.begin(), TileMap[get_1d(X, Y - 1)].posibilities.end(), back_inserter(options));

		//We remove those posibilities from the current ones
		std::set_symmetric_difference(TileMap[get_1d(X, Y - 1)].posibilities.begin(), TileMap[get_1d(X, Y - 1)].posibilities.end(),
			options.begin(), options.end(), std::back_inserter(rest));
		TileMap[get_1d(X, Y - 1)].posibilities = rest;
	}
}

void WorldGenerator::SetRiver(int X, int Y, Block block)
{
	TileMap[get_1d(X, Y)].block = block;
	TileMap[get_1d(X, Y)].posibilities = { (int)block };
	std::vector<int> options;
	//Change pos of adjacent tiles
	switch (block)
	{
	case Block::RIVER_N_S:
		//Conectar con puentes adyacentes
		if (((Y + 1 < Side && TileMap[get_1d(X, Y + 1)].block == Block::BRIDGE_N_S) && (TileMap[get_1d(X, Y - 1)].block == Block::BRIDGE_N_S || std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X, Y - 1)].block) == std::end(AllRivers))) ||
			((Y - 1 >= 0 && TileMap[get_1d(X, Y - 1)].block == Block::BRIDGE_N_S) && (TileMap[get_1d(X, Y + 1)].block == Block::BRIDGE_N_S || std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X, Y + 1)].block) == std::end(AllRivers))))
		{
			SetRiver(X, Y, Block::BRIDGE_N_S);
			return;
		}
		//Si no hay puentes adyacentes quitamos conexiones
		RemoveConnections(X, Y);
		break;
	case Block::RIVER_N_E:
		//Desconectar puentes adyacentes
		//Sur
		if (X-1 >= 0 && TileMap[get_1d(X-1, Y)].block == Block::BRIDGE_E_W)
			SetRiver(X-1, Y, Block::RIVER_E_W);
		//Oeste
		if (Y - 1 >= 0 && TileMap[get_1d(X, Y - 1)].block == Block::BRIDGE_N_S)
			SetRiver(X, Y - 1, Block::RIVER_N_S);
		//Si no hay puentes adyacentes quitamos conexiones
		RemoveConnections(X, Y);
		break;
	case Block::RIVER_N_W:
		//Desconectar puentes adyacentes
		//Sur
		if (X - 1 >= 0 && TileMap[get_1d(X - 1, Y)].block == Block::BRIDGE_E_W)
			SetRiver(X - 1, Y, Block::RIVER_E_W);
		//Este
		if (Y + 1 < Side && TileMap[get_1d(X, Y + 1)].block == Block::BRIDGE_N_S)
			SetRiver(X, Y + 1, Block::RIVER_N_S);
		//Si no hay puentes adyacentes quitamos conexiones
		RemoveConnections(X, Y);
		break;
	case Block::RIVER_S_E:
		//Desconectar puentes adyacentes
		//Norte
		if (X + 1 >= 0 && TileMap[get_1d(X + 1, Y)].block == Block::BRIDGE_E_W)
			SetRiver(X + 1, Y, Block::RIVER_E_W);
		//Oeste
		if (Y - 1 >= 0 && TileMap[get_1d(X, Y - 1)].block == Block::BRIDGE_N_S)
			SetRiver(X, Y - 1, Block::RIVER_N_S);
		//Si no hay puentes adyacentes quitamos conexiones
		RemoveConnections(X, Y);
		break;
	case Block::RIVER_S_W:
		//Desconectar puentes adyacentes
		//Norte
		if (X + 1 >= 0 && TileMap[get_1d(X + 1, Y)].block == Block::BRIDGE_E_W)
			SetRiver(X + 1, Y, Block::RIVER_E_W);
		//Este
		if (Y + 1 < Side && TileMap[get_1d(X, Y + 1)].block == Block::BRIDGE_N_S)
			SetRiver(X, Y + 1, Block::RIVER_N_S);
		//Si no hay puentes adyacentes quitamos conexiones
		RemoveConnections(X, Y);
		break;
	case Block::RIVER_E_W:
		//Conectar con puentes adyacentes
		if (((X + 1 < Side && TileMap[get_1d(X+1, Y)].block == Block::BRIDGE_E_W) && (TileMap[get_1d(X - 1, Y)].block == Block::BRIDGE_E_W || std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X - 1, Y)].block) == std::end(AllRivers))) ||
			((X - 1 >= 0 && TileMap[get_1d(X-1, Y)].block == Block::BRIDGE_E_W) && (TileMap[get_1d(X + 1, Y)].block == Block::BRIDGE_E_W || std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X + 1, Y)].block) == std::end(AllRivers))))
		{
			SetRiver(X, Y, Block::BRIDGE_E_W);
			return;
		}
		//Si no hay puentes adyacentes quitamos conexiones
		RemoveConnections(X, Y);
		break;
	case Block::BRIDGE_N_S:
		//Check for adjacents
		if (TileMap[get_1d(X, Y + 1)].block == Block::RIVER_N_S)	SetRiver(X, Y + 1, Block::BRIDGE_N_S);
		if (TileMap[get_1d(X, Y - 1)].block == Block::RIVER_N_S)	SetRiver(X, Y - 1, Block::BRIDGE_N_S);

		if ((TileMap[get_1d(X, Y + 1)].block != Block::BRIDGE_N_S && std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X, Y + 1)].block) != std::end(AllRivers)) ||
			(TileMap[get_1d(X, Y - 1)].block != Block::BRIDGE_N_S && std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X, Y - 1)].block) != std::end(AllRivers)))
		{
			SetRiver(X, Y, Block::RIVER_N_S);
			return;
		}

		//Este
		if (Y + 1 < Side)
		{
			options.clear();
			std::set_intersection(std::begin(AllWest), std::end(AllWest), TileMap[get_1d(X, Y + 1)].posibilities.begin(), TileMap[get_1d(X, Y + 1)].posibilities.end(), back_inserter(options));

			TileMap[get_1d(X, Y + 1)].posibilities = options;
		}
		//Oeste
		if (Y - 1 >= 0)
		{
			options.clear();
			std::set_intersection(std::begin(AllEast), std::end(AllEast), TileMap[get_1d(X, Y - 1)].posibilities.begin(), TileMap[get_1d(X, Y - 1)].posibilities.end(), back_inserter(options));

			TileMap[get_1d(X, Y - 1)].posibilities = options;
		}
		break;
	case Block::BRIDGE_E_W:
		//Check for adjacents
		if (TileMap[get_1d(X + 1, Y)].block == Block::RIVER_E_W)	SetRiver(X + 1, Y, Block::BRIDGE_E_W);
		if (TileMap[get_1d(X - 1, Y)].block == Block::RIVER_E_W)	SetRiver(X - 1, Y, Block::BRIDGE_E_W);

		if ((TileMap[get_1d(X + 1, Y)].block != Block::BRIDGE_E_W && std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X + 1, Y)].block) != std::end(AllRivers)) ||
			(TileMap[get_1d(X - 1, Y)].block != Block::BRIDGE_E_W && std::find(std::begin(AllRivers), std::end(AllRivers), (int)TileMap[get_1d(X - 1, Y)].block) != std::end(AllRivers)))
		{
			SetRiver(X, Y, Block::RIVER_E_W);
			return;
		}

		//Norte
		if (X + 1 < Side)
		{
			options.clear();
			std::set_intersection(std::begin(AllSouth), std::end(AllSouth), TileMap[get_1d(X + 1, Y)].posibilities.begin(), TileMap[get_1d(X + 1, Y)].posibilities.end(), back_inserter(options));

			TileMap[get_1d(X + 1, Y)].posibilities = options;
		}
		//Sur
		if (X - 1 >= 0)
		{
			options.clear();
			std::set_intersection(std::begin(AllNorth), std::end(AllNorth), TileMap[get_1d(X - 1, Y)].posibilities.begin(), TileMap[get_1d(X - 1, Y)].posibilities.end(), back_inserter(options));

			TileMap[get_1d(X - 1, Y)].posibilities = options;
		}
		break;
	default:
		break;
	}
	master->GenerarActor(TileMap[get_1d(X, Y)].block, X, Y);
}

ABaseBlock* WorldGenerator::GetActor(int X, int Y)
{
	return TileMap[get_1d(X, Y)].agent;
}
