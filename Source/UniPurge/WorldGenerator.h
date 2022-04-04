// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UniPurge/Enums.h"
#include <vector>
#include <stdexcept>
#include <map>

/* Class used to generate the world in which the game takes place */
class UNIPURGE_API WorldGenerator
{
public:
	
	WorldGenerator(int S, int H, int W, int MR);
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GenerationOptions)
		/* Maximum influence range */
		int MaxRange;


	/* Returns the most influenced point in the grid in coordinates || 
	* REMEMBER TO REMOVE POINTER WITH (delete[] pointer) */
	int* GetMostInfluenced();

	/**
	* Function called whenever an element is added to the grid. Used to recalculate the map
	* @param X : The X position in the grid ->
	* @param Y : The Y position in the grid ^
	*/
	void AddItem(int X, int Y, Block UsedTile);

protected:
	/* Tile map used to guide generation */
	std::vector<Tile> TileMap;

private:
	/* Priority List used to check for max influence*/
	//std::priority_queue<Tile*, std::vector<Tile*>, CustomCompare> InfluenceList;
	std::multimap <int, Tile*> InfluenceList;

	/**
	* Function used to recalculate the influence in a certain road
	* @param Pos : The position of the road in the grid
	*/
	void RecalculateRoad(int Pos);

	/**
	* Recursive function called by "RecalculateRoad" and used to recalculate the influence in a certain nothing spot
	* @param Pos : The position of the element in the grid
	* @param InfluenceGain : The gain of influence to the element
	* @param RemainingRange : The remaining range for the recursion
	*/
	void RecalculateNext(int Pos, int InfluenceGain, int RemainingRange);

	/**
	* Function used to update the influence in a certain road
	* @param Pos : The position of the road in the grid
	* @param InfluenceGain : The influence that the road gains
	*/
	void UpdateRoad(int Pos, int InfluenceGain);

	/**
	* Recursive function called by "UpdateRoad" and used to recalculate the influence in a certain nothing spot
	* @param Pos : The position of the road in the grid
	* @param InfluenceGain : The gain of influence to the road
	* @param RemainingRange : The remaining range for the recursion
	*/
	void UpdateNext(int Pos, int InfluenceGain, int RemainingRange);

	/**
	* Recursive function called by "AddItem" and used to find all roads inside its influence area
	* @param Pos : The position of the road in the grid
	* @param RemainingInfluence : The remaining influence of the building
	* @param RemainingRange : The remaining range for the recursion
	*/
	void CheckRoads(int Pos, int RemainingInfluence, int RemainingRange);

	/**
	* Function used to attain the Array position of a 2D position
	* @param Columna : The X Position ->
	* @param Fila : The Y Position ^
	*/
	Tile* get_1d(int Columna, int Fila);
	/**
	* Function used to attain the 2D position from the Array || 
	* REMEMBER TO REMOVE POINTER WITH (delete[] pointer)
	* @param Point : The position in the array
	*/
	int* get_2d(Tile* Point);

	/**
	* Function used to replace the key of a certain tile in the multimap
	* @param list : The list in which to change the keys
	* @param oldKey : Original key of the pair
	* @param newKey : New key of the pair
	* @param tile : Tile which we are changing from
	*/
	void replace_key(std::multimap <int, Tile*> list, int oldKey, int newKey, Tile* tile);
};




