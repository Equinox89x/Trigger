// Fill out your copyright notice in the Description page of Project Settings.


//Player Controller keeps track of owning client and server + the pawn
#pragma once

#include "CoreMinimal.h"
#include "TriggerCPPv3/Game/GM_Menu.h"
#include <TriggerCPPv3/Game/GM_Versus.h>
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include <TriggerCPPv3/Actors/Character/P_VRPawn.h>
#include "PC_VRPawn.generated.h"

/**
 * 
 */
UCLASS()
class TRIGGERCPPV3_API APC_VRPawn : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float MoveSpeed{ 2.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float RotationSpeed{ 3.f };

#pragma region input actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* GrabRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* GrabLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* Movement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* TriggerRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* TriggerLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* Rotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* ReloadLeftKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputAction* ReloadRightKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputActions)
		UInputMappingContext* inputMappingContext;
#pragma endregion

	APC_VRPawn();

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void SRSpawnPlayer();

	virtual void AcknowledgePossession(class APawn* P);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	AP_VRPawn* Player{ nullptr };


	//Input Action Functions
#pragma region Shooting
	UFUNCTION(BlueprintCallable)
		void ShootGunLeft();
	UFUNCTION(BlueprintCallable)
		void ShootGunRight();
	UFUNCTION(BlueprintCallable)
		void ShootGunReleasedLeft();
	UFUNCTION(BlueprintCallable)
		void ShootGunReleasedRight();
#pragma endregion

#pragma region grabbing
	UFUNCTION(BlueprintCallable)
		void GrabRightPressed();
	UFUNCTION(BlueprintCallable)
		void GrabRightReleased();
	UFUNCTION(BlueprintCallable)
		void GrabLeftPressed();
	UFUNCTION(BlueprintCallable)
		void GrabLeftReleased();
#pragma endregion

#pragma region reloading
	UFUNCTION(BlueprintCallable)
		void ReloadLeft();
	UFUNCTION(BlueprintCallable)
		void ReloadRight();
#pragma endregion

	void Move(const FInputActionValue& value);
	void Turn(const FInputActionValue& value);
};
