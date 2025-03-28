// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchServerData.generated.h"

UENUM(BlueprintType)
enum EServerState : uint8
{
	Unknown = 0,
	Free,
	Drafting,
	Lobby,
	InGame,
	PostMatch
};

UENUM(BlueprintType)
enum EServerType : uint8
{
	Private = 0,
	Public = 1,
	Matchmaking = 2
};


USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FGameServerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Id;
	UPROPERTY(BlueprintReadWrite)
	int32 ServerType;
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	FString Address;
	UPROPERTY(BlueprintReadWrite)
	int32 Port;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Players;
	UPROPERTY(BlueprintReadWrite)
	int64 CreationTime;
	UPROPERTY(BlueprintReadWrite)
	int64 UpdatedTime;
	UPROPERTY(BlueprintReadWrite)
	int64 LastAliveTime;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 State;
};

USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FGameServerGroup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FGameServerData> Servers;
};
