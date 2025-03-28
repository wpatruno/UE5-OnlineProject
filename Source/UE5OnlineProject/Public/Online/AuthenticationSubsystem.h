// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BackendWrapper.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AuthenticationSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAuthResponse, bool, Result, FString, Message);

/**
 * 
 */
UCLASS()
class UE5ONLINEPROJECT_API UAuthenticationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	// Not for prod
	FString BASE_AES_KEY = FString("1234567890ABCDEF1234567890ABCDEF");
	FString ConfigFilePath = FPaths::ProjectConfigDir() / TEXT("PlayerPrefs.ini");
	FString ConfigCategory = FString("API");
	FString ConfigTokenKey = FString("token");
	FString AUTH_URL_PATH = FString("auth");

	UPROPERTY()
	TObjectPtr<UBackendWrapper> BackendWrapper;

	FString AccessToken;
	FString LoginToken;

	void ApplyToken() const;

public:
	UBackendWrapper* GetBackendWrapper() const;

	UFUNCTION(BlueprintCallable)
	void InitialiseBackend(const FString& URL, const TMap<FString, FString>& Headers);

	UFUNCTION(BlueprintCallable)
	FString GetAccessToken();

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	UFUNCTION(BlueprintCallable)
	bool IsLoggedIn();

	UFUNCTION(BlueprintCallable)
	FString GetLoginToken() const;

	// Login using mail and password to get an AccessToken
	UFUNCTION(BlueprintCallable)
	void Login(const FString& Mail, const FString& Password, const bool KeepLogged, FAuthResponse Callback);

	// Use login token saved locally to login and get an access token (ex: Keep logged in feature of Steam)
	UFUNCTION(BlueprintCallable)
	void AutoLogin(FAuthResponse Callback);
};
