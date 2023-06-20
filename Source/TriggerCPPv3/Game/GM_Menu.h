// Fill out your copyright notice in the Description page of Project Settings.

//GameMode is server; keeps track of everything for everyone
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/PlayerStart.h>
#include "GM_Menu.generated.h"

/**
 * 
 */
UCLASS()
class TRIGGERCPPV3_API AGM_Menu : public AGameMode
{
	GENERATED_BODY()

public:
	AGM_Menu();

	TArray<FName> sessions{};

protected:
	virtual void BeginPlay();
	virtual void Tick(float deltaSeconds);

private:

	TArray<AController*> ConnectedPlayers{};
	TArray<APlayerStart*> FreeSpawns{};
	TArray<APlayerStart*> UsedSpawns{};

	UFUNCTION()
		virtual void OnPostLogin(AController* player);

	UFUNCTION()
		virtual void OnSwapPlayerController(APlayerController* newPlayer, APlayerController* oldController);

	UFUNCTION()
		virtual void OnLogout(AController* player);

	UFUNCTION()
		virtual void GenericPlayerInitialization(AController* player);

	virtual void HandleStartingNewPlayer();

};
