// Fill out your copyright notice in the Description page of Project Settings.
//UNUSED-----DELETE

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include <P_VRPawn.h>
#include "A_Bullet.generated.h"

UCLASS()
class TRIGGERCPPV3_API AA_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_Bullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
	UStaticMeshComponent* bullet {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
	UStaticMesh* bulletMesh {};

	AP_VRPawn* GetPlayer() { return Player; };
	void SetPlayer(AP_VRPawn* player) { Player = player; };
private:
	AP_VRPawn* Player;
};
