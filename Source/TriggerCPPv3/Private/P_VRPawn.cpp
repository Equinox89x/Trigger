// Fill out your copyright notice in the Description page of Project Settings.


#include "P_VRPawn.h"
#include <HeadMountedDisplayFunctionLibrary.h>
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include <A_MainMenu.h>
#include <GM_Menu.h>
#include <GS_Menu.h>
#include <A_Score.h>

// Sets default values
AP_VRPawn::AP_VRPawn()
{
	//bReplicates = true;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bNetLoadOnClient = true;
	SetReplicateMovement(true);

	#pragma region Instantiating
	//Create the objects in the class
	VRTrackingCenter = CreateDefaultSubobject<USceneComponent>(TEXT("VRTrackingCenter"));
	Head = CreateDefaultSubobject<UCameraComponent>(TEXT("Head"));

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));

	LeftInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftInteraction"));
	RightInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightInteraction"));

	LeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftMesh"));
	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightMesh"));	
	LeftBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBlockMesh"));
	RightBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBlockMesh"));

	Belt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Belt"));
	RightHolster = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHolster"));
	LeftHolster = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHolster"));
	RightBeltOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBeltOverlap"));
	LeftBeltOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBeltOverlap"));
	RightBeltSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBeltSocket"));
	LeftBeltSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBeltSocket"));

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character"));
	HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollision"));
	NeckCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("NeckCollision"));
	HeartCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeartCollision"));
	RightKneeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightKneeCollision"));
	LeftKneeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftKneeCollision"));
	LeftElbowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftElbowCollision"));
	RightElbowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightElbowCollision"));
	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollision"));
	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollision"));
	LeftKidneyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftKidneyCollision"));
	RightKidneyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightKidneyCollision"));
	DickCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DickCollision"));
	#pragma endregion
	
	#pragma region Parenting
	//make the parenting structure
	Head->SetupAttachment(VRTrackingCenter);

	LeftController->SetupAttachment(VRTrackingCenter);
	RightController->SetupAttachment(VRTrackingCenter);

	LeftMesh->SetupAttachment(LeftController);
	RightMesh->SetupAttachment(RightController);	
	LeftBlockMesh->SetupAttachment(LeftController);
	RightBlockMesh->SetupAttachment(RightController);
	LeftInteraction->SetupAttachment(LeftController);
	RightInteraction->SetupAttachment(RightController);
	
	Belt->SetupAttachment(VRTrackingCenter);
	RightHolster->SetupAttachment(Belt);
	LeftHolster->SetupAttachment(Belt);
	RightBeltOverlap->SetupAttachment(RightHolster);
	LeftBeltOverlap->SetupAttachment(LeftHolster);
	RightBeltSocket->SetupAttachment(RightHolster);
	LeftBeltSocket->SetupAttachment(LeftHolster);

	CharacterMesh->SetupAttachment(VRTrackingCenter);
	HeadCollision->SetupAttachment(CharacterMesh);
	NeckCollision->SetupAttachment(CharacterMesh);
	HeartCollision->SetupAttachment(CharacterMesh);
	RightKneeCollision->SetupAttachment(CharacterMesh);
	LeftKneeCollision->SetupAttachment(CharacterMesh);
	LeftElbowCollision->SetupAttachment(CharacterMesh);
	RightElbowCollision->SetupAttachment(CharacterMesh);
	LeftHandCollision->SetupAttachment(CharacterMesh);
	RightHandCollision->SetupAttachment(CharacterMesh);
	LeftKidneyCollision->SetupAttachment(CharacterMesh);
	RightKidneyCollision->SetupAttachment(CharacterMesh);
	DickCollision->SetupAttachment(CharacterMesh);

	Head->bLockToHmd = true;
	this->RootComponent = VRTrackingCenter;

	#pragma endregion

	#pragma region VR
	//load meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cone(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	LeftMesh->SetStaticMesh(cone.Object);
	RightMesh->SetStaticMesh(cone.Object);

	//load material
	static ConstructorHelpers::FObjectFinder<UMaterial> solid(TEXT("/Script/Engine.Material'/Game/LevelPrototyping/Materials/M_Solid.M_Solid'"));
	LeftMesh->SetMaterial(0, solid.Object);
	RightMesh->SetMaterial(0, solid.Object);

	LeftBlockMesh->SetStaticMesh(cone.Object);
	RightBlockMesh->SetStaticMesh(cone.Object);

	//LeftBlockMesh->SetVisibility(false);
	//RightBlockMesh->SetVisibility(false);

	//tell which hand is which
	RightController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	LeftController->MotionSource = FXRMotionControllerBase::LeftHandSourceId;

	LeftInteraction->TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	RightInteraction->TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	RightInteraction->PointerIndex = 1;
	LeftInteraction->bShowDebug = true;
	RightInteraction->bShowDebug = true;
	LeftInteraction->InteractionDistance = 1500;
	RightInteraction->InteractionDistance = 1500;

	//transform
	LeftMesh->SetRelativeRotation(FRotator(-90, 180, 540));
	LeftMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	LeftBlockMesh->SetRelativeRotation(FRotator(-90, 180, 540));
	LeftBlockMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	RightMesh->SetRelativeRotation(FRotator(-90, 180, 180));
	RightMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	RightBlockMesh->SetRelativeRotation(FRotator(-90, 180, 180));
	RightBlockMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	//collisions
	LeftMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftBlockMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightBlockMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//set meshes to overlap collision
	LeftMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	LeftBlockMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	RightBlockMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);


	//LeftMesh->SetCollisionProfileName(FName{ "PhysicsActor" });
	//RightMesh->SetCollisionProfileName(FName{ "PhysicsActor" });
	#pragma endregion

	#pragma region Belt
	static ConstructorHelpers::FObjectFinder<UStaticMesh> belt(TEXT("/Script/Engine.StaticMesh'/Game/Characters/VRHands/Belt_pTorus1.Belt_pTorus1'"));
	Belt->SetStaticMesh(belt.Object);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> lHolster(TEXT("/Script/Engine.StaticMesh'/Game/Characters/VRHands/Belt_polySurface11.Belt_polySurface11'"));
	RightHolster->SetStaticMesh(lHolster.Object);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> rHolster(TEXT("/Script/Engine.StaticMesh'/Game/Characters/VRHands/Belt_polySurface1.Belt_polySurface1'"));
	LeftHolster->SetStaticMesh(rHolster.Object);

	Belt->SetRelativeLocation(FVector(0, 0, 75));
	Belt->SetRelativeRotation(FRotator(0, 180, 0));
	Belt->SetWorldScale3D(FVector(0.015f, 0.015f, 0.015f));

	LeftBeltOverlap->SetRelativeLocation(FVector(-58.924389, 1908.278371, 186));
	LeftBeltOverlap->SetRelativeScale3D(FVector(10.468744, 5.156250, 8.999999));
	LeftBeltOverlap->SetRelativeRotation(FRotator(-70, 0, 0));

	RightBeltOverlap->SetRelativeLocation(FVector(-58.924393, -1908.278371, 186));
	RightBeltOverlap->SetRelativeScale3D(FVector(10.468744, 5.156250, 8.999999));
	RightBeltOverlap->SetRelativeRotation(FRotator(-70, 0, 0));

	RightBeltSocket->SetRelativeLocation(FVector(-16.252785, -1849.112689, -350));
	RightBeltSocket->SetRelativeRotation(FRotator(-70, 0, 0));
	LeftBeltSocket->SetRelativeLocation(FVector(-16.252785, 1849.112689, -350));
	LeftBeltSocket->SetRelativeRotation(FRotator(-70, 0, 0));

	LeftBeltSocket->SetVisibility(false);
	RightBeltSocket->SetVisibility(false);

	LeftBeltOverlap->SetGenerateOverlapEvents(true);
	RightBeltOverlap->SetGenerateOverlapEvents(true);
	LeftBeltOverlap->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::HolsterOverlap);
	RightBeltOverlap->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::HolsterOverlap);
	#pragma endregion

	//navmesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
	//navmesh->AgentRadius = 60.f;

	#pragma region Character
	CharacterMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CharacterMesh->SetGenerateOverlapEvents(true);
	HeadCollision->SetGenerateOverlapEvents(true);
	NeckCollision->SetGenerateOverlapEvents(true);
	HeartCollision->SetGenerateOverlapEvents(true);
	RightKneeCollision->SetGenerateOverlapEvents(true);
	LeftKneeCollision->SetGenerateOverlapEvents(true);
	LeftElbowCollision->SetGenerateOverlapEvents(true);
	RightElbowCollision->SetGenerateOverlapEvents(true);
	LeftHandCollision->SetGenerateOverlapEvents(true);
	RightHandCollision->SetGenerateOverlapEvents(true);
	LeftKidneyCollision->SetGenerateOverlapEvents(true);
	RightKidneyCollision->SetGenerateOverlapEvents(true);
	DickCollision->SetGenerateOverlapEvents(true);

	HeadCollision->SetRelativeLocation(FVector(0.574568, 0.000000, 647.658561));
	NeckCollision->SetRelativeLocation(FVector(0.574568, 0.000000, 573.812407));
	HeartCollision->SetRelativeLocation(FVector(32.574568, 0.000000, 507.350869));
	RightKneeCollision->SetRelativeLocation(FVector(37.497645, 0.000000, 177.504715));
	LeftKneeCollision->SetRelativeLocation(FVector(-36.348509, 0.000000, 177.504715));
	LeftElbowCollision->SetRelativeLocation(FVector(-201.271586, 0.000000, 529.504715));
	RightElbowCollision->SetRelativeLocation(FVector(195.036106, 0.000000, 529.504715));
	LeftHandCollision->SetRelativeLocation(FVector(-361.271586, 0.000000, 529.504715));
	RightHandCollision->SetRelativeLocation(FVector(359.959183, 0.000000, 529.504715));
	LeftKidneyCollision->SetRelativeLocation(FVector(-51.117740, 0.000000, 394.120099));
	RightKidneyCollision->SetRelativeLocation(FVector(52.266875, 0.000000, 394.120099));
	DickCollision->SetRelativeLocation(FVector(0.574567, 0.000000, 335.043176));

	CharacterMesh->SetRelativeScale3D(FVector(0.29, 0.29, 0.29));
	CharacterMesh->SetRelativeRotation(FRotator(0, -90, 0));

	HeadCollision->SetRelativeScale3D(FVector(1.078189, 3.469530, 0.658177));
	NeckCollision->SetRelativeScale3D(FVector(1.078189, 3.469530, 0.314427));
	HeartCollision->SetRelativeScale3D(FVector(1.078189, 3.469530, 0.658177));
	RightKneeCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
	LeftKneeCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
	LeftElbowCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
	RightElbowCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
	LeftHandCollision->SetRelativeScale3D(FVector(1.515689, 3.469530, 0.658177));
	RightHandCollision->SetRelativeScale3D(FVector(1.515689, 3.469530, 0.658177));
	LeftKidneyCollision->SetRelativeScale3D(FVector(0.484439, 3.469530, 0.345677));
	RightKidneyCollision->SetRelativeScale3D(FVector(0.484439, 3.469530, 0.345677));
	DickCollision->SetRelativeScale3D(FVector(0.484439, 3.469530, 1.126927));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> charObj(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Cowboy/source/PlayerRigged__2_.PlayerRigged__2_'"));
	CharacterMesh->SetSkeletalMeshAsset(charObj.Object);
	static ConstructorHelpers::FObjectFinder<UMaterial> charMat(TEXT("/Script/Engine.Material'/Game/Characters/Cowboy/source/Material_001.Material_001'"));
	CharacterMesh->SetMaterial(0, charMat.Object);

	HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	NeckCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	HeartCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	RightKneeCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	LeftKneeCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	LeftElbowCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	RightElbowCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	LeftKidneyCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	RightKidneyCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);
	DickCollision->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::BodyPartOverlap);

	CharacterMesh->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::PlayerOverlap);
	//CharacterMesh->OnComponentBeginOverlap.AddDynamic(this, &AP_VRPawn::PlayerOverlap);
	//CharacterMesh->OnComponentHit.AddDynamic(this, &AP_VRPawn::PlayerHit);

	Collisions = { HeadCollision, NeckCollision, HeartCollision, RightKneeCollision, LeftKneeCollision, LeftElbowCollision, RightElbowCollision, LeftHandCollision, RightHandCollision, LeftKidneyCollision, RightKidneyCollision, DickCollision };
	#pragma endregion

}

