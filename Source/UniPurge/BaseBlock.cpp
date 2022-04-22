// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"

// Sets default values
ABaseBlock::ABaseBlock()
{
	currentBlock = Block::NOTHING;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	for (int i = 0; i < std::size(StaticElements); i++)
	{
		StaticElements[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::FromInt(i)));
		StaticElements[i]->AttachToComponent(StaticMesh,FAttachmentTransformRules::KeepWorldTransform);
	}
	RootComponent = StaticMesh;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

void ABaseBlock::SetStats(Block block)
{
	currentBlock = block;
	StaticMesh->SetStaticMesh(GetMesh(currentBlock));
	
}

void ABaseBlock::UpdateBuilding()
{
	CreateBuildingElement(0);	//North
	CreateBuildingElement(1);	//East
	CreateBuildingElement(2);	//South
	CreateBuildingElement(3);	//West
	CreateBuildingElement(4);	//Up
}

void ABaseBlock::CreateBuildingElement(int side)
{
	const FVector Location = GetActorLocation();

	if (side < 4)
	{
		FRotator Rotation = FRotator::MakeFromEuler(FVector{0,0,90.0f * side});	// North - East - South - West
		StaticElements[side]->AddLocalRotation(Rotation);
		switch (horizontalExits[side])
		{
		case Connections::DISCONNECTED:
			StaticElements[side]->SetStaticMesh(OpenWall);
			break;
		case Connections::EXIT:
			StaticElements[side]->SetStaticMesh(DoorWall);
			break;
		case Connections::SAMEGROUP:
			StaticElements[side]->SetStaticMesh(ConectorWall);
			break;
		case Connections::DIFFERENTGROUP:
			StaticElements[side]->SetStaticMesh(BlockWall);
			break;
		}
	}
	else
	{
		FRotator Rotation = GetActorRotation();
		StaticElements[side]->SetStaticMesh(CeilingMesh);
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

UStaticMesh* ABaseBlock::GetMesh(Block selected)
{
	switch (selected)
	{
	case Block::ROAD_N:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		return ListOfMeshes[0];
		break;
	case Block::ROAD_S:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		return ListOfMeshes[0];
		break;
	case Block::ROAD_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		return ListOfMeshes[0];
		break;
	case Block::ROAD_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		return ListOfMeshes[0];
		break;
	case Block::ROAD_N_S:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		return ListOfMeshes[1];
		break;
	case Block::ROAD_N_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		return ListOfMeshes[2];
		break;
	case Block::ROAD_N_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		return ListOfMeshes[2];
		break;
	case Block::ROAD_S_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		return ListOfMeshes[2];
		break;
	case Block::ROAD_S_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		return ListOfMeshes[2];
		break;
	case Block::ROAD_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		return ListOfMeshes[1];
		break;
	case Block::ROAD_N_S_E:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,180 }));
		return ListOfMeshes[3];
		break;
	case Block::ROAD_N_S_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		return ListOfMeshes[3];
		break;
	case Block::ROAD_N_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,90 }));
		return ListOfMeshes[3];
		break;
	case Block::ROAD_S_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,270 }));
		return ListOfMeshes[3];
		break;
	case Block::ROAD_N_S_E_W:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		return ListOfMeshes[4];
		break;
	case Block::BUILDING:
		StaticMesh->AddLocalRotation(FQuat::MakeFromEuler(FVector{ 0,0,0 }));
		return ListOfMeshes[5];
		break;
	default:
		break;
	}
	return nullptr;
}

