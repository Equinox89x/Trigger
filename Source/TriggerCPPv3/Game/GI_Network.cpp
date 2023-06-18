#include "GI_Network.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include <TriggerCPPv3/Game/GM_Menu.h>
#include <TriggerCPPv3/Actors/Character/PC_VRPawn.h>
#include "OnlineSubsystemUtils.h"
#include <TriggerCPPv3/UI/A_MainMenu.h>

#include <SocketSubsystem.h>
#include <IPAddress.h>


UGI_Network::UGI_Network()
{}

void UGI_Network::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGI_Network::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGI_Network::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGI_Network::OnJoinSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGI_Network::OnDestroySessionComplete);
			SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UGI_Network::OnEndSessionComplete);
			SessionInterface->OnSessionFailureDelegates.AddUObject(this, &UGI_Network::OnSessionFailed);
			GetEngine()->OnNetworkFailure().AddUObject(this, &UGI_Network::HandleNetworkFailure);
		}
	}
}

#pragma region Create Server
void UGI_Network::CreateServer(int playerAmount, FString mapName, FString gameMode, bool isLan)
{
	SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bAllowJoinInProgress = true;
	//SessionSettings->bIsDedicated = true;
	SessionSettings->bIsLANMatch = isLan;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->NumPublicConnections = playerAmount;
	SessionSettings->bAllowInvites = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->Set(SEARCH_PRESENCE, true, EOnlineDataAdvertisementType::ViaOnlineService);
	//SessionSettings->Set(SETTING_MAPNAME, mapName, EOnlineDataAdvertisementType::ViaOnlineService);
	//SessionSettings->Set(SETTING_GAMEMODE, gameMode, EOnlineDataAdvertisementType::ViaOnlineService);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), FName("NoiceSession"), *SessionSettings);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("create!"));
}

void UGI_Network::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/L_Western?listen");
		UGameplayStatics::OpenLevel(GetWorld(), "/Game/Maps/L_Western", true, "listen");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("server travel!"));
	}
}
#pragma endregion

#pragma region Find Session
void UGI_Network::FindServers(FString mapName, FString gameMode, bool isLan)
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = isLan;
	SessionSearch->MaxSearchResults = 10;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//SessionSearch->QuerySettings.Set(SETTING_MAPNAME, mapName, EOnlineComparisonOp::Equals);
	//SessionSearch->QuerySettings.Set(SETTING_GAMEMODE, gameMode, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("searching!"));
}


void UGI_Network::OnFindSessionComplete(bool Succeeded)
{
	if (Succeeded)
	{
		if (SessionSearch) {
			TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("nr of sessions: "+ SearchResults.Num()));
			if (SearchResults.Num() > 0)
			{
				FirstSession = SearchResults[0];
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("found: "+ FirstSession.GetSessionIdStr()));
				JoinServer();
			}
		}
	}
}
#pragma endregion

#pragma region Join Session
void UGI_Network::JoinServer()
{
	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), FName(GetFirstServerName()), FirstSession);
}

void UGI_Network::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (PController)
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
			FString name{ "joined " + JoinAddress + "  |  " + SessionName.ToString()};
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, name);
			//ClientTravelToSession(PController->GetUniqueID(), SessionName);
		}
	}
}
#pragma endregion

#pragma region End Game
void UGI_Network::OnDestroySessionComplete(FName SessionName, bool Succeeded)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("destroy!"));

}

void UGI_Network::OnEndSessionComplete(FName SessionName, bool Succeeded)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("end!"));
}

void UGI_Network::EndMatch(const EName& sessionName)
{
	SessionInterface->EndSession(sessionName);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientTravel("/Game/Maps/FirstPersonMap", ETravelType::TRAVEL_Absolute);
	SessionInterface->DestroySession(sessionName);
}
#pragma endregion

void UGI_Network::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) {
	if (FailureType == ENetworkFailure::FailureReceived || FailureType == ENetworkFailure::ConnectionLost) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (!ensure(PlayerController != nullptr)) return;

		PlayerController->ClientTravel("/Game/Maps/L_Western", ETravelType::TRAVEL_Absolute);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("network error!"));
	}
}

void UGI_Network::OnSessionFailed(const FUniqueNetId& id, ESessionFailure::Type fail)
{
	const FUniqueNetId& netid{ id };
	ESessionFailure::Type text{ fail };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("fail! " + text));
}

void UGI_Network::StartMatch(const EName& sessionName)
{
	SessionInterface->StartSession(sessionName);
}