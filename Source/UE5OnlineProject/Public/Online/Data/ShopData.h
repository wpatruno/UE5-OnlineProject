// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopData.generated.h"

UENUM()
enum EItemType : uint8
{
	Account,
	Skin,
	Chest,
	Key,
	Game,
};


USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FShopItemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Id = FString();
	UPROPERTY(BlueprintReadWrite)
	FString Name = FString();
	UPROPERTY(BlueprintReadWrite)
	FString Description = FString();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 Rarity = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TEnumAsByte<EItemType> Type; // EItemType
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 MoneyPrice = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool AvailableFromShop;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool AvailableFromChest;
};

USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FGroupItemData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TArray<FShopItemData> Items;
};


USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FGroupItemID
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> Items;
};

USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FOwnedItemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Id = FString(); // Instance id of the owned item
	UPROPERTY(BlueprintReadWrite)
	FString ItemId = FString(); // Id of the BaseItem (shop item)
	UPROPERTY(BlueprintReadWrite)
	int32 Value;
};

USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FShopTransactionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Id = FString();
	UPROPERTY(BlueprintReadWrite)
	FString Status = FString();
	UPROPERTY(BlueprintReadWrite)
	bool Success;
	UPROPERTY(BlueprintReadWrite)
	FString UserId;
	UPROPERTY(BlueprintReadWrite)
	int32 MoneyUsed;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> InputItems;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> OutputOwnedItems;
};
