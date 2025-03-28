// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EncryptionHelper.generated.h"

/**
 * 
 */
UCLASS()
class UE5ONLINEPROJECT_API UEncryptionHelper : public UObject
{
	GENERATED_BODY()

public:
	// Encrypts a string using AES-256 and returns the encrypted data as a Base64 string
	UFUNCTION(BlueprintCallable, Category = "Encryption")
	static FString EncryptString(const FString& PlainText, const FString& Key);

	// Decrypts a Base64 string using AES-256
	UFUNCTION(BlueprintCallable, Category = "Encryption")
	static FString DecryptString(const FString& EncryptedText, const FString& Key);
};
