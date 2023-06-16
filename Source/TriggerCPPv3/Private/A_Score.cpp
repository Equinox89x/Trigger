// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Score.h"

// Sets default values
AA_Score::AA_Score()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScoreMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScoreMenu"));
	ScoreMenu->SetDrawSize(FVector2D(3000.f, 1080.f));
	ScoreMenu->SetGeometryMode(EWidgetGeometryMode::Cylinder);
	ScoreMenu->SetCylinderArcAngle(100);
	RootComponent = ScoreMenu;

	SetActorScale3D(FVector{ 0.2,0.2,0.2 });

}

// Called when the game starts or when spawned
void AA_Score::BeginPlay()
{
	Super::BeginPlay();
	
	//SetHidden(true);
}

// Called every frame
void AA_Score::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

