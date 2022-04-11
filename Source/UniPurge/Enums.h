// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <array>
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class Block : uint8
{
	EMPTY UMETA(DisplayName = "Empty"),
	ROAD_N UMETA(DisplayName = "Road North"),
	ROAD_S UMETA(DisplayName = "Road South"),
	ROAD_E UMETA(DisplayName = "Road East"),
	ROAD_W UMETA(DisplayName = "Road West"),
	ROAD_N_S UMETA(DisplayName = "Road North-South"),
	ROAD_N_E UMETA(DisplayName = "Road North-East"),
	ROAD_N_W UMETA(DisplayName = "Road North-West"),
	ROAD_S_E UMETA(DisplayName = "Road South-East"),
	ROAD_S_W UMETA(DisplayName = "Road South-West"),
	ROAD_E_W UMETA(DisplayName = "Road East-West"),
	ROAD_N_S_E UMETA(DisplayName = "Road North-South-East"),
	ROAD_N_S_W UMETA(DisplayName = "Road North-South-West"),
	ROAD_N_E_W UMETA(DisplayName = "Road North-East-West"),
	ROAD_S_E_W UMETA(DisplayName = "Road South-East-West"),
	ROAD_N_S_E_W  UMETA(DisplayName = "Road Cross"),
	BUILDING UMETA(DisplayName = "Building")
};

//Basic exits arrays
static std::array<Block, 8> NorthExits = { Block::ROAD_N, Block::ROAD_N_E, Block::ROAD_N_S, Block::ROAD_N_W, Block::ROAD_N_S_E, Block::ROAD_N_S_W, Block::ROAD_N_E_W, Block::ROAD_N_S_E_W };
// 1, 5, 6, 7, 11, 12, 13, 15
static std::array<Block, 8> SouthExits = { Block::ROAD_S, Block::ROAD_S_E, Block::ROAD_N_S, Block::ROAD_S_W, Block::ROAD_N_S_E, Block::ROAD_N_S_W, Block::ROAD_S_E_W, Block::ROAD_N_S_E_W };
// 2, 5, 8, 9, 11, 12, 14, 15
static std::array<Block, 8> EastExits = { Block::ROAD_E, Block::ROAD_N_E, Block::ROAD_S_E, Block::ROAD_E_W, Block::ROAD_N_S_E, Block::ROAD_S_E_W, Block::ROAD_N_E_W, Block::ROAD_N_S_E_W };
// 3, 6, 8, 10, 11, 13, 14, 15
static std::array<Block, 8> WestExits = { Block::ROAD_W, Block::ROAD_N_W, Block::ROAD_S_W, Block::ROAD_E_W, Block::ROAD_N_S_W, Block::ROAD_S_E_W, Block::ROAD_N_E_W, Block::ROAD_N_S_E_W };
// 4, 7, 9, 10, 12, 13, 14, 15

struct Tile
{
	/* Location of the tile in the array representing the 2D grid*/
	int Location;
	/* Influence of the tile in question*/
	int Influence;
	/* Block currently build in the location*/
	Block block;
};

class UNIPURGE_API Enums
{
public:
	Enums();
	~Enums();
};
