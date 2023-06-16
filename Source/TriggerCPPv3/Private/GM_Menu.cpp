// Copyright Epic Games, Inc. All Rights Reserved.

#include "GM_Menu.h"
#include "UObject/ConstructorHelpers.h"
#include <P_VRPawn.h>
#include <Net/UnrealNetwork.h>
#include <PC_VRPawn.h>
#include <GameFramework/GameState.h>
#include <GameFramework/PlayerState.h>

AGM_Menu::AGM_Menu() : Super() {
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = nullptr;
	static ConstructorHelpers::FClassFinder<APlayerController> PCPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PC_VRPawn'"));
	PlayerControllerClass = PCPawn.Class;	
	static ConstructorHelpers::FClassFinder<APlayerState> PSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PS_Menu'"));
	PlayerStateClass = PSPawn.Class;	
	static ConstructorHelpers::FClassFinder<AGameState> GSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.GS_Menu'"));
	GameStateClass = GSPawn.Class;

	SetupSpawn();

}

#pragma region player spawn
void AGM_Menu::SetupSpawn()
{
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), outActors);

	FreeSpawns.Empty();
	UsedSpawns.Empty();

	for (AActor* actor : outActors)
	{
		APlayerStart* start{ Cast<APlayerStart>(actor) };
		if (start) {
			FreeSpawns.Add(start);
		}
	}
}

const FTransform AGM_Menu::FindPlayerStart()
{

	if (FreeSpawns.Num() > 0) {
		APlayerStart* currentSpawn{ FreeSpawns.Top() };
		FTransform trans{ currentSpawn->GetActorTransform() };
		UsedSpawns.Add(currentSpawn);
		FreeSpawns.Remove(currentSpawn);
		return trans;
	}
	return FTransform();

}

void AGM_Menu::SpawnPlayer(APlayerController* controller)
{
	APC_VRPawn* vrController{ Cast<APC_VRPawn>(controller) };
	if (vrController) {
		if (vrController->GetPawn() != nullptr) {
			vrController->GetPawn()->Destroy();
		}

		FTransform trans{ FindPlayerStart() };
		AP_VRPawn* newPawn{ GetWorld()->SpawnActor<AP_VRPawn>(AP_VRPawn::StaticClass(), trans) };
		vrController->Possess(newPawn);
		//vrController->SetupController();
	}
}
#pragma endregion

#pragma region Timing
void AGM_Menu::Tick(float deltaSeconds)
{}
#pragma endregion

#pragma region player connection
void AGM_Menu::OnPostLogin(AController* player)
{
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