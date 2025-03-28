// Fill out your copyright notice in the Description page of Project Settings.
#include "Online/ServerSubsystem.h"

#include "BackendData.h"
#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "Online/AuthenticationSubsystem.h"
#include "Online/OnlineSubsystem.h"

class UAuthenticationSubsystem;
class UOnlineSubsystem;


UBackendWrapper* UServerSubsystem::GetBackendWrapper()
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

void UServerSubsystem::StartLocalServerAuto(const FString& ServerName, bool IsPublic, const FString& InPassword)
{
	if (ServerName.Len() < 4)return;
	ServerData.Id = FGuid::NewGuid().ToString();
	ServerData.Name = ServerName;
	ServerData.State = Lobby;
	if (IsPublic)ServerData.ServerType = 1;
	Password = InPassword;
	ServerData.Address = "127.0.0.1"; // Temp local address
	ServerData.Port = 9999; // Temp value
	Server = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"), true, "listen");
}

void UServerSubsystem::StartLocalServer(const FString& ServerID, bool IsPublic, const FString& InPassword,
                                        const TArray<FString>& InWhiteListPlayers)
{
	if (ServerID.Len() < 4)return;
	WhiteListPlayers = InWhiteListPlayers;
	ServerData.Id = ServerID;
	ServerData.Name = ServerID;
	ServerData.State = Lobby;
	if (IsPublic)ServerData.ServerType = 1;
	ServerData.Address = "127.0.0.1"; // Temp local address
	Password = InPassword;
	ServerData.Port = 9999; // Temp value
	Server = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"), true, "listen");
}

void UServerSubsystem::StartMatchmakingServer()
{
	ServerData.Id = FGuid::NewGuid().ToString();
	ServerData.Name = ServerData.Id;
	ServerData.State = Free;
	ServerData.ServerType = 2;
	// Not used for debug
	//ServerData.Password = FGuid::NewGuid().ToString();
	ServerData.Address = "127.0.0.1"; // Temp local address
	ServerData.Port = 9999; // Temp value
	Server = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"), true, "listen");
}

bool UServerSubsystem::IsServer() const
{
	return Server;
}

bool UServerSubsystem::IsServerReady() const
{
	return ServerReady;
}


void UServerSubsystem::SendServerReady()
{
	if (!IsServer())return;

	ServerReady = true;
	if (const UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		auto OnlineSubsystem = GameInstance->GetSubsystem<UOnlineSubsystem>();
		TArray<FString> Args;
		Args.Add(ServerData.Id);
		Args.Add("0");
		Args.Add("Message");
		OnlineSubsystem->SendRawArgs(TEXT("CommandToGroup"), Args);
	}

	if (ServerData.ServerType >= 1)
	{
		RegisterPublic();
	}
}

void UServerSubsystem::SendServerClose()
{
	Server = false;
	ServerReady = false;
	if (ServerData.ServerType >= 1)
	{
		UnregisterPublic();
	}
}

void UServerSubsystem::JoinGameByID(FString MatchID)
{
	// Call backend for server info
	// then connect to it
}

void UServerSubsystem::ConnectByIP(FString Address) const
{
	if (Address.Len() < 3)return;
	FString Command = "open " + Address;
	UE_LOG(LogTemp, Display, TEXT("Connect to %s"), *Command);
	GEngine->Exec(GetWorld(), *Command);
}

void UServerSubsystem::JoinGame(const FGameServerData& Data)
{
	ServerData = Data;
	ConnectByIP(ServerData.Address);
}

void UServerSubsystem::LeaveGame(FString ServerID)
{
}

FGameServerData UServerSubsystem::GetServerData() const
{
	return ServerData;
}


void UServerSubsystem::RegisterPublic()
{
	GetBackendWrapper();

	FCommandData Data;
	Data.Verbose = EHttpMethod::POST;
	Data.Path = "gameserver/register";

	FString JsonString;
	if (FJsonObjectConverter::UStructToJsonObjectString<FGameServerData>(ServerData, JsonString))
	{
		UE_LOG(LogTemp, Log, TEXT("RegisterPublic Serialized JSON: %s"), *JsonString);
		Data.JsonContent = JsonString;

		BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
			                            [this](const FBackendResponse& Response)
			                            {
				                            UE_LOG(LogTemp, Log, TEXT("RegisterPublic Response"));
			                            }));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("RegisterPublic Failed Json serialization"));
	}
}

void UServerSubsystem::UnregisterPublic()
{
	GetBackendWrapper();

	FCommandData Data;
	Data.Verbose = EHttpMethod::GET;
	Data.Path = "gameserver/unregister";
	Data.Parameters.Add(ServerData.Id);

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this](const FBackendResponse& Response)
		                            {
		                            }));
}
