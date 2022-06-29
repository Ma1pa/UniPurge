// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"


// Sets default values
ABaseBlock::ABaseBlock()
{
	blockUp = nullptr;
	currentBlock = Block::NOTHING;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	HeightFromThis = 0;
	placedCenter = false;
	for (int i = 0; i < 5; i++)
	{
		StaticElements.Push(CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::FromInt(i))));
		StaticElements[i]->LDMaxDrawDistance = 32000.0f;
		StaticElements[i]->SetupAttachment(StaticMesh);
		//StaticElements[i]->AttachToComponent(StaticMesh,FAttachmentTransformRules::KeepWorldTransform);
	}
	RootComponent = StaticMesh;
	floored = false;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
void ABaseBlock::SetRenderDistance(int distance)
{
	StaticMesh->LDMaxDrawDistance = StaticCast<float>(distance);
	for (int i = 0; i < StaticElements.Num(); i++)
	{
		StaticElements[i]->LDMaxDrawDistance = StaticCast<float>(distance);
	}
}

// Called when the game starts or when spawned
void ABaseBlock::BeginPlay()
{

	Super::BeginPlay();
	
}

// Called every frame
void ABaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBlock::SetStats(Block block, int height, UStaticMesh* newMesh, TArray<UStaticMesh*> Elements1, TArray<UStaticMesh*> Elements2, TArray<UStaticMesh*> Elements3, TArray<UStaticMesh*> Elements4, TArray<UStaticMesh*> Elements5, TArray<UStaticMesh*> Elements6)
{
	MainMesh = newMesh;
	Addons_1 = Elements1;
	Addons_2 = Elements2;
	Addons_3 = Elements3;
	Addons_4 = Elements4;
	Addons_5 = Elements5;
	Addons_6 = Elements6;
	currentBlock = block;

	StaticMesh->SetStaticMesh(newMesh);
	GetMesh(currentBlock);

	HeightFromThis = height;
	UE_LOG(LogTemp, Warning, TEXT("Height = %d"), height);
	if (HeightFromThis > 0 && currentBlock == Block::BUILDING)
	{
		//We create a new block on top
		const FVector Location = { AActor::GetActorLocation().X, AActor::GetActorLocation().Y, AActor::GetActorLocation().Z + 300.0f };
		const FRotator Rotation = GetActorRotation();
		ABaseBlock* actor = GetWorld()->SpawnActor<ABaseBlock>(GetClass(), Location, Rotation);
		
		actor->SetNewExits(	horizontalExits[0] == Connections::SAMEGROUP ? 2 : horizontalExits[0] == Connections::DIFFERENTGROUP ? 3 : 0,
							horizontalExits[1] == Connections::SAMEGROUP ? 2 : horizontalExits[1] == Connections::DIFFERENTGROUP ? 3 : 0, 
							horizontalExits[2] == Connections::SAMEGROUP ? 2 : horizontalExits[2] == Connections::DIFFERENTGROUP ? 3 : 0, 
							horizontalExits[3] == Connections::SAMEGROUP ? 2 : horizontalExits[3] == Connections::DIFFERENTGROUP ? 3 : 0);
		actor->SetStats(Block::BUILDING, HeightFromThis - 1, newMesh, Elements1, Elements2, Elements3, Elements4, Elements5, Elements6);
		blockUp = actor;
		actor->UpdateBuilding();
	}
}

void ABaseBlock::UpdateBuilding()
{
	placedCenter = false;
	//TODO Make more floortypes for heights
	CreateBuildingElement(0, true);	//North
	CreateBuildingElement(1, true);	//East
	CreateBuildingElement(2, true);	//South
	CreateBuildingElement(3, true);	//West
	CreateBuildingElement(4, true);	//Up
}

void ABaseBlock::UpdateAll()
{
	for (int i = 0; i < StaticElements.Num(); i++)
		CreateBuildingElement(i, false);

	if (blockUp != nullptr)
		blockUp->UpdateAll();
}

void ABaseBlock::SetOneSide(int side, bool sideClosed, bool rotate)
{
	if(rotate)
		StaticElements[side]->AddRelativeRotation(StaticMesh->GetRelativeRotation().GetInverse());
	FRotator Rotation;
	const FVector Location = GetActorLocation();
	std::discrete_distribution<int> SeleccionItem;
	switch (side)
	{
	case 0:
		//North
		Rotation = FRotator::MakeFromEuler(FVector{ 0,0,0 });
		SeleccionItem.param({ (double)!placedCenter,(double)!sideClosed,(double)!sideClosed,(double)sideClosed,(double)sideClosed,3 });
		SetSideElement(side, SeleccionItem(generator));
		break;
		break;
	case 1:
		//East
	case 2:
		//South
	case 3:
		//West
		Rotation = FRotator::MakeFromEuler(FVector{ 0,0,90.0f * side });
		SeleccionItem.param({ 0,(double)!sideClosed,(double)!sideClosed,(double)sideClosed,(double)sideClosed,3 });
		SetSideElement(side, SeleccionItem(generator));
		break;
	case 4:
		//General
		Rotation = FRotator::MakeFromEuler(FVector{ 0,0,90.0f * (generator() % 4) });
		if (sideClosed)
		{
			StaticElements[side]->SetStaticMesh(Addons_6[generator() % Addons_6.Num()]);
		}
		else
		{
			//Set weighted probabilities: Alcantarilla, Basura, Luces, Tapas, Correo, Vacio
			SeleccionItem.param({ (double)!placedCenter,1,0,0,0,3});
			SetSideElement(side, SeleccionItem(generator));
		}
		break;
	}
	if(rotate)
		StaticElements[side]->AddLocalRotation(Rotation);
}

