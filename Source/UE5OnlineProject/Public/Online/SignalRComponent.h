// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IHubConnection.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SignalRComponent.generated.h"




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ONLINEPROJECT_API USignalRComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USignalRComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString ServerURL = TEXT("https://127.0.0.1"); // Your server URL. You can use ws, wss or wss+insecure.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Port = TEXT("44340");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Route;

	TSharedPtr<IHubConnection> Hub;
	TMap<FString, FString> Parameters;
	TMap<FString, FString> Headers;

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Connect();
	UFUNCTION(BlueprintCallable)
	void Close();

	void CreateHub();

	UFUNCTION(BlueprintCallable)
	void AddParam(const FString& key, const FString& value);
	UFUNCTION(BlueprintCallable)
	void AddHeader(const FString& key, const FString& value);

	UFUNCTION(BlueprintCallable)
	void Negotiate();
	UFUNCTION(BlueprintCallable)
	void SendMessage(const FString& UserName, const FString& Message);
	UFUNCTION(BlueprintCallable)
	void SendRaw(const FString& RequestName, TArray<FString> Arguments);
	UFUNCTION(BlueprintCallable)
	bool IsConnected() const;
	UFUNCTION()
	void OnMessageResponse(const FString& Message);

	void PrintResponse(const FString& EventName, const TArray<FSignalRValue>& Arguments);
};