void AP_VRPawn::SetPlayerScore(bool didHaveMaxBullets)
{
	if (PlayerState) PlayerState->Score += Score;
	if (PlayerState) PlayerState->SetScoreDetails(didHaveMaxBullets, DrawSpeedTimer, ShootSpeedTimer, ShotAccuracy, Score, didHaveMaxBullets ? 1 : MatchTimer);
}

// Called when the game starts or when spawned
void AP_VRPawn::BeginPlay()
{
	Super::BeginPlay();

	//set head tracking to not be in the floor
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	UGI_Network* gi{ Cast<UGI_Network>(GetWorld()->GetGameInstance()) };
	if (gi) MaxBullets = gi->MaxBullets;

	APS_Menu* ps{ Cast<APS_Menu>(GetPlayerState()) };
	if (ps) PlayerState = ps;

	AGM_Versus* gm{ Cast<AGM_Versus>(GetWorld()->GetAuthGameMode()) };
	if(gm) GameMode = gm;

	CharacterMesh->SetOwnerNoSee(true);

	InitWeaponOnBelt();
}

#pragma region Init
void AP_VRPawn::InitWeaponOnBelt()
{
	AA_Weapon* weapon{ GetWorld()->SpawnActor<AA_Weapon>(AA_Weapon::StaticClass()) };
	weapon->HandleAttachment(RightBeltSocket);
	MaxBullets = Cast<UGI_Network>(GetGameInstance())->MaxBullets;
	for (size_t i = 0; i < MaxBullets; i++)
	{
		SpawnBullet(weapon);
	}
}

