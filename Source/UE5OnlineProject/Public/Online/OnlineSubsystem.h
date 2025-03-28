// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.generated.h"

class IHubConnection;
class FSignalRValue;
/**
 * Subsystem for two-way online features like matchmaking and friend list
 * Use SignalR implementation
 */
UCLASS()
class UE5ONLINEPROJECT_API UOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	FString ServerURL;
	FString UserToken;
	TSharedPtr<IHubConnection> Hub;

public:
	UFUNCTION(BlueprintCallable)
	void Connect(const FString& URL, const FString& Token);
	UFUNCTION(BlueprintCallable)
	void Close();
	UFUNCTION(BlueprintCallable)
	bool IsConnected() const;
	UFUNCTION(BlueprintCallable)
	void SendMessage(const FString& UserName, const FString& Message);
	UFUNCTION(BlueprintCallable)
	void SendRawArgs(const FString& RequestName, TArray<FString> Arguments);
	UFUNCTION(BlueprintCallable)
	void SendRaw(const FString& RequestName);
	TSharedPtr<IHubConnection> GetHubConnection() const;
	static void OnClosedCallback();

protected:
	void CreateHub();

	void PrintResponse(const FString& EventName, const TArray<FSignalRValue>& Arguments);
};
