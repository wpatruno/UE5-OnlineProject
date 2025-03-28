// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Components/BackendWrapper.h"
#include "Data/MatchServerData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MatchmakingSubsystem.generated.h"


DECLARE_DYNAMIC_DELEGATE_TwoParams(FServerResponse, bool, Result, const FString&, ServerID);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FServerRegisterResponse, bool, Result, const FGameServerData&, Data);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FServerExplorerResponse, bool, Result, const TArray<FGameServerData>&, Data);

/**
 * 
 */
UCLASS()
class UE5ONLINEPROJECT_API UMatchmakingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UOnlineSubsystem> OnlineSubsystem;
	bool IsInMatchmaking;

	UPROPERTY()
	UBackendWrapper* BackendWrapper;

	UBackendWrapper* GetBackendWrapper();
	UOnlineSubsystem* GetOnlineSubsystem();

	void OnServerReady(const TArray<FSignalRValue>& SignalRValues) const;

	void MatchmakingResponse(const TArray<FSignalRValue>& SignalRValues);

public:
	UFUNCTION(BlueprintCallable)
	void JoinMatchmaking(const int32& GameMode);

	UFUNCTION(BlueprintCallable)
	void QuitMatchmaking();

	UFUNCTION(BlueprintCallable)
	void CreateGameByID(FString MatchID);

	UFUNCTION(BlueprintCallable)
	void GetPublicServers(const int32& PageIndex, FServerExplorerResponse Callback);

	UFUNCTION(BlueprintCallable)
	void GetPingServer(const FString& ServerID, FServerResponse Response);

	UFUNCTION(BlueprintCallable)
	void PostUpdateServer(const FGameServerData& ServerData, FServerResponse Response);
};