void AP_VRPawn::SpawnBullet(AA_Weapon* weapon) const
{
	//spawn bullet
	AA_Projectile* bullet{ Cast<AA_Projectile>(GetWorld()->SpawnActor(AA_Projectile::StaticClass())) };
	//get current bullet socket of the weapon
	const USkeletalMeshSocket* socket{ weapon->GetCurrentSocket() };
	//disable phyciscs to attach actor
	bullet->bulletMesh->SetSimulatePhysics(false);
	bullet->bulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//attach
	socket->AttachActor(bullet, weapon->weapon);
	bullet->bulletMesh->SetRelativeRotation(FRotator(90, 0, 0));
	//increase socket nr after firing so the next empty socket is always chosen.
	weapon->IncreaseSocketIndex();
	weapon->bullets.Add(bullet);
}
#pragma endregion

// Called every frame
void AP_VRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (wasPressed) {
		reloadTimer += GetWorld()->GetDeltaSeconds();
		if (reloadTimer >= 0.5f) {
			if (GunLeft)GunLeft->SetCanLock(true);
			if (GunRight)GunRight->SetCanLock(true);
			reloadTimer = 0;
			wasPressed = false;
		}
	}

	if (GameMode) {
		if (OwningSpawn) {
			if (OwningSpawn->GetSeconds() == 0 && ShotsFired < MaxBullets) {
				MatchTimer += DeltaTime;
			}

			if (ShotsFired < 1 && OwningSpawn->GetSeconds() == 0) {
				DrawSpeedTimer += DeltaTime;
			}
		}


		if (ShotsFired > 0 && ShotsFired < MaxBullets) {
			ShootSpeedTimer += DeltaTime;
		}
	}


	FVector head{ Head->GetRelativeLocation() };
	Belt->SetRelativeLocation(FVector(head.X-10, head.Y, Belt->GetRelativeLocation().Z));
}

