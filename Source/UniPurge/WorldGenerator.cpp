// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(){};
WorldGenerator::WorldGenerator( int H, int W, int MR)
{
	Height = H;
	Width = W;
	MaxRange = MR;
	BuildingInfluence = 1;
	for (int i = 0; i < Width * Height; i++) 
	{
		TileMap.push_back(Tile());
		TileMap[i].Location = i;
		TileMap[i].block = Block::EMPTY;
		TileMap[i].Influence = 0;
	}
	//Add point in the middle to the influence list to check at the beginning
	InfluenceList.insert({ TileMap[(Height * Width) / 2].Influence, &TileMap[(Height * Width) / 2] });

	iterator = 0;
	
}

WorldGenerator::~WorldGenerator()
{
}

std::pair<int, int> WorldGenerator::GetMostInfluenced()
{
	UE_LOG(LogTemp, Warning, TEXT("Iteración numero %d"), ++iterator);
	//Check if list is empty
	if (InfluenceList.empty())
	{
		//If it is empty, we then check all tiles up until an Empty tile shows up
		FixList();
		if (InfluenceList.empty()) 
		{
			//If it passes the for, then there is nothing else to place
			return std::pair<int, int>{-1, -1};
		}
	}
	//Remove the selected tile from the list and return the position
	Tile* SelectedTile = prev(InfluenceList.end())->second;
	std::pair<int, int> valor = get_2d(SelectedTile->Location);
	InfluenceList.erase(prev(InfluenceList.end()));
	return valor;
}

void WorldGenerator::FixList()
{
	for (int i = 0; i < TileMap.size(); i++)
	{
		if (TileMap[i].block == Block::EMPTY) 
		{
			InfluenceList.insert({ TileMap[i].Influence, &TileMap[i] });
			return;
		}
	}
}

void WorldGenerator::AddItem(int X, int Y, Block UsedTile)
{
	TileMap[get_1d(X, Y)->Location].block = UsedTile;
	//Check what are we placing
	if (UsedTile == Block::EMPTY) 
	{
		//We aren't placing anything, error
		throw std::invalid_argument("You attempted to place nothing");
		return;
	}
	//It is road
	else if(UsedTile <= Block::ROAD_N_S_E_W)
	{
		//Vector with variable length used to know whether a point has been visited
		std::vector<int> visited;
		//We recalculate all the influence in empty spots with the influence the road has
		RecalculateRoad(get_1d(X, Y)->Location, 0, &visited);
	}
	else 
	{
		//Vector with variable length used to know whether a point has been visited
		std::vector<int> visited;
		//We recalculate all influence in all the close roads
		CheckRoads(get_1d(X, Y)->Location, BuildingInfluence, MaxRange,  &visited);
	}
}


void WorldGenerator::RecalculateRoad(int Pos, int InfluenceGain, std::vector<int>* VisitedList)
{
	//Add the current point in the VisitedList
	VisitedList->push_back(Pos);

	TileMap[Pos].Influence += InfluenceGain;
	int Influence = (InfluenceGain == 0)? TileMap[Pos].Influence : InfluenceGain;
	////If we're building a road and the road influence is 0; set the influence to 1
	//if (Influence == 0) 
	//{
	//	Influence = 1;
	//	TileMap[Pos].Influence = 1;
	//}
	//We call recursibly to all the sides, the recursive function will then take all options by itself
	if ((Pos + 1) % Width != 0)
		RecalculateNext(Pos + 1, Influence, MaxRange, VisitedList);
	if (Pos % Width != 0)
		RecalculateNext(Pos - 1, Influence, MaxRange, VisitedList);
	if (Pos + Width < TileMap.size())
		RecalculateNext(Pos + Width, Influence, MaxRange, VisitedList);
	if (Pos - Width >= 0)
		RecalculateNext(Pos - Width, Influence, MaxRange, VisitedList);

}

