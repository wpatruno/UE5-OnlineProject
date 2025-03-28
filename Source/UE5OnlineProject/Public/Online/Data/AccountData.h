// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopData.h"
#include "AccountData.generated.h"
/**
 *
 */


USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FAccountData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Id = FString();
	UPROPERTY(BlueprintReadWrite)
	FString Name = FString();
	UPROPERTY(BlueprintReadWrite)
	FString Mail = FString();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 XP = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 Elo = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 Level = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 Money = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 ChestKeys = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<FOwnedItemData> Inventory;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString EquippedItemID;
};
