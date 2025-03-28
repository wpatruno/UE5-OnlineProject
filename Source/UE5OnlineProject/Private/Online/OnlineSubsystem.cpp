// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/OnlineSubsystem.h"
#include "IHubConnection.h"
#include "SignalRSubsystem.h"


void UOnlineSubsystem::Connect(const FString& URL, const FString& Token)
{
	ServerURL = URL;
	UserToken = Token;
	if (!Hub)
	{
		CreateHub();
	}

	Hub->Start();
}

void UOnlineSubsystem::Close()
{
	if (Hub)
	{
		Hub->Stop();
	}
}

bool UOnlineSubsystem::IsConnected() const
{
	if (Hub)
	{
		return true;
	}
	return false;
}

void UOnlineSubsystem::CreateHub()
{
	TMap<FString, FString> InHeaders;
	//InHeaders.Add(TEXT("access_token"), UserToken);
	ServerURL += TEXT("?access_token=") + UserToken;
	Hub = GEngine->GetEngineSubsystem<USignalRSubsystem>()->CreateHubConnection(ServerURL, InHeaders);
	UE_LOG(LogTemp, Warning, TEXT("SignalR Connect to %s"), *ServerURL);

	Hub->On(TEXT("ReceiveMessage")).BindLambda([](const TArray<FSignalRValue>& Arguments)
	{
		if (Arguments.Num() >= 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("Response: %s / %s"), *Arguments[0].AsString(), *Arguments[1].AsString());
		}
		else if (Arguments.Num() == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Arguments[0].AsString());
		}
		else UE_LOG(LogTemp, Warning, TEXT("Response: No Args"));
	});

	Hub->On(TEXT("ServerMessage")).BindLambda([this](const TArray<FSignalRValue>& Arguments)
	{
		PrintResponse(TEXT("ServerMessage"), Arguments);
	});

	Hub->OnClosed().AddStatic(&UOnlineSubsystem::OnClosedCallback);
}

void UOnlineSubsystem::OnClosedCallback()
{
	UE_LOG(LogTemp, Warning, TEXT("Signal R connection closed"));
}

void UOnlineSubsystem::SendMessage(const FString& UserName, const FString& Message)
{
	Hub->Invoke(TEXT("ServerMessage"), UserName, Message).BindLambda([](const FSignalRInvokeResult& Result)
	{
		if (!Result.HasError())
		{
			if (Result.IsString())
				UE_LOG(LogTemp, Warning, TEXT("The result value is: %s"), *Result.AsString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ServerMessage Failed %s"), *Result.GetErrorMessage());
		}
	});
}

void UOnlineSubsystem::SendRawArgs(const FString& RequestName, TArray<FString> Arguments)
{
	TArray<FSignalRValue> InArguments;
	if (Arguments.Num() > 0)
	{
		for (auto Element : Arguments)
		{
			InArguments.Add(FSignalRValue(Element));
		}
	}
	Hub->Send(RequestName, InArguments);
}

void UOnlineSubsystem::SendRaw(const FString& RequestName)
{
	Hub->Send(RequestName);
}

TSharedPtr<IHubConnection> UOnlineSubsystem::GetHubConnection() const
{
	return Hub;
}

void UOnlineSubsystem::PrintResponse(const FString& EventName, const TArray<FSignalRValue>& Arguments)
{
	FString Str = "";
	for (auto Value : Arguments)
	{
		if (Value.IsString())
		{
			Str += Value.AsString() + ", ";
		}
		else if (Value.IsNumber())
		{
			Str += Value.AsString() + ", ";
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("On %s: %s"), *EventName, *Str);
}
