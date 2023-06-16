// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "XRMotionControllerBase.h"
#include <Components/TextRenderComponent.h>
#include "Kismet/GameplayStatics.h"
//#include "Navmesh/RecastNavMesh.h"
#include "A_Projectile.h"
#include "A_Collectible.h"
#include "A_Weapon.h"
#include "PS_Menu.h"
#include <Components/WidgetInteractionComponent.h>
#include <Components/BoxComponent.h>
#include <GI_Network.h>
#include <GM_Versus.h>
#include "P_VRPawn.generated.h"

UCLASS()
class TRIGGERCPPV3_API AP_VRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AP_VRPawn();

	bool didWin{ false };

	#pragma region VR
	//uproperty says it should be visible in the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	USceneComponent* VRTrackingCenter { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	UCameraComponent* Head { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	UMotionControllerComponent* LeftController { nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	UMotionControllerComponent* RightController { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	UWidgetInteractionComponent* LeftInteraction {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	UWidgetInteractionComponent* RightInteraction {};


	UPROPERTY(EditAnywhere, Category = Meshes)
	UStaticMeshComponent* LeftMesh;
	UPROPERTY(EditAnywhere, Category = Meshes)
	UStaticMeshComponent* RightMesh;	
	UPROPERTY(EditAnywhere, Category = Meshes)
	UStaticMeshComponent* LeftBlockMesh;
	UPROPERTY(EditAnywhere, Category = Meshes)
	UStaticMeshComponent* RightBlockMesh;
	#pragma endregion

	#pragma region Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		USkeletalMeshComponent* CharacterMesh;

	UPROPERTY()
		UBoxComponent* HeadCollision;
	UPROPERTY()
		UBoxComponent* NeckCollision;
	UPROPERTY()
		UBoxComponent* HeartCollision;
	UPROPERTY()
		UBoxComponent* RightKneeCollision;
	UPROPERTY()
		UBoxComponent* LeftKneeCollision;
	UPROPERTY()
		UBoxComponent* LeftElbowCollision;
	UPROPERTY()
		UBoxComponent* RightElbowCollision;
	UPROPERTY()
		UBoxComponent* LeftHandCollision;
	UPROPERTY()
		UBoxComponent* RightHandCollision;
	UPROPERTY()
		UBoxComponent* LeftKidneyCollision;
	UPROPERTY()
		UBoxComponent* RightKidneyCollision;
	UPROPERTY()
		UBoxComponent* DickCollision;

	UFUNCTION()
	void BodyPartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	int ShotsFired{ 0 };
	float Score{ 0 };
	#pragma endregion

	#pragma region Belt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Belt)
	UStaticMeshComponent* Belt { nullptr };
	UPROPERTY()
	UStaticMeshComponent* RightHolster { nullptr };
	UPROPERTY()
	UStaticMeshComponent* LeftHolster { nullptr };
	UPROPERTY()
	UBoxComponent* RightBeltOverlap { nullptr };
	UPROPERTY()
	UBoxComponent* LeftBeltOverlap { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Belt)
	UStaticMeshComponent* RightBeltSocket { nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Belt)
	UStaticMeshComponent* LeftBeltSocket { nullptr };

	UFUNCTION()
		void HolsterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	#pragma endregion

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = __hide)
	//	ARecastNavMesh* navmesh { nullptr };

	#pragma region properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		bool isMenu{ true };
	#pragma endregion

	AA_Spawn* OwningSpawn{ nullptr };

	void Reload(bool isRightHanded);
	void ShootGun(bool isRightHanded);
	void ShootGunReleased(bool isRightHanded) const;
	void LetGo(bool isRightController);
	void GrabPressed(bool isRightController);

	bool IsGunRight() const {
		if (GunRight)return true;
		return false;
	};
	AActor* GetGrabbedItemLeft() { return GrabbedItemLeft; }
	AActor* GetGrabbedItemRight() { return GrabbedItemRight; }	
	void SetGrabbedItemLeft(AActor* item) { GrabbedItemLeft = item; }
	void SetGrabbedItemRight(AActor* item) { GrabbedItemRight = item; }
	void SetPlayerScore(bool didHaveMaxBullets);

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	APS_Menu* PlayerState{ nullptr };
	AGM_Versus* GameMode{ nullptr };

	float reloadTimer{ 0 };
	bool wasPressed{ false };
	int MaxBullets{ 0 };
	float DrawSpeedTimer{ 0 };
	float ShootSpeedTimer{ 0 };
	float MatchTimer{ 0 };
	float ShotAccuracy{ 0 };

	double CurrentSmallestValue{ 99999999.0 };
	float MaxDistanceScore{ 1000 };
	UBoxComponent* ClosestWeakpoint{};
	TArray<UBoxComponent*> Collisions;

	#pragma region items
	UPROPERTY()
	AA_Weapon* GunLeft { nullptr };
	UPROPERTY()
	AA_Weapon* GunRight { nullptr };

	UPROPERTY()
	AActor* GrabbedItemLeft { nullptr };
	UPROPERTY()
	AActor* GrabbedItemRight { nullptr };
	#pragma endregion

	#pragma region Helper functions
	void HandleBonePhysicsSimulation(AA_Weapon* weaponCast, FName name, bool shouldSimulate);
	void HandleAttach(AActor* actor, UStaticMeshComponent* mesh, bool shouldAttach, UMotionControllerComponent* controller = nullptr) const;
	void HandleAttach(AActor* actor, USkeletalMeshComponent* mesh, bool shouldAttach, UMotionControllerComponent* controller = nullptr) const;

	void SpawnBullet(AA_Weapon* weapon)  const;
	void InitWeaponOnBelt();
	UBoxComponent* GetClosestWeakpoint(AActor* projectile);
	#pragma endregion
};