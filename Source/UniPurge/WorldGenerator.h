// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UniPurge/Enums.h"
#include <vector>
#include <iterator>
#include <algorithm>

/* Class used to generate the world in which the game takes place */
class UNIPURGE_API WorldGenerator
{
public:
	
	WorldGenerator(int S, int H, int W);
	WorldGenerator();
	~WorldGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GenerationOptions)
		/* Height of the world generated (2D) */
		int Height;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GenerationOptions)
		/* Width of the world generated (2D) */
		int Width;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GenerationOptions)
		/* Seed to use in the random generation */
		int Seed;

	/* Function called to recieve the tile with the least entropy
	* @return Int pair indicating the 2D position of the tile in the graph
	*/
	std::pair<int, int> GetLessEntropy();

	/* Funtion called to recieve the number of the tile in the 2D position
	* @param X : The X position in the array
	* @param Y : The Y position in the array
	*/
	Block GetBlock(int X, int Y);

	/* Function called to know if an empty can be placed in the 2D position
	* @param X : The X position in the array
	* @param Y : The Y position in the array
	*/
	bool CanEmpty(int X, int Y);

	/* Function called to get the length of the vector with all the posibilities
	* @param X : The X position in the array
	* @param Y : The Y position in the array
	*/
	int GetPosibilities(int X, int Y);

	/* Function called whenever a block is chosen. It collapses other options in the tile and simplifies the others
	* @param X : The X position in the array where the block will be placed
	* @param Y : The Y position in the array where the block will be placed
	* @param chosen : The chosen block for the selected position
	*/
	void CollapseOptions(int X, int Y, int chosen);

	/* Function called whenever a block is chosen. It collapses other options in the tile and simplifies the others
	* @param X : The X position in the array where the block will be placed
	* @param Y : The Y position in the array where the block will be placed
	* @param chosen : The chosen block for the selected position
	*/
	void CollapseList(int X, int Y, int chosen);

	std::vector<int> GetOptions(int X, int Y);

	int CompareGroup(int X, int Y, int group);

	int GetGroup(int X, int Y);

	int GetHeight(int X, int Y);

	void CreateHoses(int X, int Y, int group, int h);

protected:
	/* Tile map used to guide generation */
	std::vector<Tile> TileMap;

	/* Function called recursively or by "CollapseOptions". It is used to remove all the adjacent blocks to the removed block
	* @param X : The X position in the array where the options will be collapsed
	* @param Y : The Y position in the array where the options will be collapsed
	* @param MovementFromRemoved : The direction from the previous tile where the funciton was called
	* @param collides : If the road from Removed collides with the tile
	*/
	void RecursivePropagation(int X, int Y, Direction MovementFromRemoved, bool collides, bool isEmpty);

	bool CheckSpots(int X, int Y);

	void CallSides(int X, int Y, int block);

private:

	bool RoadInDirection(Block road, Direction direction);

	/**
	* Function used to attain the Array position of a 2D position
	* @param Columna : The X Position ->
	* @param Fila : The Y Position ^
	* @return Number indicating the location in the list
	*/
	int get_1d(int Columna, int Fila);
	/**
	* Function used to attain the 2D position from the Array
	* @param Point : The position in the array
	* @return Pair indicating the X and Y positions of the location
	*/
	std::pair<int,int> get_2d(int Pos);
};