void ABaseBlock::CreateBuildingElement(int side, bool rotate)
{
	if (currentBlock == Block::BUILDING)
	{
		switch (side)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			if (rotate)
			{
				FRotator Rotation = FRotator::MakeFromEuler(FVector{ 0,0,90.0f * side });	// North - East - South - West
				StaticElements[side]->AddLocalRotation(Rotation);
			}
			switch (horizontalExits[side])
			{
			case Connections::DISCONNECTED:
				StaticElements[side]->SetStaticMesh(Addons_2[generator() % Addons_2.Num()]);
				break;
			case Connections::EXIT:
				StaticElements[side]->SetStaticMesh(Addons_3[generator() % Addons_3.Num()]);
				break;
			case Connections::SAMEGROUP:
				StaticElements[side]->SetStaticMesh(Addons_4[generator() % Addons_4.Num()]);
				break;
			case Connections::DIFFERENTGROUP:
				StaticElements[side]->SetStaticMesh(Addons_1[generator() % Addons_1.Num()]);
				break;
			}
			break;
		case 4:
			if (floored)
				StaticElements[side]->SetStaticMesh(Addons_5[generator() % Addons_5.Num()]);
			else
				StaticElements[side]->SetStaticMesh(Addons_6[generator() % Addons_6.Num()]);
			break;
		}
		return;
	}
	if (currentBlock == Block::PARK)
	{
		switch (side)
		{
		case 0:
			SetOneSide(side, generator() % 2, rotate);
			break;
		case 1:
			SetOneSide(side, generator() % 2, rotate);
			break;
		case 2:
			SetOneSide(side, generator() % 2, rotate);
			break;
		case 3:
			SetOneSide(side, generator() % 2, rotate);
			break;
		default:
			//General option
			SetOneSide(side, generator() % 2, rotate);
			break;
		}
		return;
	}
	int* list;
	if (currentBlock > Block::ROAD_N_S_E_W) 
	{
		switch (side)
		{
		case 0:
			list = AllRiverNorth;
			SetOneSide(side, !Find(currentBlock, list, 4), rotate);
			break;
		case 1:
			list = AllRiverEast;
			SetOneSide(side, !Find(currentBlock, list, 4), rotate);
			break;
		case 2:
			list = AllRiverSouth;
			SetOneSide(side, !Find(currentBlock, list, 4), rotate);
			break;
		case 3:
			list = AllRiverWest;
			SetOneSide(side, !Find(currentBlock, list, 4), rotate);
			break;
		default:
			//General option
			SetOneSide(side, generator() % 2, rotate);
			break;
		}
		return;
	}
	switch (side)
	{
	case 0:
		list = AllRoadsNorth;
		SetOneSide(side, !Find(currentBlock, list, 7), rotate);
		break;
	case 1:
		list = AllRoadsEast;
		SetOneSide(side, !Find(currentBlock, list, 7), rotate);
		break;
	case 2:
		list = AllRoadsSouth;
		SetOneSide(side, !Find(currentBlock, list, 7), rotate);
		break;
	case 3:
		list = AllRoadsWest;
		SetOneSide(side, !Find(currentBlock, list, 7), rotate);
		break;
	default:
		//General option
		SetOneSide(side, ((currentBlock >= Block::ROAD_N_S_E && currentBlock <= Block::ROAD_N_S_E_W) || currentBlock == Block::BRIDGE_E_W || currentBlock == Block::BRIDGE_N_S), rotate);
		break;
	}
}

void ABaseBlock::SetSideElement(int side, int choice)
{
	switch (choice)
	{
	case 0:
		placedCenter = true;
		StaticElements[side]->SetStaticMesh(Addons_1[generator() % Addons_1.Num()]);
		break;
	case 1:
		StaticElements[side]->SetStaticMesh(Addons_2[generator() % Addons_2.Num()]);
		break;
	case 2:
		StaticElements[side]->SetStaticMesh(Addons_3[generator() % Addons_3.Num()]);
		break;
	case 3:
		StaticElements[side]->SetStaticMesh(Addons_4[generator() % Addons_4.Num()]);
		break;
	case 4:
		StaticElements[side]->SetStaticMesh(Addons_5[generator() % Addons_5.Num()]);
		break;
	default:
		break;
	}
}



void ABaseBlock::SetNewExits(int North, int South, int East, int West)
{
	horizontalExits[0] = StaticCast<Connections>(North);
	horizontalExits[1] = StaticCast<Connections>(South);
	horizontalExits[2] = StaticCast<Connections>(East);
	horizontalExits[3] = StaticCast<Connections>(West);
	return;
}

void ABaseBlock::toggleFloor()
{
	floored = true;
}

void ABaseBlock::GetMesh(Block selected)
{
	StaticMesh->SetRelativeRotation(FQuat::MakeFromEuler(FVector{0,0,0}));
	switch (selected)
	{
	case Block::ROAD_N:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		break;
	case Block::ROAD_S:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::ROAD_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::ROAD_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		break;
	case Block::ROAD_N_S:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::ROAD_N_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::ROAD_N_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		break;
	case Block::ROAD_S_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::ROAD_S_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		break;
	case Block::ROAD_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::ROAD_N_S_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		break;
	case Block::ROAD_N_S_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::ROAD_N_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::ROAD_S_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		break;
	case Block::ROAD_N_S_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::RIVER_N_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::RIVER_N_S:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::RIVER_N_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		break;
	case Block::RIVER_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::RIVER_S_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::RIVER_S_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		break;
	case Block::BRIDGE_N_S:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		break;
	case Block::BRIDGE_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::BUILDING:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		break;
	case Block::PARK:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
	default:
		break;
	}
	return;
}



void ABaseBlock::Remove()
{
	if (blockUp != nullptr)
		blockUp->Remove();
	Destroy();
}
