// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include <GI_Network.h>
#include "A_MainMenu.generated.h"

UCLASS()
class TRIGGERCPPV3_API AA_MainMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	AA_MainMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	UWidgetComponent* menu { nullptr };
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector2D> GetMapInfo() { return TArray<FVector2D>{ SelectedSpawnLocation1, SelectedSpawnLocation2}; };
	UFUNCTION(BlueprintCallable)
	const FString GetMapName() { return ChosenMap; };

	UFUNCTION(BlueprintCallable)
	void SetGameProperties(int distance, int maxBulletNr, int playerAmount);

	UFUNCTION(BlueprintCallable)
	TArray<double> GetRecalculatedDistance();	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	#pragma region Predefined Spawn locations
	const float DefaultDistance{ 600 }; //spawn distance between 2 players
	const float MaxDistance{ DefaultDistance + 300 };
	const float MinDistance{ DefaultDistance - 300 };

	//Spawn locations from the L_Western map
	const TArray<TArray<FVector2D>> SpawnLocations{ //last spawn comes first
		TArray<FVector2D>{ {930, 850 - DefaultDistance / 2}, {930, 850 + DefaultDistance / 2},       {1,1} }, //first 2 values are location, last fvector2d is 1 or 0 based on if it's a horizontally aligned spawn or not
		TArray<FVector2D>{ {1750 - DefaultDistance / 2,2750 }, {1750 + DefaultDistance / 2, 2750 },  {1,1} },
		TArray<FVector2D>{ {2180, 750 - DefaultDistance / 2 }, {2180, 750 + DefaultDistance / 2},    {1,1} },
		TArray<FVector2D>{ {1000, 2760 - DefaultDistance / 2}, {1000, 2760 + DefaultDistance / 2},   {1,1} },
		TArray<FVector2D>{ {1200, 2760 - DefaultDistance / 2}, {1200, 2260 + DefaultDistance / 2},   {1,1} }, 
		TArray<FVector2D>{ {1400, 2760 - DefaultDistance / 2}, {1400, 2460 + DefaultDistance / 2},   {1,1} } 
	};
	const FVector2D MapSize{ 3000,3500 };
	const FVector2D TargetSize{ 872,872 }; //size of Image in W_MainMenu (Third Section)
	//const FVector2D Correction{ 25, 20 };
	const FVector2D Ratio{ MapSize / TargetSize }; //size difference of the target and map size in %
	const TArray<FString> MapNames{ "L_Western" };
	#pragma endregion


	int MaxBullets{ 3 }; //max bullets to be fired in a match
	int PlayerAmount{ 2 }; //required amount of players in a match
	FVector2D SelectedSpawnLocation1; //spawn location scaled down to UI mapsize
	FVector2D SelectedSpawnLocation2; //spawn location scaled down to UI mapsize
	float SelectedDistance{ DefaultDistance }; //How far the spawns should be from eachother (modifier)
	double IsHorizontal;
	FString ChosenMap;

	int SelectedSpawnIndex; //index to know which spawn to pick in ThreeBulletsGM

	UGI_Network* GameInstance{ nullptr };

	void HandleVersusSpawn();
};
