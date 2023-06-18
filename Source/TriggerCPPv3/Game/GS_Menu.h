// Fill out your copyright notice in the Description page of Project Settings.


//GameState keeps track of data for all clients (like score) (all clients and server can access this) 
//Gamestate is owned by gamemode and will be destroyed when going to another.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include <Net/UnrealNetwork.h>
#include "GS_Menu.generated.h"

/**
 * 
 */
UCLASS()
class TRIGGERCPPV3_API AGS_Menu : public AGameState
{
	GENERATED_BODY()

public:

	AGS_Menu();

protected:
	virtual void NotifyBeginPlay();

	//void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
};
