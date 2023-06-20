// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include <Components/SphereComponent.h>
#include "A_StartButton.generated.h"

UCLASS()
class TRIGGERCPPV3_API AA_StartButton : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	USceneComponent* Scene { nullptr };	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	UWidgetComponent* button { nullptr };	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	UWidgetComponent* button2 { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	USphereComponent* Overlapper { nullptr };

	// Sets default values for this actor's properties
	AA_StartButton();

	void SetStartBtnActive(bool isActive);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
