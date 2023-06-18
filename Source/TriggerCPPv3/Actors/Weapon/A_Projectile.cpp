// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Projectile.h"
#include <TriggerCPPv3/Actors/Weapon/A_Weapon.h>
#include <TriggerCPPv3/Actors/Character/P_VRPawn.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AA_Projectile::AA_Projectile()
{
	bReplicates = true;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = bulletMesh;

	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> projectileMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/FPWeapon/Materials/FirstPersonProjectileMaterial.FirstPersonProjectileMaterial'"));
	//bulletMesh->SetMaterial(0, projectileMaterial.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObject(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	bulletMesh->SetStaticMesh(loadedObject.Object);
	bulletMesh->SetRelativeScale3D(FVector(0.011484f, 0.011484f, 0.046375f));

	bulletMesh->SetSimulatePhysics(true);
	bulletMesh->SetEnableGravity(false);

	bulletMesh->SetMassOverrideInKg("Name_None", 0.2f);

	//bind the eventhit
	bulletMesh->SetNotifyRigidBodyCollision(true);
	bulletMesh->OnComponentHit.AddDynamic(this, &AA_Projectile::EventHit);
	//bulletMesh->OnComponentBeginOverlap.AddDynamic(this, &AA_Projectile::EventOverlap);
}

// Called when the game starts or when spawned
void AA_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AA_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AA_Projectile::EventHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector nornalImpulse, const FHitResult& hit) {
	
	//destroy bullet if it hits a collectible and change collectible's color
	AA_Collectible* castedHit{ Cast<AA_Collectible>(otherActor) };
	if (castedHit) {
		float value{ castedHit->treasureValue };
		castedHit->GetStaticMeshComponent()->SetMaterial(0, colorfulMaterial);
		Destroy();
	}

	//Check if projectile overlaps with weapon
	//if so, attach the bullet into the first available weapon bullet socket
	AA_Weapon* castedHit2{ Cast<AA_Weapon>(otherActor) };
	if (castedHit2) {
		const USkeletalMeshSocket* socket{ castedHit2->GetCurrentSocket()};

		if (socket) {
			bulletMesh->SetSimulatePhysics(false);
			bulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			socket->AttachActor(this, castedHit2->weapon);
			bulletMesh->SetRelativeRotation(FRotator(90, 0, 0));
			//increase socket nr after firing so the next empty socket is always chosen.
			castedHit2->IncreaseSocketIndex();
			castedHit2->bullets.Add(this);
		
			//set grabbed item (which would be a bullet) from the pawn to nullptr after having it placed in the gun
			AP_VRPawn* pawn{ Cast<AP_VRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) };
			if (pawn) {
				AA_Projectile* rightItem{ Cast<AA_Projectile>(pawn->GetGrabbedItemRight()) };
				AA_Projectile* leftItem{ Cast<AA_Projectile>(pawn->GetGrabbedItemLeft()) };
				if (rightItem) { pawn->SetGrabbedItemRight(nullptr); }
				if (leftItem) { pawn->SetGrabbedItemLeft(nullptr); }
			}
		}
	}
}

//void AA_Projectile::EventOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hit)
//{
//	GLog->Log("Bullet hit something");
//
//	//dynamic cast to check for derived class type. ACPP_Collectible is derived of AStaticMeshActor
//	AA_Weapon* castedHit{ dynamic_cast<AA_Weapon*>(otherActor) };
//	if (castedHit) {
//		FName name{ "bulletSocket" + castedHit->socketIndex };
//		bulletMesh->AttachToComponent(castedHit->weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale, name);
//	}
//
//}
