// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/AccountSubsystem.h"
#include "BackendData.h"
#include "JsonObjectConverter.h"
#include "Components/BackendWrapper.h"
#include "Online/AuthenticationSubsystem.h"
#include "Online/Data/AccountData.h"

void UAccountSubsystem::InitialiseBackend()
{
	if (!BackendWrapper)
	{
		if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			if (const auto AuthSystem = GameInstance->GetSubsystem<UAuthenticationSubsystem>())
			{
				BackendWrapper = AuthSystem->GetBackendWrapper();
			}
		}
	}
}

FAccountData UAccountSubsystem::GetSavedAccount()
{
	return Account;
}

void UAccountSubsystem::GetAccountInformation(FAccountInfoResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = GET;
	Data.Path = "GameAccount";

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            FAccountData Data;
					                            if (FJsonObjectConverter::JsonObjectStringToUStruct<FAccountData>(
						                            Response.Content, &Data))
					                            {
						                            Account = Data;
						                            Callback.ExecuteIfBound(true, Data);
						                            return;
					                            }
				                            }
			                            }
			                            Callback.ExecuteIfBound(false, FAccountData());
		                            }));
}

void UAccountSubsystem::GetShopItems(FShopItemResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = GET;
	Data.Path = "shop/items";

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            FGroupItemData GroupItems;
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            if (FJsonObjectConverter::JsonObjectStringToUStruct<FGroupItemData>(
						                            Response.Content, &GroupItems))
					                            {
						                            Callback.ExecuteIfBound(true, GroupItems.Items);
						                            return;
					                            }
				                            }
			                            }
			                            Callback.ExecuteIfBound(false, TArray<FShopItemData>());
		                            }));
}

void UAccountSubsystem::BuyShopItem(const FString& ItemId, FShopTransactionResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = GET;
	Data.Parameters.Add(ItemId);
	Data.Path = "shop/buy";

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            FShopTransactionData Transaction;
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            if (FJsonObjectConverter::JsonObjectStringToUStruct<
						                            FShopTransactionData>(
						                            Response.Content, &Transaction))
					                            {
						                            Callback.ExecuteIfBound(true, Transaction);
						                            return;
					                            }
				                            }
			                            }
			                            Callback.ExecuteIfBound(false, Transaction);
		                            }));
}

void UAccountSubsystem::GetItems(const TArray<FString>& ItemID, FShopItemResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = POST;
	Data.Path = "shop/items";

	FGroupItemID Items;
	Items.Items = ItemID;
	FJsonObjectConverter::UStructToJsonObjectString<FGroupItemID>(Items, Data.JsonContent);

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            FGroupItemData GroupItems;
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            if (FJsonObjectConverter::JsonObjectStringToUStruct<FGroupItemData>(
						                            Response.Content, &GroupItems))
					                            {
						                            Callback.ExecuteIfBound(true, GroupItems.Items);
						                            return;
					                            }
				                            }
			                            }
			                            Callback.ExecuteIfBound(false, TArray<FShopItemData>());
		                            }));
}

void UAccountSubsystem::EquipItem(const FString ItemID, FAccountResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = GET;
	Data.Path = "GameAccount/equipitem";
	Data.Parameters.Add(ItemID);

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            Callback.Execute(true);
					                            return;
				                            }
			                            }
			                            Callback.Execute(false);
		                            }));
}

void UAccountSubsystem::GetItemsByOwned(const TArray<FOwnedItemData>& Items, FShopItemResponse Callback)
{
	if (Items.Num() == 0)return;
	TArray<FString> ItemIDs;
	for (auto Owned : Items)
	{
		ItemIDs.Add(Owned.ItemId);
	}
	GetItems(ItemIDs, Callback);
}

void UAccountSubsystem::GetItemsByOwnedID(const TArray<FString>& OwnedItems, FShopItemResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = POST;
	Data.Path = "shop/owned";

	FGroupItemID Items;
	Items.Items = OwnedItems;
	FJsonObjectConverter::UStructToJsonObjectString<FGroupItemID>(Items, Data.JsonContent);

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            FGroupItemData GroupItems;
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            if (FJsonObjectConverter::JsonObjectStringToUStruct<FGroupItemData>(
						                            Response.Content, &GroupItems))
					                            {
						                            Callback.ExecuteIfBound(true, GroupItems.Items);
						                            return;
					                            }
				                            }
			                            }
			                            Callback.ExecuteIfBound(false, TArray<FShopItemData>());
		                            }));
}

void UAccountSubsystem::GetOpenChest(const FString& ChestItemID, FShopTransactionResponse Callback)
{
	InitialiseBackend();
	FCommandData Data;
	Data.Verbose = GET;
	Data.Parameters.Add(ChestItemID);
	Data.Path = "shop/open";

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            FShopTransactionData Transaction;
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            if (FJsonObjectConverter::JsonObjectStringToUStruct<
						                            FShopTransactionData>(
						                            Response.Content, &Transaction))
					                            {
						                            Callback.ExecuteIfBound(true, Transaction);
						                            return;
					                            }
				                            }
			                            }
			                            Callback.ExecuteIfBound(false, Transaction);
		                            }));
}
