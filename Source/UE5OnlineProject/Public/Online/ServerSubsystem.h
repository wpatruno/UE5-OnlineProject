// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/MatchServerData.h"
#include "Components/BackendWrapper.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ServerSubsystem.generated.h"

/**
 * Game server commands
 */
UCLASS()
class UE5ONLINEPROJECT_API UServerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FGameServerData ServerData;

	// If not empty, only limit connection from this players
	TArray<FString> WhiteListPlayers;
	bool Server;
	bool ServerReady;
	FString Password;

	void RegisterPublic();
	void UnregisterPublic();

	UPROPERTY()
	UBackendWrapper* BackendWrapper;
	UBackendWrapper* GetBackendWrapper();

public:
	UFUNCTION(BlueprintCallable)
	void StartLocalServerAuto(const FString& ServerName, bool IsPublic, const FString& Password);

	UFUNCTION(BlueprintCallable)
	void StartLocalServer(const FString& ServerID, bool IsPublic, const FString& Password,
	                      const TArray<FString>& InWhiteListPlayers);

	UFUNCTION(BlueprintCallable)
	bool IsServer() const;
	UFUNCTION(BlueprintCallable)
	bool IsServerReady() const;

	UFUNCTION(BlueprintCallable)
	void StartMatchmakingServer();

	UFUNCTION(BlueprintCallable)
	void SendServerReady();

	UFUNCTION(BlueprintCallable)
	void SendServerClose();

	UFUNCTION(BlueprintCallable)
	void JoinGameByID(FString MatchID);

	UFUNCTION(BlueprintCallable)
	void ConnectByIP(FString Address) const;

	UFUNCTION(BlueprintCallable)
	void JoinGame(const FGameServerData& Data);
	UFUNCTION(BlueprintCallable)
	void LeaveGame(FString ServerID);
	UFUNCTION(BlueprintCallable)
	FGameServerData GetServerData() const;
};
