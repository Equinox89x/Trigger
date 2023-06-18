// Fill out your copyright notice in the Description page of Project Settings.

//PlayerState keeps track of replicated things of the player (like health)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_Menu.generated.h"

/**
 * 
 */
UCLASS()
class TRIGGERCPPV3_API APS_Menu : public APlayerState
{
	GENERATED_BODY()
	
public:

	APS_Menu();

	virtual void NotifyBeginPlay();

	float Score{ 0 };

	void SetScoreDetails(bool fullCombo, float drawSpeed, float averageShootSpeed, float averageAccuracy, float score, float speedMultiplier);

	UFUNCTION(BlueprintCallable)
		bool GetFullCombo() const { return FullCombo; }
	UFUNCTION(BlueprintCallable)
		float GetDrawSpeed() const { return DrawSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetAverageShootSpeed() const { return AverageShootSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetAverageAccuracy() const { return AverageAccuracy; }
	UFUNCTION(BlueprintCallable)
		float GetSpeedMultiplier() const { return SpeedMultiplier; }
	UFUNCTION(BlueprintCallable)
		float GetCombined() const { return Combined; }
	UFUNCTION(BlueprintCallable)
		float GetTotal() const { return Total; }	
	UFUNCTION(BlueprintCallable)
		bool GetIsGameEnded() const { return IsGameEnded; }

private:
	bool FullCombo{ false };
	float DrawSpeed{ 0 };
	float AverageShootSpeed{ 0 };
	float AverageAccuracy{ 0 };
	float SpeedMultiplier{ 0 };
	float Combined{ 0 };
	float Total{ 0 };

	bool IsGameEnded{ false };

};