#pragma region Shooting
void AP_VRPawn::ShootGun(bool isRightHanded) {
	UWidgetInteractionComponent* interaction{ isRightHanded ? RightInteraction : LeftInteraction };
	if (isMenu && interaction) {
		interaction->PressPointerKey(EKeys::LeftMouseButton);
		interaction->ReleasePointerKey(EKeys::LeftMouseButton);
	}

	if(isRightHanded){
		if (GunRight) {
			GunRight->Shoot(this);
			ShotsFired++;
		}
	}
	else {
		if(GunLeft){
			GunLeft->Shoot(this);
			ShotsFired++;
		}
	}
}

void AP_VRPawn::ShootGunReleased(bool isRightHanded) const {
	UWidgetInteractionComponent* interaction{ isRightHanded ? RightInteraction : LeftInteraction };
	if (isMenu && interaction) {
		interaction->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}
#pragma endregion

#pragma region reload
void AP_VRPawn::Reload(bool isRightHanded){
	if (isRightHanded) {
		if (GunRight) {
			HandleBonePhysicsSimulation(GunRight, "HingeBone", true);
			wasPressed = true;
		}
	}
	else {
		if (GunLeft) {
			HandleBonePhysicsSimulation(GunLeft, "HingeBone", true);
			wasPressed = true;
		}
	}
}
#pragma endregion

#pragma region releasing
void AP_VRPawn::LetGo(bool isRightController) {
	if (isRightController) {
		if (GrabbedItemRight) {
			if (GrabbedItemRight->IsA(AA_Collectible::StaticClass())) {
				AA_Collectible* col{ Cast<AA_Collectible>(GrabbedItemRight) };
				if (col) HandleAttach(col, col->GetStaticMeshComponent(), false);
			}
			if (GrabbedItemRight->IsA(AA_Projectile::StaticClass())) {
				AA_Projectile* bul{ Cast<AA_Projectile>(GrabbedItemRight) };
				if (bul) HandleAttach(bul, bul->bulletMesh, false);
			}
			GrabbedItemRight = nullptr;


			//AA_Collectible* col{ Cast<AA_Collectible>(GrabbedItemLeft) };
			//AA_Projectile* bul{ Cast<AA_Projectile>(GrabbedItemLeft) };
			//if (col) HandleAttach(col, col->GetStaticMeshComponent(), false);
			//if (bul) HandleAttach(bul, bul->bulletMesh, false);
			//GrabbedItemRight = nullptr;
		}
		if (GunRight) {
			LeftController->GetAttachChildren()[0]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			HandleAttach(GunRight, GunRight->weapon, false);
			GunRight->SetIsHeld(false);
			GunRight->SetPlayer(nullptr);
			//HandleBonePhysicsSimulation(weaponCast, "HingeBone", true);
			GunRight = nullptr;
		}
	}
	else {
		if (GrabbedItemLeft) {
			if (GrabbedItemLeft->IsA(AA_Collectible::StaticClass())) {
				AA_Collectible* col{ Cast<AA_Collectible>(GrabbedItemLeft) };
				if (col) HandleAttach(col, col->GetStaticMeshComponent(), false);
			}
			if (GrabbedItemLeft->IsA(AA_Projectile::StaticClass())) {
				AA_Projectile* bul{ Cast<AA_Projectile>(GrabbedItemLeft) };
				if (bul) HandleAttach(bul, bul->bulletMesh, false);
			}
			GrabbedItemLeft = nullptr;

			//AA_Collectible* col{ Cast<AA_Collectible>(GrabbedItemLeft) };
			//AA_Projectile* bul{ Cast<AA_Projectile>(GrabbedItemLeft) };
			//if (col) HandleAttach(col, col->GetStaticMeshComponent(), false);
			//if (bul) HandleAttach(bul, bul->bulletMesh, false);
			//GrabbedItemLeft = nullptr;
		}
		if (GunLeft) {
			RightController->GetAttachChildren()[0]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			HandleAttach(GunLeft, GunLeft->weapon, false);
			GunLeft->SetIsHeld(false);
			GunLeft->SetPlayer(nullptr);
			//HandleBonePhysicsSimulation(weaponCast, "HingeBone", true);
			GunLeft = nullptr;
		}
	}
}
#pragma endregion

#pragma region grabbing
void AP_VRPawn::GrabPressed(bool isRightController) {
	UMotionControllerComponent* controller{ isRightController ? RightController : LeftController };
	UStaticMeshComponent* mesh{ isRightController ? RightMesh : LeftMesh };

	TArray<AActor*> outOverlap{};
	mesh->GetOverlappingActors(outOverlap, AA_Weapon::StaticClass());
	if (outOverlap.Num() > 0) {
		AActor* itemCast{ outOverlap[0] };
		if (itemCast) {
			if (itemCast->IsA(AA_Collectible::StaticClass())) {
				AA_Collectible* col{ Cast<AA_Collectible>(itemCast) };
				if (col) HandleAttach(col, col->GetStaticMeshComponent(), true, controller);
				isRightController ? GrabbedItemRight = itemCast : GrabbedItemLeft = itemCast;
			}
			if (itemCast->IsA(AA_Projectile::StaticClass())) {
				AA_Projectile* bul{ Cast<AA_Projectile>(itemCast) };
				if (bul) HandleAttach(bul, bul->bulletMesh, true, controller);
				isRightController ? GrabbedItemRight = itemCast : GrabbedItemLeft = itemCast;
			}
			if (itemCast->IsA(AA_Weapon::StaticClass())) {
				AA_Weapon* weaponCast{ Cast<AA_Weapon>(itemCast) };
				HandleAttach(weaponCast, weaponCast->weapon, true, controller);
				HandleBonePhysicsSimulation(weaponCast, "HingeBone", false);
				//make sure the controller is not the controller holding the gun
				weaponCast->MotionController = isRightController ? LeftController : RightController;
				weaponCast->SetIsHeld(true);
				weaponCast->SetPlayer(this);
				if (isRightController) {
					GunRight = weaponCast;
					if (GunLeft == GunRight) {
						GunLeft = nullptr;
					}
				}
				else {
					GunLeft = weaponCast;
					if (GunRight == GunLeft) {
						GunRight = nullptr;
					}
				}
			}
		}

		/*AActor* itemCast{ Cast<AActor>(outOverlap[0]) };
		if (itemCast) {
			AA_Collectible* col{ Cast<AA_Collectible>(outOverlap[0]) };
			AA_Projectile* bul{ Cast<AA_Projectile>(outOverlap[0]) };
			if(col) HandleAttach(col, col->GetStaticMeshComponent(), true, controller);
			if(bul) HandleAttach(bul, bul->bulletMesh, true, controller);
			if (isRightController) {
				GrabbedItemRight = itemCast;
			}
			else {
				GrabbedItemLeft = itemCast;
			}
		}

		AA_Weapon* weaponCast{ Cast<AA_Weapon>(outOverlap[0]) };
		if (weaponCast) {
			HandleAttach(weaponCast, weaponCast->weapon, true, controller);
			HandleBonePhysicsSimulation(weaponCast, "HingeBone", false);
			if (isRightController) {
				GunRight = weaponCast;
				if (GunLeft == GunRight){
					GunLeft = nullptr;
				}
			}
			else {
				GunLeft = weaponCast;
				if (GunRight == GunLeft){
					GunRight = nullptr;
				}
			}
		}*/
	}
}
#pragma endregion

#pragma region attachment and simulation
void AP_VRPawn::HandleBonePhysicsSimulation(AA_Weapon* weaponCast, FName name, bool shouldSimulate)
{
	FBodyInstance* instance{ weaponCast->weapon->GetBodyInstance(name) };
	if (instance) {
		instance->SetInstanceSimulatePhysics(shouldSimulate);
	}
}

void AP_VRPawn::HandleAttach(AActor* actor, UStaticMeshComponent* mesh, bool shouldAttach, UMotionControllerComponent* controller) const
{
	if (shouldAttach) {
		mesh->SetSimulatePhysics(false);
		actor->AttachToComponent(controller, FAttachmentTransformRules::KeepWorldTransform);
	}
	else {
		actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		mesh->SetSimulatePhysics(true);
	}
}

void AP_VRPawn::HandleAttach(AActor* actor, USkeletalMeshComponent* mesh, bool shouldAttach, UMotionControllerComponent* controller) const
{
	if (shouldAttach) {
		mesh->SetSimulatePhysics(false);
		actor->AttachToComponent(controller, FAttachmentTransformRules::KeepWorldTransform);
	}
	else {
		actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		mesh->SetSimulatePhysics(true);
	}
}
#pragma endregion

#pragma region belt
void AP_VRPawn::HolsterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	UStaticMeshComponent* component{ nullptr };
	if (OverlappedComponent == RightBeltOverlap) {
		component = RightBeltSocket;
	}
	if(OverlappedComponent == LeftBeltOverlap){
		component = LeftBeltSocket;
	}

	if (component) {
		AA_Weapon* gun{ Cast<AA_Weapon>(OtherActor) };
		if (gun) {
			gun->HandleAttachment(component);

			//gun->weapon->SetSimulatePhysics(false);
			//gun->AttachToComponent(component, FAttachmentTransformRules::KeepWorldTransform);
			//gun->SetActorLocation(component->GetComponentLocation());

			//FRotator rot{ component->GetComponentRotation() };
			//gun->SetActorRotation(FRotator(360-rot.Pitch, 90, -90));


			if (gun == GunRight) {
				GunRight = nullptr;
			}
			if (gun == GunLeft) {
				GunLeft = nullptr;
			}
		}
	}
}
#pragma endregion

