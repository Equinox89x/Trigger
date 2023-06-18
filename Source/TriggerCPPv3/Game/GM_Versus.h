// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/PlayerStart.h>
#include <TriggerCPPv3/Actors/A_Spawn.h>
#include <TriggerCPPv3/Game/GI_Network.h>
#include "GM_Versus.generated.h"

/**
 * 
 */
UCLASS()
class TRIGGERCPPV3_API AGM_Versus : public AGameMode
{
	GENERATED_BODY()

public:
	AGM_Versus();

	void SpawnPlayer(APlayerController* controller);
	void Countdown();

	UFUNCTION(BlueprintCallable)
	AA_Spawn* GetChosenSpawn() const { return ChosenSpawn; };

	UGI_Network* GameInstance{ nullptr };

protected:
	virtual void Tick(float deltaSeconds) override;
	virtual void BeginPlay() override;

private:
	TArray<AController*> ConnectedPlayers{};

	TArray<AA_Spawn*> FreeSpawns{};
	AA_Spawn* ChosenSpawn{};
	TArray<APlayerStart*> UsedSpawns{};
	TArray<APlayerStart*> CurrentSpawn{};

	int matchNr{ 1 };

	void SetupSpawn(bool isNewGame = false);
	const FTransform FindPlayerStart();

	#pragma region login
	UFUNCTION()
		virtual void OnPostLogin(AController* player);
	UFUNCTION()
		virtual void OnSwapPlayerController(APlayerController* newPlayer, APlayerController* oldController);
	UFUNCTION()
		virtual void OnLogout(AController* player);

	UFUNCTION()
		virtual void GenericPlayerInitialization(AController* player);

	virtual void HandleStartingNewPlayer();
	#pragma endregion
};
