// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuthenticationData.generated.h"

USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FLoginDataInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Email = FString();
	UPROPERTY(BlueprintReadWrite)
	FString Password = FString();
	UPROPERTY(BlueprintReadWrite)
	bool KeepLogged = false;

	FLoginDataInput()
	{
		Email = FString("");
		Password = FString("");
		KeepLogged = false;
	}

	FString ToJson();
};

USTRUCT(BlueprintType)
struct UE5ONLINEPROJECT_API FLoginDataOutput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString AccessToken = FString();
	UPROPERTY(BlueprintReadWrite)
	FString LoginToken = FString();

	FLoginDataOutput()
	{
		LoginToken = FString("");
		AccessToken = FString("");
	}

	void FromJson(const FString& JsonString);
};
