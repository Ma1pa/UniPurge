// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
