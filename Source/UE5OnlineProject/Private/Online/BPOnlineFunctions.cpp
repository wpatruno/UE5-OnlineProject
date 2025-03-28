// Fill out your copyright notice in the Description page of Project Settings.
#include "Online/BPOnlineFunctions.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"


bool UBPOnlineFunctions::CreateSessionAdvanced(APlayerController* Player, int PublicConnections, bool UseLan,
                                               bool UseLobbies)
{
	auto session = Online::GetSessionInterface(Player->GetWorld());
	FOnlineSessionSettings Settings;
	Settings.bIsLANMatch = UseLan;
	Settings.bUseLobbiesIfAvailable = UseLobbies;
	Settings.bUsesPresence = true;
	Settings.bIsDedicated = false;
	session->CreateSession(*Player->NetConnection->PlayerId.GetUniqueNetId(), FName(""), Settings);
	return true;
}

bool UBPOnlineFunctions::CreateSessionDedicated(int PublicConnections, bool UseLan, bool UseLobbies)
{
	auto session = Online::GetSessionInterface(GWorld);
	FOnlineSessionSettings Settings;
	Settings.bIsLANMatch = UseLan;
	Settings.bUseLobbiesIfAvailable = UseLobbies;
	Settings.bUsesPresence = true;
	Settings.bIsDedicated = true;
	session->CreateSession(0, FName(""), Settings);
	return true;
}
