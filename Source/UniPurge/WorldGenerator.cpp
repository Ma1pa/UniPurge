// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(){};
WorldGenerator::WorldGenerator(int S, int H, int W)
{
	//TODO Obtener todos los bojetos que se pueden generar

	Height = H;
	Width = W;
	Seed = S;
	for (int i = 0; i < Width * Height; i++) 
	{
		TileMap.push_back(Tile());
		TileMap[i].Location = i;
		TileMap[i].posibilities.assign(AllRoads, AllRoads + 12);
		//TileMap[i].posibilities.assign(AllSimpleRoads, AllSimpleRoads + 7);
		//for (int j = 0; j < 16; j++)
		//	TileMap[i].posibilities.push_back(j+1);
		TileMap[i].block = Block::NOTHING;
		TileMap[i].group = -1;
		TileMap[i].height = 0;
	}
	
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
		//The position selected has to be Nothing and have the minimal size
		if ((TileMap[i].block == Block::NOTHING &&
			TileMap[i].posibilities.size() < TileMap[bestPosition].posibilities.size()) || 
			TileMap[bestPosition].block != Block::NOTHING)
			bestPosition = i;
	}
	return get_2d(bestPosition);
}

void WorldGenerator::CollapseList(int X, int Y, int chosen)
{
	if (chosen == -1)
	{
		//There are no options left; so we search for any of the no exit routes
		//North
		if (X < (Height - 1) && RoadInDirection(TileMap[get_1d(X + 1, Y)].block, Direction::SOUTH))	CollapseOptions(X, Y, 2);
		//South
		else if (X > 0 && RoadInDirection(TileMap[get_1d(X - 1, Y)].block, Direction::NORTH))	CollapseOptions(X, Y, 3);
		//East
		else if (Y < (Width - 1) && RoadInDirection(TileMap[get_1d(X, Y + 1)].block, Direction::WEST))	CollapseOptions(X, Y, 4);
		//West
		else if (Y > 0 && RoadInDirection(TileMap[get_1d(X, Y - 1)].block, Direction::EAST))	CollapseOptions(X, Y, 5);
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
	if (X < 0 || Y < 0 || X >= Width || Y >= Height)	return 0;
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
	if (X < (Height - 1) && RoadInDirection(TileMap[get_1d(X + 1, Y)].block, Direction::SOUTH))	return true;
	if (X > 0 &&			RoadInDirection(TileMap[get_1d(X-1, Y)].block, Direction::NORTH))	return true;
	if (Y < (Width - 1) &&	RoadInDirection(TileMap[get_1d(X, Y+1)].block, Direction::WEST))	return true;
	if (Y > 0 &&			RoadInDirection(TileMap[get_1d(X, Y-1)].block, Direction::EAST))	return true;
	return false;
}

void WorldGenerator::CallSides(int X, int Y, int block)
{
	//Call to north
	if (X < (Height - 1))	RecursivePropagation(X + 1, Y, Direction::NORTH, RoadInDirection(StaticCast<Block>(block), Direction::NORTH), block == 1);
	//Call to south
	if (X > 0)	RecursivePropagation(X - 1, Y, Direction::SOUTH, RoadInDirection(StaticCast<Block>(block), Direction::SOUTH), block == 1);
	//Call to east
	if (Y < (Width - 1))	RecursivePropagation(X, Y + 1, Direction::EAST, RoadInDirection(StaticCast<Block>(block), Direction::EAST), block == 1);
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

int WorldGenerator::get_1d(int Columna, int Fila)	{ return Fila + Width * Columna ; }

std::pair<int, int> WorldGenerator::get_2d( int Pos){ return { (int)(Pos / Width), Pos % Width}; }

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

void WorldGenerator::AddAgent(int X, int Y, ABaseBlock* bloque)
{
	TileMap[get_1d(X, Y)].agent = bloque;
}