void WorldGenerator::RecalculateNext(int Pos, int InfluenceGain, int RemainingRange, std::vector<int>* VisitedList)
{
	//Check if we already visited the position
	if (std::count(VisitedList->begin(), VisitedList->end(), Pos) > 0)		return;

	VisitedList->push_back(Pos);

	//If no influence is left to distribute or no more range, stop, since it is the end
	if (InfluenceGain <= 0 || RemainingRange <= 0)		return;

	//Check if the block is not already built
	if (TileMap[Pos].block == Block::EMPTY)
	{
		TileMap[Pos].Influence += InfluenceGain;
		//If influence was 0, we add the block into the list; else we replace the influence of the block
		if (TileMap[Pos].Influence == InfluenceGain)
			InfluenceList.insert({ InfluenceGain, &TileMap[Pos] });
		else
			replace_key(InfluenceList, TileMap[Pos].Influence - InfluenceGain, TileMap[Pos].Influence, &TileMap[Pos]);
	}
	
	if ((Pos + 1) % Width != 0)
		RecalculateNext(Pos + 1, InfluenceGain - 1, RemainingRange - 1, VisitedList);
	if (Pos % Width != 0)
		RecalculateNext(Pos - 1, InfluenceGain - 1, RemainingRange - 1, VisitedList);
	if (Pos + Width < TileMap.size())
		RecalculateNext(Pos + Width, InfluenceGain - 1, RemainingRange - 1, VisitedList);
	if (Pos - Width >= 0)
		RecalculateNext(Pos - Width, InfluenceGain - 1, RemainingRange - 1, VisitedList);
}

void WorldGenerator::CheckRoads(int Pos, int RemainingInfluence, int RemainingRange, std::vector<int>* VisitedList)
{
	//Check if we visited already the position
	if (std::count(VisitedList->begin(), VisitedList->end(), Pos))
		return;
	VisitedList->push_back(Pos);

	//If no influence is left to distribute or no more range, stop, since it is the end
	if (RemainingInfluence <= 0 || RemainingRange <= 0)		return;

	//Check if the block is a road
	if (TileMap[Pos].block != Block::EMPTY && TileMap[Pos].block <= Block::ROAD_N_S_E_W)
	{
		std::vector<int> visited;
		RecalculateRoad(Pos, RemainingInfluence, &visited);
	}
	//TODO Maybe remove the loss of ingfluence by distance here?
	if ((Pos + 1) % Width != 0)
		CheckRoads(Pos + 1, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
	if (Pos % Width != 0)
		CheckRoads(Pos - 1, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
	if (Pos + Width < TileMap.size())
		CheckRoads(Pos + Width, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
	if (Pos - Width >= 0)
		CheckRoads(Pos - Width, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
}

int WorldGenerator::GetInfluence(int X, int Y) 
{
	return get_1d(X, Y)->Influence;
}

Block WorldGenerator::GetBlock(int X, int Y)
{
	if (X < 0 || Y < 0 || (X + Width * Y) >= (Width * Height))	return Block::EMPTY;
	return get_1d(X, Y)->block;
}

Tile* WorldGenerator::get_1d(int Columna, int Fila)	{ return & TileMap[Fila + Width * Columna] ; }

std::pair<int, int> WorldGenerator::get_2d (int Pos){ return { (int)(Pos / Width), Pos % Width}; }

void WorldGenerator::replace_key(std::multimap <int, Tile*> list, int oldKey, int newKey, Tile* tile)
{
	Tile* punto;
	switch(list.count(oldKey))
	{
	case 0:
		//No element as such exists in the list
		return;
		break;
	case 1:
		//Only one element exists in the list
		punto = list.find(oldKey)->second;
		list.erase(list.find(oldKey));
		list.insert({ newKey, punto });
		break;
	default:
		//More than one element exists in the list
		//We do a binary search looking for the correct item
		auto itr1 = list.lower_bound(oldKey);
		auto itr2 = list.upper_bound(oldKey);
		while (itr1 != itr2)
		{
			if (itr1->first == oldKey && itr1->second == tile)
			{
				punto = itr1->second;
				list.erase(itr1);
				list.insert({ newKey, punto });
				return;
			}
			itr1++;
		}
		break;
	}
	return;
}