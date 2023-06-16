// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "A_Collectible.generated.h"

/**
 *
 */
UCLASS()
class TRIGGERCPPV3_API AA_Collectible : public AStaticMeshActor
{
    GENERATED_BODY()

public:

    AA_Collectible();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collectible)
    float treasureValue{ 0.f };
};