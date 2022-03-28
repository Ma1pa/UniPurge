// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(){};
WorldGenerator::WorldGenerator(int S, int H, int W, int MR)
{
	//TODO Obtener todos los bojetos que se pueden generar

	Height = H;
	Width = W;
	Seed = S;
	MaxRange = MR;

	InfluenceMap [Height * Width] = {};
}

WorldGenerator::~WorldGenerator()
{
}

int* WorldGenerator::GetMostInfluenced()
{
	//TODO Return the X, Y position of the most influential zone
	return nullptr;
}

void WorldGenerator::AddBuilding(int X, int Y)
{
	//TODO Add influence to the indicated position and all positions around up until MaxRange
	//TODO Recalculate all influence from Roads
}

void WorldGenerator::AddRoad(int X, int Y)
{
	//TODO Add the point into a "bag" of roads to check later
	//TODO Add influence to the indicated position and all positions around up until MaxRange
}

int WorldGenerator::get_1d(int Columna, int Fila)	{ return InfluenceMap[Columna + Width * Fila]; }
int* WorldGenerator::get_2d( int Point)				{ return new int[2]{ Point % Width, Point / Width}; }
