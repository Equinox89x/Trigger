// Fill out your copyright notice in the Description page of Project Settings.
//UNUSED-----DELETE

#include "A_Bullet.h"

// Sets default values
AA_Bullet::AA_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bullet"));
	bulletMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("bulletMesh"));

	RootComponent = bullet;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObj(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	bulletMesh = loadedObj.Object;

	bullet->SetStaticMesh(bulletMesh);
	bullet->SetRelativeScale3D(FVector(0.656250f, 0.656250f, 2.650000f));
	bullet->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AA_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

