// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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


	/* Returns the most influenced point in the grid in coordinates */
	int* GetMostInfluenced();

	/**
	* Function called whenever a building is added to the grid. Used to recalculate the map
	* @param X : The X position in the grid ->
	* @param Y : The Y position in the grid ^
	*/
	void AddBuilding(int X, int Y);

	/**
	* Function called whenever a road is added to the grid. Used to recalculate the map
	* @param X : The X position in the grid ->
	* @param Y : The Y position in the grid ^
	*/
	void AddRoad(int X, int Y);

protected:
	/* Influence map used to guide generation */
	int InfluenceMap [];

private:
	/**
	* Function used to attain the Array position of a 2D position
	* @param Columna : The X Position ->
	* @param Fila : The Y Position ^
	*/
	int get_1d(int Columna, int Fila);
	/**
	* Function used to attain the 2D position from the Array || 
	* REMEMBER TO REMOVE POINTER WITH (delete[] pointer)
	* @param Point : The position in the array
	*/
	int* get_2d(int Point);
};


