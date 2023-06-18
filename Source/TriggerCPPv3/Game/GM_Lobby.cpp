// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Lobby.h"
#include <GameFramework/GameState.h>
#include <Kismet/GameplayStatics.h>
#include <TriggerCPPv3/Actors/Character/PC_VRPawn.h>

AGM_Lobby::AGM_Lobby() : Super() {
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = nullptr;
	static ConstructorHelpers::FClassFinder<APlayerController> PCPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PC_VRPawn'"));
	PlayerControllerClass = PCPawn.Class;
	static ConstructorHelpers::FClassFinder<APlayerState> PSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.PS_Menu'"));
	PlayerStateClass = PSPawn.Class;
	static ConstructorHelpers::FClassFinder<AGameState> GSPawn(TEXT("/Script/CoreUObject.Class'/Script/TriggerCPPv3.GS_Menu'"));
	GameStateClass = GSPawn.Class;
}

void AGM_Lobby::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), actors);
	for (auto actor: actors)
	{
		FreeSpawns.Push(Cast<APlayerStart>(actor));
	}

}
void AGM_Lobby::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}


#pragma region player connection
void AGM_Lobby::OnSwapPlayerController(APlayerController* newPlayer, APlayerController* oldController)
{
	ConnectedPlayers.Add(newPlayer);
}

void AGM_Lobby::OnPostLogin(AController* player)
{
	ConnectedPlayers.Add(player);

	CanPlay = ConnectedPlayers.Num() > 3 && ConnectedPlayers.Num() % 2 == 0;
}

void AGM_Lobby::OnLogout(AController* player)
{
	player->GetPawn()->Destroy();
	player->Destroy();
	ConnectedPlayers.Remove(player);
	CanPlay = ConnectedPlayers.Num() > 3 && ConnectedPlayers.Num() % 2 == 0;
}

void AGM_Lobby::GenericPlayerInitialization(AController* player)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player init: " + player->GetName()));

		FName CurrentPlayerType{ IsBandit ? "Bandit" : "Sheriff" };
		auto spawn = *FreeSpawns.FindByPredicate([&](const APlayerStart* start) {
			return start->PlayerStartTag == CurrentPlayerType;
		});
		IsBandit = !IsBandit;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.OverrideLevel = GetLevel();
		APC_VRPawn* vrController = GetWorld()->SpawnActor<APC_VRPawn>(APC_VRPawn::StaticClass(), spawn->GetActorLocation(), spawn->GetActorRotation(), SpawnInfo);
		if (vrController)
		{
			vrController->Possess(GetWorld()->SpawnActor<AP_VRPawn>());
		}

		FreeSpawns.Remove(spawn);
}
#pragma endregion