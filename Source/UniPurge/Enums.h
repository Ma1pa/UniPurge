// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include <random>

UENUM(BlueprintType)
enum class Block : uint8
{
	NOTHING UMETA(DisplayName = "Nothing"),							//0
	EMPTY UMETA(DisplayName = "Empty"),								//1
	ROAD_N UMETA(DisplayName = "Road North"),						//2
	ROAD_S UMETA(DisplayName = "Road South"),						//3
	ROAD_E UMETA(DisplayName = "Road East"),						//4
	ROAD_W UMETA(DisplayName = "Road West"),						//5
	ROAD_N_S UMETA(DisplayName = "Road North-South"),				//6
	ROAD_N_E UMETA(DisplayName = "Road North-East"),				//7
	ROAD_N_W UMETA(DisplayName = "Road North-West"),				//8
	ROAD_S_E UMETA(DisplayName = "Road South-East"),				//9
	ROAD_S_W UMETA(DisplayName = "Road South-West"),				//10
	ROAD_E_W UMETA(DisplayName = "Road East-West"),					//11
	ROAD_N_S_E UMETA(DisplayName = "Road North-South-East"),		//12
	ROAD_N_S_W UMETA(DisplayName = "Road North-South-West"),		//13
	ROAD_N_E_W UMETA(DisplayName = "Road North-East-West"),			//14
	ROAD_S_E_W UMETA(DisplayName = "Road South-East-West"),			//15
	ROAD_N_S_E_W  UMETA(DisplayName = "Road Cross"),				//16
	BUILDING UMETA(DisplayName = "Building"),						//17
	PARK UMETA(DisplayName = "Park")								//18
};

UENUM(BlueprintType)
enum class Direction : uint8
{
	NORTH UMETA(DisplayName = "North"),
	SOUTH UMETA(DisplayName = "South"),
	EAST UMETA(DisplayName = "East"),
	WEST UMETA(DisplayName = "West")
};

enum class Connections : uint8
{
	DISCONNECTED,	//Connections with unrelated blocks; like rivers or other groups
	EXIT,			//Connections that can have an exit; like a road or park
	SAMEGROUP,		//Connections between the same house groups
	DIFFERENTGROUP,	//Connections to another building without any connection
	NOTHING
};

static int AllBlocks[] {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
static int AllRoads[]{ 1,6,7,8,9,10,11,12,13,14,15,16 };
static int AllSimpleRoads[]{ 1,6,7,8,9,10,11 };
static int AllNorth[7]{ 6,7,8,12,13,14,16 };
static int AllSouth[7]{ 6,9,10,12,13,15,16 };
static int AllEast[7]{ 7,9,11,12,14,15,16 };
static int AllWest[7]{ 8,10,11,13,14,15,16 };

static const int GridToCoordMult = 800;

static std::default_random_engine generator;

class UNIPURGE_API Enums
{
public:
	Enums();
	~Enums();
};
