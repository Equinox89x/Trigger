//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include <Components/BoxComponent.h>
//#include <PS_Menu.h>
//#include "AC_BodyCollisionComponent.generated.h"
//
//
//class AP_VRPawn;
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class TRIGGERCPPV3_API UAC_BodyCollisionComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	// Sets default values for this component's properties
//	UAC_BodyCollisionComponent();
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
//		USkeletalMeshComponent* CharacterMesh;
//
//	UPROPERTY()
//		UBoxComponent* HeadCollision;
//	UPROPERTY()
//		UBoxComponent* NeckCollision;
//	UPROPERTY()
//		UBoxComponent* HeartCollision;
//	UPROPERTY()
//		UBoxComponent* RightKneeCollision;
//	UPROPERTY()
//		UBoxComponent* LeftKneeCollision;
//	UPROPERTY()
//		UBoxComponent* LeftElbowCollision;
//	UPROPERTY()
//		UBoxComponent* RightElbowCollision;
//	UPROPERTY()
//		UBoxComponent* LeftHandCollision;
//	UPROPERTY()
//		UBoxComponent* RightHandCollision;
//	UPROPERTY()
//		UBoxComponent* LeftKidneyCollision;
//	UPROPERTY()
//		UBoxComponent* RightKidneyCollision;
//	UPROPERTY()
//		UBoxComponent* DickCollision;
//
//	UFUNCTION()
//		void BodyPartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
//	UFUNCTION()
//		void PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
//	
//	float Score{ 0 };
//	int ShotsFired{ 0 };
//
//	void SetPlayerScore(bool didHaveMaxBullets);
//
//
//protected:
//	// Called when the game starts
//	virtual void BeginPlay() override;
//
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//private:
//	APS_Menu* PlayerState{ nullptr };
//
//	float reloadTimer{ 0 };
//	bool wasPressed{ false };
//	int MaxBullets{ 0 };
//	float DrawSpeedTimer{ 0 };
//	float ShootSpeedTimer{ 0 };
//	float MatchTimer{ 0 };
//	float ShotAccuracy{ 0 };
//
//	double CurrentSmallestValue{ 99999999.0 };
//	float MaxDistanceScore{ 1000 };
//	UBoxComponent* ClosestWeakpoint{};
//	TArray<UBoxComponent*> Collisions;
//	UBoxComponent* GetClosestWeakpoint(AActor* projectile);
//};
