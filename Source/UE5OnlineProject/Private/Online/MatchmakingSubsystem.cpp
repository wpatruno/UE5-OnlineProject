// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/MatchmakingSubsystem.h"

#include "BackendData.h"
#include "IHubConnection.h"
#include "JsonObjectConverter.h"
#include "Online/AuthenticationSubsystem.h"
#include "Online/ServerSubsystem.h"


UBackendWrapper* UMatchmakingSubsystem::GetBackendWrapper()
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
	return BackendWrapper;
}

UOnlineSubsystem* UMatchmakingSubsystem::GetOnlineSubsystem()
{
	if (!OnlineSubsystem)
	{
		if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			OnlineSubsystem = GameInstance->GetSubsystem<UOnlineSubsystem>();
			OnlineSubsystem->GetHubConnection()->On(TEXT("MatchFound")).BindUObject(
				this, &UMatchmakingSubsystem::MatchmakingResponse);
			GetOnlineSubsystem()->GetHubConnection()->On(TEXT("ServerReady")).BindUObject(
				this, &UMatchmakingSubsystem::OnServerReady);
		}
	}
	return OnlineSubsystem;
}

void UMatchmakingSubsystem::OnServerReady(const TArray<FSignalRValue>& SignalRValues) const
{
	if (SignalRValues.Num() > 0)
	{
		if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			if (const UServerSubsystem* Server = GameInstance->GetSubsystem<UServerSubsystem>())
			{
				Server->ConnectByIP(SignalRValues[0].AsString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Server ready Callback XXX"));
	}
}

void UMatchmakingSubsystem::MatchmakingResponse(const TArray<FSignalRValue>& SignalRValues)
{
	IsInMatchmaking = false;
	if (SignalRValues.Num() < 4)
	{
		UE_LOG(LogTemp, Error, TEXT("MatchmakingResponse> Not enough args"));
	}
	FString MatchID = SignalRValues[0].AsString();
	FString Player1 = SignalRValues[1].AsString();
	FString Player2 = SignalRValues[2].AsString();
	bool IsHost = SignalRValues[3].AsBool();
	UE_LOG(LogTemp, Display, TEXT("MatchmakingResponse: Match ID %s and is host %d"), *MatchID, IsHost?1:0);

	if (IsHost)
	{
		CreateGameByID(MatchID);
	}
}

void UMatchmakingSubsystem::JoinMatchmaking(const int32& GameMode)
{
	if (IsInMatchmaking)return;
	IsInMatchmaking = true;
	TArray<FString> Args;

	Args.Add(FString::FromInt(GameMode));
	GetOnlineSubsystem()->SendRawArgs(TEXT("JoinMatchmaking"), Args);
}

void UMatchmakingSubsystem::QuitMatchmaking()
{
	IsInMatchmaking = false;
	GetOnlineSubsystem()->SendRaw(TEXT("QuitMatchmaking"));
}

void UMatchmakingSubsystem::CreateGameByID(FString MatchID)
{
	if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		UServerSubsystem* Server = GameInstance->GetSubsystem<UServerSubsystem>();
		Server->StartLocalServer(MatchID, false, "", TArray<FString>());
	}
}


void UMatchmakingSubsystem::GetPublicServers(const int32& PageIndex, FServerExplorerResponse Callback)
{
	GetBackendWrapper();

	// PageIndex : Can be used later to pull data page per page

	FCommandData Data;
	Data.Verbose = EHttpMethod::GET;
	Data.Path = "gameserver/servers";


	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            FGameServerGroup Output;
			                            if (FJsonObjectConverter::JsonObjectStringToUStruct<FGameServerGroup>(
				                            Response.Content, &Output))
			                            {
				                            Callback.ExecuteIfBound(Response.Success, Output.Servers);
			                            }
		                            }));
}


void UMatchmakingSubsystem::GetPingServer(const FString& ServerID, FServerResponse Response)
{
}

void UMatchmakingSubsystem::PostUpdateServer(const FGameServerData& ServerData, FServerResponse Response)
{
}
