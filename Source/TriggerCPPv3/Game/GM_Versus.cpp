// Fill out your copyright notice in the Description page of Project Settings.

#include <TriggerCPPv3/Game/GM_Versus.h>
#include "UObject/ConstructorHelpers.h"
#include <TriggerCPPv3/Actors/Character/P_VRPawn.h>
#include <Net/UnrealNetwork.h>
#include <TriggerCPPv3/Actors/Character/PC_VRPawn.h>
#include <GameFramework/GameState.h>
#include <GameFramework/PlayerState.h>
#include <TriggerCPPv3/Game/GS_Menu.h>

AGM_Versus::AGM_Versus(): Super() {
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = nullptr;
	static ConstructorHelpers::FClassFinder<APlayerController> PCPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PC_VRPawn'"));
	PlayerControllerClass = PCPawn.Class;
	static ConstructorHelpers::FClassFinder<APlayerState> PSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PS_Menu'"));
	PlayerStateClass = PSPawn.Class;
	static ConstructorHelpers::FClassFinder<AGameState> GSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.GS_Menu'"));
	GameStateClass = GSPawn.Class;
}

void AGM_Versus::BeginPlay()
{
	Super::BeginPlay();
	UGI_Network* gi{ Cast<UGI_Network>(GetWorld()->GetGameInstance()) };
	if (gi) {
		GameInstance = gi;
	}

	SetupSpawn();

	/*FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AGM_Versus::Countdown, 1.f, true, 0);*/
}

#pragma region Player Spawning
void AGM_Versus::SetupSpawn(bool isNewGame)
{
	FreeSpawns.Empty();
	UsedSpawns.Empty();
	CurrentSpawn.Empty();

	//get all spawns and pick a random one
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AA_Spawn::StaticClass(), outActors);
	int spawnIndex{ FMath::RandRange(0, outActors.Num() - 1) };
	AA_Spawn* spawn{ Cast<AA_Spawn>(outActors[spawnIndex]) };

	//extract the PlayerStart actor positions
	if (spawn) {
		spawn->SetActorHiddenInGame(false);
		ChosenSpawn = spawn;
		CurrentSpawn = TArray{ Cast<APlayerStart>(spawn->GetSheriffSpawn()), Cast<APlayerStart>(spawn->GetBanditSpawn()) };
		outActors.RemoveAt(spawnIndex);
	}

	//Hide the other spawns
	for (size_t i = 0; i < FreeSpawns.Num(); i++)
	{
		AA_Spawn* spawnToHide{ Cast<AA_Spawn>(FreeSpawns[i]) };
		if (spawnToHide) {
			spawnToHide->SetActorHiddenInGame(true);
		}
	}
}

const FTransform AGM_Versus::FindPlayerStart()
{
	//Return the location of one of the PlayerStart Actors
	if (CurrentSpawn.Num() > 0) {
		APlayerStart* playerSpawn{ CurrentSpawn.Pop() };
		UsedSpawns.Add(playerSpawn);
		return FTransform(FRotator(0.000000, 179.999999, 0.000000), FVector(394.528538, 857.629368, 31.127607), FVector(1,1,1));
		//return playerSpawn->GetActorTransform();
	}
	return FTransform();
}

void AGM_Versus::SpawnPlayer(APlayerController* controller)
{
	APC_VRPawn* vrController{ Cast<APC_VRPawn>(controller) };
	if (vrController) {
		if (vrController->GetPawn() != nullptr) {
			vrController->GetPawn()->Destroy();
		}

		AP_VRPawn* newPawn{ GetWorld()->SpawnActor<AP_VRPawn>(AP_VRPawn::StaticClass(), FindPlayerStart()) };
		ChosenSpawn->SetPlayer(newPawn);
		newPawn->OwningSpawn = ChosenSpawn;
		vrController->Possess(newPawn);
		vrController->AcknowledgePossession(newPawn);
		//newPawn->SetupController();
		
		//DisableInput(vrController);
	}
}
#pragma endregion

void AGM_Versus::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

#pragma region player connection
void AGM_Versus::OnPostLogin(AController* player)
{
	ConnectedPlayers.Add(player);
}

void AGM_Versus::OnLogout(AController* player)
{
	ConnectedPlayers.Remove(player);
}

void AGM_Versus::GenericPlayerInitialization(AController* player)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player init: " + player->GetName()));
}

void AGM_Versus::HandleStartingNewPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("new player"));
}

void AGM_Versus::OnSwapPlayerController(APlayerController* newPlayer, APlayerController* oldController)
{
	ConnectedPlayers.Add(newPlayer);
}
#pragma endregion