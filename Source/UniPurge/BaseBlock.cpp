// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBlock.h"


// Sets default values
ABaseBlock::ABaseBlock()
{
	blockUp = nullptr;
	currentBlock = Block::NOTHING;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	HeightFromThis = 0;
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
		actor->UpdateBuilding();
		blockUp = actor;
	}
	
}

void ABaseBlock::UpdateBuilding()
{
	//TODO Make more floortypes for heights
	CreateBuildingElement(0);	//North
	CreateBuildingElement(1);	//East
	CreateBuildingElement(2);	//South
	CreateBuildingElement(3);	//West
	CreateBuildingElement(4);	//Up
}

void ABaseBlock::UpdateAll()
{
	if (currentBlock == Block::BUILDING)
	{
		for (int i = 0; i < StaticElements.Num(); i++)
			UpdateAdditionsBuilding(i);
	}
	if (blockUp != nullptr)
		blockUp->UpdateAll();
}

void ABaseBlock::CreateBuildingElement(int side)
{
	FRotator Rotation;
	const FVector Location = GetActorLocation();
	switch (currentBlock)
	{
	case Block::ROAD_N:
	case Block::ROAD_S:
	case Block::ROAD_E:
	case Block::ROAD_W:
	case Block::ROAD_N_S:
	case Block::ROAD_N_E:
	case Block::ROAD_N_W:
	case Block::ROAD_S_E:
	case Block::ROAD_S_W:
	case Block::ROAD_E_W:
	case Block::ROAD_N_S_E:
	case Block::ROAD_N_S_W:
	case Block::ROAD_N_E_W:
	case Block::ROAD_S_E_W:
	case Block::ROAD_N_S_E_W:
		switch (side)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			break;
		case 4:
			StaticElements[side]->SetStaticMesh(Addons_6[generator() % Addons_6.Num()]);
			break;
		}
		break;
	case Block::RIVER_N_S:
	case Block::BRIDGE_N_S:
	case Block::RIVER_N_E:
	case Block::RIVER_N_W:
	case Block::RIVER_S_E:
	case Block::RIVER_S_W:
	case Block::RIVER_E_W:
	case Block::BRIDGE_E_W:
		switch (side)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			break;
		case 4:
			StaticElements[side]->SetStaticMesh(Addons_6[generator() % Addons_6.Num()]);
			break;
		}
		break;
		break;
	case Block::BUILDING:
		switch (side)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			Rotation = FRotator::MakeFromEuler(FVector{ 0,0,90.0f * side });	// North - East - South - West
			StaticElements[side]->AddLocalRotation(Rotation);
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
		break;
	case Block::PARK:
		switch (side)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			break;
		case 4:
			StaticElements[side]->SetStaticMesh(Addons_6[generator() % Addons_6.Num()]);
			break;
		}
		break;
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

void ABaseBlock::UpdateAdditionsBuilding(int element)
{
	switch (element)	//0-4 Horizontal
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		switch (horizontalExits[element])
		{
		case Connections::DISCONNECTED:
			StaticElements[element]->SetStaticMesh(Addons_2[generator() % Addons_2.Num()]);
			break;
		case Connections::EXIT:
			StaticElements[element]->SetStaticMesh(Addons_3[generator() % Addons_3.Num()]);
			break;
		case Connections::SAMEGROUP:
			StaticElements[element]->SetStaticMesh(Addons_4[generator() % Addons_4.Num()]);
			break;
		case Connections::DIFFERENTGROUP:
			StaticElements[element]->SetStaticMesh(Addons_1[generator() % Addons_1.Num()]);
			break;
		}
		break;
	case 5:
		if (floored)
		{
			StaticElements[element]->SetStaticMesh(Addons_5[generator() % Addons_5.Num()]);
		}
		else
		{
			StaticElements[element]->SetStaticMesh(Addons_5[generator() % Addons_5.Num()]);
		}
		break;
	}
}

void ABaseBlock::Remove()
{
	if (blockUp != nullptr)
		blockUp->Remove();
	Destroy();
}
