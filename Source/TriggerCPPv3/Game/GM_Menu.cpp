// Copyright Epic Games, Inc. All Rights Reserved.

#include "GM_Menu.h"
#include "UObject/ConstructorHelpers.h"
#include <TriggerCPPv3/Actors/Character/P_VRPawn.h>
#include <Net/UnrealNetwork.h>
#include <TriggerCPPv3/Actors/Character/PC_VRPawn.h>
#include <GameFramework/GameState.h>
#include <GameFramework/PlayerState.h>

AGM_Menu::AGM_Menu() {
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = nullptr;
	static ConstructorHelpers::FClassFinder<APC_VRPawn> PCPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PC_VRPawn'"));
	PlayerControllerClass = PCPawn.Class;		
	static ConstructorHelpers::FClassFinder<AP_VRPawn> PPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.P_VRPawn'"));
	DefaultPawnClass = PPawn.Class;
	static ConstructorHelpers::FClassFinder<APlayerState> PSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PS_Menu'"));
	PlayerStateClass = PSPawn.Class;	
	static ConstructorHelpers::FClassFinder<AGameState> GSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.GS_Menu'"));
	GameStateClass = GSPawn.Class;
}

#pragma region Timing
void AGM_Menu::BeginPlay()
{
	Super::BeginPlay();
}

void AGM_Menu::Tick(float deltaSeconds){}
#pragma endregion

#pragma region player connection
void AGM_Menu::OnPostLogin(AController* player)
{
	Super::OnPostLogin(player);

	ConnectedPlayers.Add(player);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("New Player: " + player->GetName()));
	FString text{ "Player amount : " + ConnectedPlayers.Num() };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, text);
}

void AGM_Menu::OnLogout(AController* player)
{
	ConnectedPlayers.Remove(player);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("removed Player: " + player->GetName()));
	FString text{ "Player amount : " + ConnectedPlayers.Num() };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, text);
}

void AGM_Menu::GenericPlayerInitialization(AController* player)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player init: " + player->GetName()));
}

void AGM_Menu::HandleStartingNewPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("new player"));
}

void AGM_Menu::OnSwapPlayerController(APlayerController* newPlayer, APlayerController* oldController)
{
	ConnectedPlayers.Add(newPlayer);
}
#pragma endregion