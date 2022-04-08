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
	for (int i = 0; i < Width * Height; i++) 
	{
		TileMap.push_back(Tile());
		TileMap[i].Location = i;
	}
		
	
	//Fixing wrong location value
	//for (int i = 0; i < TileMap.size(); i++) { TileMap[i].Location = i; }
	//Add point 0,0 to the influence list to check at the beginning
	InfluenceList.insert({ TileMap[0].Influence, &TileMap[0] });

	iterator = 0;
	
}

WorldGenerator::~WorldGenerator()
{
}

std::pair<int, int> WorldGenerator::GetMostInfluenced()
{
	UE_LOG(LogTemp, Warning, TEXT("Iteración numero %d"), ++iterator);
	//Check if list is empty
	if(InfluenceList.empty())
	{
		//If it is empty, we then check all tiles up until an Empty tile shows up
		FixList();
		//If it passes the for, then there is nothing else to place
		return std::pair<int, int>{-1, -1};
	}
	//Check if the block has something, If it does, remove it from the dic and check the next
	Tile* SelectedTile = prev(InfluenceList.end())->second;
	//while(SelectedTile->block != Block::EMPTY)
	//{
	//	InfluenceList.erase(prev(InfluenceList.end()));
	//	if (InfluenceList.empty())
	//		FixList();
	//	SelectedTile = prev(InfluenceList.end())->second;
	//}
	std::pair<int, int> valor = get_2d(SelectedTile->Location);
	UE_LOG(LogTemp, Warning, TEXT("1D location = %d"), SelectedTile->Location);
	InfluenceList.erase(prev(InfluenceList.end()));
	return valor;
}

