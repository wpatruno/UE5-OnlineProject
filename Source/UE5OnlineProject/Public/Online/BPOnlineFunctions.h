// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPOnlineFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UE5ONLINEPROJECT_API UBPOnlineFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable, Category = "CppOnline")
	static bool CreateSessionAdvanced(APlayerController* Player, int PublicConnections, bool UseLan,
	                                  bool UseLobbies);

	UFUNCTION(BlueprintCallable, Category = "CppOnline")
	static bool CreateSessionDedicated(int PublicConnections, bool UseLan,
	                                   bool UseLobbies);
};
