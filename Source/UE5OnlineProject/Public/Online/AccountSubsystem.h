// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/AccountData.h"
#include "Components/BackendWrapper.h"
#include "Data/ShopData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AccountSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FAccountResponse, bool, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAccountInfoResponse, bool, Result, FAccountData, Data);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FShopItemResponse, bool, Result, const TArray<FShopItemData>&, Data);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FShopTransactionResponse, bool, Result, const FShopTransactionData&, Data);

/**
 * 
 */
UCLASS()
class UE5ONLINEPROJECT_API UAccountSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UBackendWrapper> BackendWrapper;

public:
	void InitialiseBackend();

	FAccountData Account;

	UFUNCTION(BlueprintCallable)
	FAccountData GetSavedAccount();

	UFUNCTION(BlueprintCallable)
	void GetAccountInformation(FAccountInfoResponse Callback);

	UFUNCTION(BlueprintCallable)
	void GetShopItems(FShopItemResponse Callback);

	UFUNCTION(BlueprintCallable)
	void BuyShopItem(const FString& ItemId, FShopTransactionResponse Callback);

	UFUNCTION(BlueprintCallable)
	void GetItems(const TArray<FString>& ItemID, FShopItemResponse Callback);

	UFUNCTION(BlueprintCallable)
	void EquipItem(const FString ItemID, FAccountResponse Callback);


	UFUNCTION(BlueprintCallable)
	void GetItemsByOwned(const TArray<FOwnedItemData>& Items, FShopItemResponse Callback);

	UFUNCTION(BlueprintCallable)
	void GetItemsByOwnedID(const TArray<FString>& OwnedItems, FShopItemResponse Callback);

	UFUNCTION(BlueprintCallable)
	void GetOpenChest(const FString& ChestItemID, FShopTransactionResponse Callback);
};
