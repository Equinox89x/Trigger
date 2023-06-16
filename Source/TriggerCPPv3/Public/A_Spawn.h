// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameFramework/PlayerStart.h>
#include <Components/WidgetComponent.h>
#include <Net/UnrealNetwork.h>
#include <Components/TextRenderComponent.h>
#include <GI_Network.h>
#include <Components/BoxComponent.h>
#include "A_Spawn.generated.h"

class AA_Projectile;
class AP_VRPawn;
UCLASS()
class TRIGGERCPPV3_API AA_Spawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AA_Spawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	UBoxComponent* Overlapper {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	UStaticMeshComponent* Blocker {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	APlayerStart* Spawn1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	APlayerStart* Spawn2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Timer)
	UTextRenderComponent* TimerFront {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Timer)
	UTextRenderComponent* TimerBack {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	UChildActorComponent* Spawn1Comp {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	UChildActorComponent* Spawn2Comp { nullptr };

	UFUNCTION(NetMulticast, Unreliable)
	void SetSeconds(int seconds);
	void Countdown();
	void EndMatch();

	const int GetElapsedTime() { return ElapsedTime; }
	const int GetSeconds() { return Seconds; }
	void RegisterBullet(AA_Projectile* projectile);

	void SetPlayer(AP_VRPawn* player);

	UGI_Network* GameInstance{ nullptr };

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:

	int Seconds{ 15 }, ElapsedTime{ 0 };

	int Player1Bullets{ 0 }, Player2Bullets{ 0 };
	AP_VRPawn* Player1, *Player2;

	float distance{ 600 }, OriginalTime;

	USceneComponent* Comp{ nullptr };

	//UFUNCTION()
	//void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void RemoveScoreScreen();

};
