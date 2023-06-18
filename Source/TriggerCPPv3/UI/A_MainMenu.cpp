// Fill out your copyright notice in the Description page of Project Settings.


#include "A_MainMenu.h"

// Sets default values
AA_MainMenu::AA_MainMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	menu = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainMenu"));
	menu->SetDrawSize(FVector2D(3000.f, 1080.f));
	menu->SetGeometryMode(EWidgetGeometryMode::Cylinder);
	menu->SetCylinderArcAngle(100);
	RootComponent = menu;

	SetActorScale3D(FVector{ 0.2,0.2,0.2 });
	
	//HandleVersusSpawn();
}

void AA_MainMenu::HandleVersusSpawn()
{
	ChosenMap = MapNames[FMath::RandRange(0, MapNames.Num() - 1)];
	SelectedSpawnIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
	TArray<FVector2D> spawn{ SpawnLocations[SelectedSpawnIndex] };
	SelectedSpawnLocation1 = FVector2D(double(spawn[0].X / Ratio.X), double(spawn[0].Y / Ratio.Y));
	SelectedSpawnLocation2 = FVector2D(double(spawn[1].X / Ratio.X), double(spawn[1].Y / Ratio.Y));
	IsHorizontal = spawn[2].X;
}

// Called when the game starts or when spawned
void AA_MainMenu::BeginPlay()
{
	Super::BeginPlay();
	UGI_Network* gi{ Cast<UGI_Network>(GetWorld()->GetGameInstance()) };
	if (gi) {
		GameInstance = gi;
	}
}

// Called every frame
void AA_MainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Spawn Customisation (UI)
void AA_MainMenu::SetGameProperties(int distance, int maxBulletNr, int playerAmount)
{
	SelectedDistance = distance * Ratio.Y;
	MaxBullets = maxBulletNr;
	PlayerAmount = playerAmount;

	if (GameInstance) {
		GameInstance->SelectedSpawnIndex = SelectedSpawnIndex;
		GameInstance->Distance = SelectedDistance;
		GameInstance->MaxBullets = MaxBullets;
		GameInstance->PlayerAmount = PlayerAmount;
	}
}

TArray<double> AA_MainMenu::GetRecalculatedDistance()
{
	return TArray<double>{
		DefaultDistance / Ratio.Y,
		MaxDistance / Ratio.Y,
		MinDistance / Ratio.Y,
		double(Ratio.Y),
		IsHorizontal
	};
}
#pragma endregion

