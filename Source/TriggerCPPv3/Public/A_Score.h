// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include <GI_Network.h>
#include "A_Score.generated.h"

UCLASS()
class TRIGGERCPPV3_API AA_Score : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_Score();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	UWidgetComponent* ScoreMenu { nullptr };

	UGI_Network* GameInstance{ nullptr };

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