void WorldGenerator::FixList()
{
	for (int i = 0; i < TileMap.size(); i++)
	{
		if (TileMap[i].block == Block::EMPTY)
			InfluenceList.insert({ TileMap[i].Influence, &TileMap[i] });
		break;
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
	else if(UsedTile <= Block::ROAD_N_S_E_W)
	{
		//Vector with variable length used to know whether a point has been visited
		std::vector<int> visited;
		//It is road
		//We recalculate all the influence in empty spots with the influence the road has
		RecalculateRoad(get_1d(X, Y)->Location, &visited);
	}
	else 
	{
		//Vector with variable length used to know whether a point has been visited
		std::vector<int> visited;
		//We recalculate all influence in all the close roads
		CheckRoads(get_1d(X, Y)->Location, BuildingInfluence, MaxRange,  &visited);
	}
	
	//TODO Recalculate all influence from Roads
}


void WorldGenerator::RecalculateRoad(int Pos, std::vector<int>* VisitedList)
{
	//Limpiamos la lista
	VisitedList->clear();
	VisitedList->push_back(Pos);

	int Influence = TileMap[Pos].Influence;
	TileMap[Pos].Influence = 0;
	//We call recursibly to all the sides, the recursive function will then take all options by itself
	if (Pos + 1 % Width != 0)
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
	//Check if we visited already the position
	if (std::count(VisitedList->begin(), VisitedList->end(), Pos))
		return;
	VisitedList->push_back(Pos);
	//Check if the block is not already built
	if (TileMap[Pos].block == Block::EMPTY)
	{
		TileMap[Pos].Influence += InfluenceGain;
		if (TileMap[Pos].Influence == InfluenceGain)
			InfluenceList.insert({ InfluenceGain, &TileMap[Pos] });
		else
			replace_key(InfluenceList, TileMap[Pos].Influence - InfluenceGain, TileMap[Pos].Influence, &TileMap[Pos]);
	}
		
	if(RemainingRange > 0 && InfluenceGain > 0)
	{
		if(Pos+1 % Width != 0)
			RecalculateNext(Pos + 1, InfluenceGain - 1, RemainingRange - 1, VisitedList);
		if(Pos % Width != 0)
			RecalculateNext(Pos - 1, InfluenceGain - 1, RemainingRange - 1, VisitedList);
		if(Pos + Width < TileMap.size())
			RecalculateNext(Pos + Width, InfluenceGain - 1, RemainingRange - 1, VisitedList);
		if(Pos - Width >= 0)
			RecalculateNext(Pos - Width, InfluenceGain - 1, RemainingRange - 1, VisitedList);
	}
}

void WorldGenerator::UpdateRoad(int Pos, int InfluenceGain, std::vector<int>* VisitedList)
{
	//Limpiamos la lista
	VisitedList->clear();
	VisitedList->push_back(Pos);

	TileMap[Pos].Influence += InfluenceGain;
	//We call recursibly to all the sides, the recursive function will then take all options by itself
	if (Pos + 1 % Width != 0)
		UpdateNext(Pos + 1, InfluenceGain, MaxRange, VisitedList);
	if (Pos % Width != 0)
		UpdateNext(Pos - 1, InfluenceGain, MaxRange, VisitedList);
	if (Pos + Width < TileMap.size())
		UpdateNext(Pos + Width, InfluenceGain, MaxRange, VisitedList);
	if (Pos - Width >= 0)
		UpdateNext(Pos - Width, InfluenceGain, MaxRange, VisitedList);

}

void WorldGenerator::UpdateNext(int Pos, int InfluenceGain, int RemainingRange, std::vector<int>* VisitedList)
{
	//Check if we visited already the position
	if (std::count(VisitedList->begin(), VisitedList->end(), Pos))
		return;
	VisitedList->push_back(Pos);

	//Check if the block is not already built
	if (TileMap[Pos].block == Block::EMPTY)
	{
		TileMap[Pos].Influence += InfluenceGain;
		if (TileMap[Pos].Influence == InfluenceGain)
			InfluenceList.insert({ InfluenceGain, &TileMap[Pos] });
		else
			replace_key(InfluenceList, TileMap[Pos].Influence - InfluenceGain, TileMap[Pos].Influence, &TileMap[Pos]);
	}

	//Make the recursion stop if the Max Range is reached or if the tile had 0 influence
	if (RemainingRange > 0 && TileMap[Pos].Influence > InfluenceGain)
	{
		if (Pos + 1 % Width != 0)
			UpdateNext(Pos + 1, InfluenceGain, RemainingRange - 1, VisitedList);
		if (Pos % Width != 0)
			UpdateNext(Pos - 1, InfluenceGain, RemainingRange - 1, VisitedList);
		if (Pos + Width < TileMap.size())
			UpdateNext(Pos + Width, InfluenceGain, RemainingRange - 1, VisitedList);
		if (Pos - Width >= 0)
			UpdateNext(Pos - Width, InfluenceGain, RemainingRange - 1, VisitedList);
	}
}

void WorldGenerator::CheckRoads(int Pos, int RemainingInfluence, int RemainingRange, std::vector<int>* VisitedList)
{
	//Check if we visited already the position
	if (std::count(VisitedList->begin(), VisitedList->end(), Pos))
		return;
	VisitedList->push_back(Pos);

	//Check if the block is a road
	if (TileMap[Pos].block != Block::EMPTY && TileMap[Pos].block <= Block::ROAD_N_S_E_W)
	{
		UpdateRoad(Pos, RemainingInfluence, VisitedList);
	}
	//Make the recursion stop if the Max Range is reached or if the tile had 0 influence
	if (RemainingRange > 0 && RemainingInfluence > 1)
	{
		if (Pos + 1 % Width != 0)
			CheckRoads(Pos + 1, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
		if (Pos % Width != 0)
			CheckRoads(Pos - 1, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
		if (Pos + Width < TileMap.size())
			CheckRoads(Pos + Width, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
		if (Pos - Width >= 0)
			CheckRoads(Pos - Width, RemainingInfluence - 1, RemainingRange - 1, VisitedList);
	}
}

Tile* WorldGenerator::get_1d(int Columna, int Fila)	{ return & TileMap[Fila + Width * Columna] ; }

std::pair<int, int> WorldGenerator::get_2d( int Pos){ return { (int)(Pos / Width), Pos % Width}; }

void WorldGenerator::replace_key(std::multimap <int, Tile*> list, int oldKey, int newKey, Tile* tile)
{
	Tile* punto;
	switch(list.count(oldKey))
	{
	case 0:
		return;
		break;
	case 1:
		punto = list.find(oldKey)->second;
		list.erase(list.find(oldKey));
		list.insert({ newKey, punto });
		break;
	default:
		//Hay mas de 1 claves iguales
		//Busqueda binaria para encontrar elelemento igual
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
}