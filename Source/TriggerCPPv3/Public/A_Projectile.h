// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_Collectible.h"
#include "A_Projectile.generated.h"

class AP_VRPawn;
UCLASS()
class TRIGGERCPPV3_API AA_Projectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AA_Projectile();

    UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Projectile)
        UStaticMeshComponent* bulletMesh {nullptr};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
        UMaterial* colorfulMaterial {nullptr};

    //UFUNCTION()
    //    void EventOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

    //Ufunction here is to tell editor the eventhit function in the engine has to be overridden by this one
    UFUNCTION()
        void EventHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector nornalImpulse, const FHitResult& hit);

    UFUNCTION(BlueprintCallable)
    void SetIsActive(bool isActive) { IsActive = isActive; }
    bool GetIsActive() { return IsActive; }

    AP_VRPawn* GetPlayer() { return Player; };
    void SetPlayer(AP_VRPawn* player) { Player = player; };

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    bool IsActive{ false };
    AP_VRPawn* Player;
};