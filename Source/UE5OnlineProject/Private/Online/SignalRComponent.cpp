// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Public/Online/SignalRComponent.h"
#include "IHubConnection.h"
#include "SignalRSubsystem.h"

// Sets default values for this component's properties
USignalRComponent::USignalRComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USignalRComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


void USignalRComponent::Connect()
{
	if (!Hub)
	{
		CreateHub();
	}
	//if (!Hub->IsConnected())
	{
		Hub->Start();
	}
}

void USignalRComponent::Close()
{
	if (Hub)
	{
		Hub->Stop();
	}
}

void USignalRComponent::CreateHub()
{
	FString CompleteURL = ServerURL + ":" + Port + "/" + Route;

	if (Parameters.Num() > 0)
	{
		FString Params;
		bool First = true;
		for (auto Pair : Parameters)
		{
			if (First)
			{
				Params += "?" + Pair.Key + "=" + Pair.Value;
				First = false;
			}
			else
			{
				Params += "&" + Pair.Key + "=" + Pair.Value;
			}
		}
		CompleteURL += Params;
	}

	Hub = GEngine->GetEngineSubsystem<USignalRSubsystem>()->CreateHubConnection(CompleteURL, Headers);
	UE_LOG(LogTemp, Warning, TEXT("SignalR Connect to %s"), *CompleteURL);

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

	Hub->On(TEXT("MatchFound")).BindLambda([this](const TArray<FSignalRValue>& Arguments)
	{
		PrintResponse(TEXT("MatchFound"), Arguments);
	});
}

void USignalRComponent::AddParam(const FString& key, const FString& value)
{
	Parameters.Add(key, value);
}

void USignalRComponent::AddHeader(const FString& key, const FString& value)
{
	Headers.Add(key, value);
}

void USignalRComponent::Negotiate()
{
	Hub->Invoke(TEXT("Negotiate"));
}

void USignalRComponent::SendMessage(const FString& UserName, const FString& Message)
{
	//SharedSelf->Send("{\"protocol\": \"json\",\"version\": 1}"); //
	Hub->Invoke(TEXT("SendMessage"), UserName, Message).BindLambda([](const FSignalRInvokeResult& Result)
	{
		if (!Result.HasError())
		{
			if (Result.IsString())
				UE_LOG(LogTemp, Warning, TEXT("The result value is: %s"), *Result.AsString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SendMessage Failed %s"), *Result.GetErrorMessage());
		}
	});
}

void USignalRComponent::SendRaw(const FString& RequestName, TArray<FString> Arguments)
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

bool USignalRComponent::IsConnected() const
{
	if (Hub)
	{
		return true;
	}
	return false;
}

void USignalRComponent::OnMessageResponse(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("WS: %s"), *Message);
}

void USignalRComponent::PrintResponse(const FString& EventName, const TArray<FSignalRValue>& Arguments)
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
