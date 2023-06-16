// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetInteractionComponent.h>
#include <Components/ArrowComponent.h>
#include "Engine/SkeletalMeshSocket.h"
#include <A_Projectile.h>
#include "MotionControllerComponent.h"
#include <Components/BoxComponent.h>
#include "A_Weapon.generated.h"

UCLASS()
class TRIGGERCPPV3_API AA_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_Weapon();

	const USkeletalMeshSocket* GetCurrentSocket() const;

	void HandleAttachment(USceneComponent* component);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* Scene;		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		USkeletalMeshComponent* weapon;		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		USkeletalMesh* Revolver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		UWidgetInteractionComponent* interactionComp {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		UArrowComponent* arrow;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		TArray<AA_Projectile*> bullets{ TArray<AA_Projectile*>{} };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
		UMotionControllerComponent* MotionController;

	void SetIsHeld(bool isheld) { isHeld = isheld; }
	void SetCanLock(bool canlock) { canLock = canlock; }
	void IncreaseSocketIndex() { ++socketIndex; }
	void Shoot(AP_VRPawn* player);
	void SetPlayer(AP_VRPawn* player) { Player = player; };


protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
		UArrowComponent* HammerArrow;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
		UBoxComponent* HammerEnterCollision;		
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
		UBoxComponent* HammerLeaveCollision;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
		UStaticMeshComponent* HammerMesh;		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
		UStaticMeshComponent* Hammer;

	bool GetIsHeld() const { return canLock; }
	bool GetCanLock() const { return isHeld; }
	int GetSocketIndex() const { return isHeld; }

private:
	FTransform HammerInitialRelativeTransform{};
	FTransform HammerInitialWorldTransform{};
	bool canShoot{ false };
	bool isTracking{ false };
	int socketIndex{ 1 };
	bool isHeld{ false };
	bool canLock = false;
	AP_VRPawn* Player;

	void HandleTriggerMovement();
	void ReturnTrigger();

	UFUNCTION()
	void HammerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void HammerLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void HandLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void LockBodies();
};
