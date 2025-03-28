// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5OnlineProject/Public/Online/Data/AuthenticationData.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

FString FLoginDataInput::ToJson()
{
	StaticStruct();
	FString JsonString;
	FLoginDataInput* Input = this;
	if (FJsonObjectConverter::UStructToJsonObjectString<FLoginDataInput>(*Input, JsonString))
	{
		UE_LOG(LogTemp, Log, TEXT("Serialized JSON: %s"), *JsonString);
		return JsonString;
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to serialize struct to JSON."));
	return FString();
}

void FLoginDataOutput::FromJson(const FString& JsonString)
{
	FLoginDataOutput Output;
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FLoginDataOutput>(JsonString, &Output))
	{
		AccessToken = Output.AccessToken;
		LoginToken = Output.LoginToken;
	}
}
