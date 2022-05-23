// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UniPurge/GameMaster.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGameMaster() {}
// Cross Module References
	UNIPURGE_API UClass* Z_Construct_UClass_AGameMaster_NoRegister();
	UNIPURGE_API UClass* Z_Construct_UClass_AGameMaster();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_UniPurge();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
	UNIPURGE_API UClass* Z_Construct_UClass_AUniPurgeCharacter_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	UNIPURGE_API UClass* Z_Construct_UClass_ABasicNPC_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(AGameMaster::execUpdateNPCRadius)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_NewRadius);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateNPCRadius(Z_Param_NewRadius);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(AGameMaster::execUpdateRender)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_NewDistance);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateRender_Implementation(Z_Param_NewDistance);
		P_NATIVE_END;
	}
	static FName NAME_AGameMaster_UpdateRender = FName(TEXT("UpdateRender"));
	void AGameMaster::UpdateRender(int32 NewDistance)
	{
		GameMaster_eventUpdateRender_Parms Parms;
		Parms.NewDistance=NewDistance;
		ProcessEvent(FindFunctionChecked(NAME_AGameMaster_UpdateRender),&Parms);
	}
	void AGameMaster::StaticRegisterNativesAGameMaster()
	{
		UClass* Class = AGameMaster::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "UpdateNPCRadius", &AGameMaster::execUpdateNPCRadius },
			{ "UpdateRender", &AGameMaster::execUpdateRender },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics
	{
		struct GameMaster_eventUpdateNPCRadius_Parms
		{
			int32 NewRadius;
		};
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_NewRadius;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::NewProp_NewRadius = { "NewRadius", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameMaster_eventUpdateNPCRadius_Parms, NewRadius), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::NewProp_NewRadius,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AGameMaster, nullptr, "UpdateNPCRadius", nullptr, nullptr, sizeof(GameMaster_eventUpdateNPCRadius_Parms), Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AGameMaster_UpdateNPCRadius()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_AGameMaster_UpdateNPCRadius_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_AGameMaster_UpdateRender_Statics
	{
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_NewDistance;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::NewProp_NewDistance = { "NewDistance", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameMaster_eventUpdateRender_Parms, NewDistance), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::NewProp_NewDistance,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AGameMaster, nullptr, "UpdateRender", nullptr, nullptr, sizeof(GameMaster_eventUpdateRender_Parms), Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AGameMaster_UpdateRender()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_AGameMaster_UpdateRender_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_AGameMaster_NoRegister()
	{
		return AGameMaster::StaticClass();
	}
	struct Z_Construct_UClass_AGameMaster_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticMesh_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_StaticMesh;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Side_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_Side;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_jugador_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_jugador;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RadiusOfSpawn_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_RadiusOfSpawn;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PlayerIsShifting_MetaData[];
#endif
		static void NewProp_PlayerIsShifting_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_PlayerIsShifting;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ActorToSpawn_MetaData[];
#endif
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_ActorToSpawn;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_NPCToSpawn_MetaData[];
#endif
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_NPCToSpawn;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_NPCsActivos_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_NPCsActivos_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_NPCsActivos;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGameMaster_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_UniPurge,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_AGameMaster_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AGameMaster_UpdateNPCRadius, "UpdateNPCRadius" }, // 85288414
		{ &Z_Construct_UFunction_AGameMaster_UpdateRender, "UpdateRender" }, // 4087505851
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "GameMaster.h" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_StaticMesh_MetaData[] = {
		{ "Category", "GameMaster" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_StaticMesh = { "StaticMesh", nullptr, (EPropertyFlags)0x0010000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, StaticMesh), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_StaticMesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_StaticMesh_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_Side_MetaData[] = {
		{ "Category", "GameMaster" },
		{ "Comment", "/* Side of the square of the desired grid */" },
		{ "ModuleRelativePath", "GameMaster.h" },
		{ "ToolTip", "Side of the square of the desired grid" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_Side = { "Side", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, Side), METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_Side_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_Side_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_jugador_MetaData[] = {
		{ "Category", "Jugador" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_jugador = { "jugador", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, jugador), Z_Construct_UClass_AUniPurgeCharacter_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_jugador_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_jugador_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_RadiusOfSpawn_MetaData[] = {
		{ "Category", "GameMaster" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_RadiusOfSpawn = { "RadiusOfSpawn", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, RadiusOfSpawn), METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_RadiusOfSpawn_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_RadiusOfSpawn_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting_MetaData[] = {
		{ "Category", "GameMaster" },
		{ "Comment", "//TODO Make IsShift in code so that spawns can be disabled\n" },
		{ "ModuleRelativePath", "GameMaster.h" },
		{ "ToolTip", "TODO Make IsShift in code so that spawns can be disabled" },
	};
#endif
	void Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting_SetBit(void* Obj)
	{
		((AGameMaster*)Obj)->PlayerIsShifting = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting = { "PlayerIsShifting", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(AGameMaster), &Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting_SetBit, METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_ActorToSpawn_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "GameMaster" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_ActorToSpawn = { "ActorToSpawn", nullptr, (EPropertyFlags)0x0044000000000001, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, ActorToSpawn), Z_Construct_UClass_AActor_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_ActorToSpawn_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_ActorToSpawn_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCToSpawn_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "GameMaster" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCToSpawn = { "NPCToSpawn", nullptr, (EPropertyFlags)0x0044000000000001, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, NPCToSpawn), Z_Construct_UClass_AActor_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCToSpawn_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCToSpawn_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos_Inner = { "NPCsActivos", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_ABasicNPC_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "GameMaster" },
		{ "ModuleRelativePath", "GameMaster.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos = { "NPCsActivos", nullptr, (EPropertyFlags)0x0040000000000004, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AGameMaster, NPCsActivos), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AGameMaster_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_StaticMesh,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_Side,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_jugador,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_RadiusOfSpawn,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_PlayerIsShifting,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_ActorToSpawn,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCToSpawn,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AGameMaster_Statics::NewProp_NPCsActivos,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGameMaster_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGameMaster>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AGameMaster_Statics::ClassParams = {
		&AGameMaster::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AGameMaster_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AGameMaster_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AGameMaster_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGameMaster()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AGameMaster_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AGameMaster, 103342354);
	template<> UNIPURGE_API UClass* StaticClass<AGameMaster>()
	{
		return AGameMaster::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AGameMaster(Z_Construct_UClass_AGameMaster, &AGameMaster::StaticClass, TEXT("/Script/UniPurge"), TEXT("AGameMaster"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGameMaster);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
