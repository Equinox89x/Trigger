// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include <GameFramework/PlayerStart.h>
#include "GM_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class TRIGGERCPPV3_API AGM_Lobby : public AGameMode
{
	GENERATED_BODY()

public:
	AGM_Lobby();

protected:
	virtual void Tick(float deltaSeconds) override;
	virtual void BeginPlay() override;

private:
	TArray<AController*> ConnectedPlayers{};
	TArray<APlayerStart*> FreeSpawns{};
	bool IsBandit{ false };
	bool CanPlay{ false };

	UFUNCTION()
		virtual void OnPostLogin(AController* player);
	UFUNCTION()
		virtual void OnSwapPlayerController(APlayerController* newPlayer, APlayerController* oldController);
	UFUNCTION()
		virtual void OnLogout(AController* player);
	UFUNCTION()
		virtual void GenericPlayerInitialization(AController* player);
};
