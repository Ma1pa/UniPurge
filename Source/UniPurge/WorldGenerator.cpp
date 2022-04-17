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
		//for (int j = 0; j < 16; j++)
		//	TileMap[i].posibilities.push_back(j+1);
		TileMap[i].block = Block::NOTHING;
		TileMap[i].group = 0;
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

void WorldGenerator::CollapseOptions(int X, int Y, int chosen)
{
	Block selected = StaticCast<Block>(chosen);
	TileMap[get_1d(X, Y)].block = selected;
	//Clean all other possible options and remove such options from the adjacent tiles
	TileMap[get_1d(X, Y)].posibilities = std::vector<int>{ chosen };
	
	//Call to north
	if(X < (Height-1))	RecursivePropagation(X+1, Y, Direction::NORTH, RoadInDirection(selected, Direction::NORTH));
	//Call to south
	if(X > 0)	RecursivePropagation(X-1, Y, Direction::SOUTH, RoadInDirection(selected, Direction::SOUTH));
	//Call to east
	if(Y < (Width-1))	RecursivePropagation(X, Y+1, Direction::EAST, RoadInDirection(selected, Direction::EAST));
	//Call to west
	if(Y > 0)	RecursivePropagation(X, Y-1, Direction::WEST, RoadInDirection(selected, Direction::WEST));

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
		else if (Y < (Width - 1) && RoadInDirection(TileMap[get_1d(X, Y+1)].block, Direction::WEST))	CollapseOptions(X, Y, 4);
		//West
		else if (Y > 0 && RoadInDirection(TileMap[get_1d(X, Y-1)].block, Direction::EAST))	CollapseOptions(X, Y, 5);
	}
	else
		CollapseOptions(X, Y, TileMap[get_1d(X, Y)].posibilities[chosen]); 
}

void WorldGenerator::RecursivePropagation(int X, int Y, Direction MovementFromRemoved, bool collides)
{
	if (TileMap[get_1d(X, Y)].block != Block::NOTHING)	return;
	std::vector<int> options;
	
	//if (TileMap[get_1d(X, Y)].posibilities.size() <= 0)
	//{
	//	TileMap[get_1d(X,Y)].posibilities = { 17 };
	//	return;
	//}
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
	//Technically no more than 1 lvl of recursion required with the roads
	Block chosen = TileMap[get_1d(X, Y)].block;
	if (chosen != Block::NOTHING)
	{
		//Call to north
		if (X < Height)	RecursivePropagation(X + 1, Y, Direction::SOUTH, RoadInDirection(chosen, Direction::NORTH));
		//Call to south
		if (X > 0)	RecursivePropagation(X - 1, Y, Direction::NORTH, RoadInDirection(chosen, Direction::SOUTH));
		//Call to east
		if (Y < Width)	RecursivePropagation(X, Y + 1, Direction::WEST, RoadInDirection(chosen, Direction::EAST));
		//Call to west
		if (Y > 0)	RecursivePropagation(X, Y - 1, Direction::EAST, RoadInDirection(chosen, Direction::WEST));
	}
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