#pragma region collision and score
void AP_VRPawn::BodyPartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AA_Projectile* projectile{ Cast<AA_Projectile>(OtherActor) };
	if (projectile) {
		if(HeadCollision == OverlappedComponent){
			Score += 300;
			return;
		}
		if(NeckCollision == OverlappedComponent){
			Score += 350;
			return;
		}
		if(HeartCollision == OverlappedComponent){
			Score += 300;
			return;
		}
		if(RightKneeCollision == OverlappedComponent){
			Score += 100;
			return;
		}
		if(LeftKneeCollision == OverlappedComponent){
			Score += 100;
			return;
		}
		if(LeftElbowCollision == OverlappedComponent){
			Score += 100;
			return;
		}
		if(RightElbowCollision == OverlappedComponent){
			Score += 100;
			return;
		}
		if(LeftHandCollision == OverlappedComponent){
			Score += 100;
			return;
		}
		if(RightHandCollision == OverlappedComponent){
			Score += 100;
			return;
		}
		if(LeftKidneyCollision == OverlappedComponent){
			Score += 400;
			return;
		}
		if(RightKidneyCollision == OverlappedComponent){
			Score += 400;
			return;
		}
		if(DickCollision == OverlappedComponent){
			Score += 200;
			return;
		}
	}
}

void AP_VRPawn::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AA_Projectile* projectile{ Cast<AA_Projectile>(OtherActor) };
	if(projectile) {
		if (projectile->GetIsActive()) {
			UBoxComponent* closestWeakpoint{ GetClosestWeakpoint(projectile) };
			if (closestWeakpoint) {
				Score = CurrentSmallestValue * 10;
				ClosestWeakpoint = nullptr;
				CurrentSmallestValue = 99999999.0;
				/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("vital!"));*/
			}
			Score += 50;
			OwningSpawn->RegisterBullet(projectile);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("hit!"));
		}
	}
}

UBoxComponent* AP_VRPawn::GetClosestWeakpoint(AActor* projectile)
{
	for (UBoxComponent* comp : Collisions) {
		if (comp) {
			double distance{ FVector::Distance(comp->GetComponentLocation(), projectile->GetActorLocation()) };
			if (distance < CurrentSmallestValue) {
				CurrentSmallestValue = distance;
				ClosestWeakpoint = comp;
			}
		}
	}

	projectile->Destroy();
	return ClosestWeakpoint;
}
#pragma endregion
