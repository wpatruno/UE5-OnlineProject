// Fill out your copyright notice in the Description page of Project Settings.

#include "UE5OnlineProject/Public/Online/AuthenticationSubsystem.h"
#include "UE5OnlineProject//Public/Online/Data/AuthenticationData.h"
#include "BackendData.h"
#include "HttpBackendSubsystem.h"
#include "Security/EncryptionHelper.h"


void UAuthenticationSubsystem::InitialiseBackend(const FString& URL, const TMap<FString, FString>& Headers)
{
	if (!BackendWrapper)
	{
		if (const auto HttpSystem = GEngine->GetEngineSubsystem<UHttpBackendSubsystem>())
		{
			BackendWrapper = HttpSystem->GetBackend(URL, Headers);
			GConfig->GetString(*ConfigCategory, *ConfigTokenKey, LoginToken,
			                   FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath));
		}
	}
}

void UAuthenticationSubsystem::ApplyToken() const
{
	// Set token to backend system
	BackendWrapper->AddBearerToken(AccessToken);

	GConfig->SetString(*ConfigCategory, *ConfigTokenKey, *LoginToken,
	                   FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath));
}

FString UAuthenticationSubsystem::GetLoginToken() const
{
	return LoginToken;
}

UBackendWrapper* UAuthenticationSubsystem::GetBackendWrapper() const
{
	return BackendWrapper;
}

FString UAuthenticationSubsystem::GetAccessToken()
{
	return AccessToken;
}

void UAuthenticationSubsystem::Disconnect()
{
	AccessToken = FString("");
	LoginToken = FString("");
	ApplyToken();
}

bool UAuthenticationSubsystem::IsLoggedIn()
{
	return !AccessToken.IsEmpty();
}

void UAuthenticationSubsystem::Login(const FString& Mail, const FString& Password, const bool KeepLogged,
                                     FAuthResponse Callback)
{
	if (Mail.IsEmpty() || Password.IsEmpty())
	{
		Callback.Execute(false, FString("Mail or Password empty"));
		return;
	}
	if (!BackendWrapper)
	{
		Callback.Execute(false, FString("BackendWrapper not set"));
		return;
	}
	FCommandData Data;
	Data.Verbose = POST;
	Data.Path = AUTH_URL_PATH + FString("/login");
	FLoginDataInput InputData;
	InputData.Email = Mail;
	InputData.Password = Password;
	InputData.KeepLogged = KeepLogged;

	Data.JsonContent = InputData.ToJson();

	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            FLoginDataOutput Data;
					                            Data.FromJson(Response.Content);

					                            if (!Data.AccessToken.IsEmpty())
					                            {
						                            // Set Access token to be used for auth
						                            AccessToken = Data.AccessToken;
						                            LoginToken = Data.LoginToken;

						                            // Refresh login token
						                            ApplyToken();

						                            UE_LOG(LogTemp, Warning,
						                                   TEXT("UGameBackendSubsystem::AutoLogin> LoginToken %s"),
						                                   *LoginToken);
						                            Callback.Execute(true, "Backend token logged in successfully");
					                            }
				                            }
			                            }
			                            Callback.Execute(false, FString::FromInt(Response.StatusCode));
		                            }));
}


void UAuthenticationSubsystem::AutoLogin(FAuthResponse Callback)
{
	if (!BackendWrapper)
	{
		Callback.ExecuteIfBound(false, FString("BackendWrapper not set"));
		return;
	}

	FString Token = GetLoginToken();
	if (Token.IsEmpty())
	{
		Callback.ExecuteIfBound(false, FString("Token is empty"));
		return;
	}

	FCommandData Data;
	Data.Verbose = GET;
	Data.Path = AUTH_URL_PATH;
	Data.Headers.Add(ConfigTokenKey, Token);
	FBackendResponse a;
	UE_LOG(LogTemp, Warning, TEXT("UGameBackendSubsystem::AutoLogin> LogginToken %s"), *Token);
	BackendWrapper->SendCommand(Data, FOnHttpRequestCompleteRaw::CreateLambda(
		                            [this,Callback](const FBackendResponse& Response)
		                            {
			                            if (Response.Success)
			                            {
				                            if (Response.StatusCode == 200)
				                            {
					                            FLoginDataOutput Data;
					                            Data.FromJson(Response.Content);

					                            if (!Data.AccessToken.IsEmpty())
					                            {
						                            // Set Access token to be used for auth
						                            AccessToken = Data.AccessToken;
						                            LoginToken = Data.LoginToken;

						                            // Refresh login token
						                            ApplyToken();
						                            Callback.Execute(true, "Backend token logged in successfully");
						                            return;
					                            }
				                            }
			                            }
			                            Callback.Execute(false, "Could not login");
		                            }));
}
