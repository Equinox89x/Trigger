//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "AC_BodyCollisionComponent.h"
//#include <A_Projectile.h>
//#include <Kismet/GameplayStatics.h>
//#include <P_VRPawn.h>
//
//
//// Sets default values for this component's properties
//UAC_BodyCollisionComponent::UAC_BodyCollisionComponent()
//{
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	PrimaryComponentTick.bCanEverTick = true;
//
//	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
//	CharacterMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
//	CharacterMesh->SetGenerateOverlapEvents(true);
//	CharacterMesh->SetRelativeScale3D(FVector(0.29, 0.29, 0.29));
//	CharacterMesh->SetRelativeRotation(FRotator(0, -90, 0));
//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> charObj(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Cowboy/source/PlayerRigged__2_.PlayerRigged__2_'"));
//	CharacterMesh->SetSkeletalMeshAsset(charObj.Object);
//	static ConstructorHelpers::FObjectFinder<UMaterial> charMat(TEXT("/Script/Engine.Material'/Game/Characters/Cowboy/source/Material_001.Material_001'"));
//	CharacterMesh->SetMaterial(0, charMat.Object);
//	CharacterMesh->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::PlayerOverlap);
//
//
//	HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollision"));
//	NeckCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("NeckCollision"));
//	HeartCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HeartCollision"));
//	RightKneeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightKneeCollision"));
//	LeftKneeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftKneeCollision"));
//	LeftElbowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftElbowCollision"));
//	RightElbowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightElbowCollision"));
//	LeftHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollision"));
//	RightHandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollision"));
//	LeftKidneyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftKidneyCollision"));
//	RightKidneyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightKidneyCollision"));
//	DickCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DickCollision"));
//
//	HeadCollision->SetupAttachment(CharacterMesh);
//	NeckCollision->SetupAttachment(CharacterMesh);
//	HeartCollision->SetupAttachment(CharacterMesh);
//	RightKneeCollision->SetupAttachment(CharacterMesh);
//	LeftKneeCollision->SetupAttachment(CharacterMesh);
//	LeftElbowCollision->SetupAttachment(CharacterMesh);
//	RightElbowCollision->SetupAttachment(CharacterMesh);
//	LeftHandCollision->SetupAttachment(CharacterMesh);
//	RightHandCollision->SetupAttachment(CharacterMesh);
//	LeftKidneyCollision->SetupAttachment(CharacterMesh);
//	RightKidneyCollision->SetupAttachment(CharacterMesh);
//	DickCollision->SetupAttachment(CharacterMesh);
//
//	HeadCollision->SetGenerateOverlapEvents(true);
//	NeckCollision->SetGenerateOverlapEvents(true);
//	HeartCollision->SetGenerateOverlapEvents(true);
//	RightKneeCollision->SetGenerateOverlapEvents(true);
//	LeftKneeCollision->SetGenerateOverlapEvents(true);
//	LeftElbowCollision->SetGenerateOverlapEvents(true);
//	RightElbowCollision->SetGenerateOverlapEvents(true);
//	LeftHandCollision->SetGenerateOverlapEvents(true);
//	RightHandCollision->SetGenerateOverlapEvents(true);
//	LeftKidneyCollision->SetGenerateOverlapEvents(true);
//	RightKidneyCollision->SetGenerateOverlapEvents(true);
//	DickCollision->SetGenerateOverlapEvents(true);
//
//	HeadCollision->SetRelativeLocation(FVector(0.574568, 0.000000, 647.658561));
//	NeckCollision->SetRelativeLocation(FVector(0.574568, 0.000000, 573.812407));
//	HeartCollision->SetRelativeLocation(FVector(32.574568, 0.000000, 507.350869));
//	RightKneeCollision->SetRelativeLocation(FVector(37.497645, 0.000000, 177.504715));
//	LeftKneeCollision->SetRelativeLocation(FVector(-36.348509, 0.000000, 177.504715));
//	LeftElbowCollision->SetRelativeLocation(FVector(-201.271586, 0.000000, 529.504715));
//	RightElbowCollision->SetRelativeLocation(FVector(195.036106, 0.000000, 529.504715));
//	LeftHandCollision->SetRelativeLocation(FVector(-361.271586, 0.000000, 529.504715));
//	RightHandCollision->SetRelativeLocation(FVector(359.959183, 0.000000, 529.504715));
//	LeftKidneyCollision->SetRelativeLocation(FVector(-51.117740, 0.000000, 394.120099));
//	RightKidneyCollision->SetRelativeLocation(FVector(52.266875, 0.000000, 394.120099));
//	DickCollision->SetRelativeLocation(FVector(0.574567, 0.000000, 335.043176));
//	HeadCollision->SetRelativeScale3D(FVector(1.078189, 3.469530, 0.658177));
//	NeckCollision->SetRelativeScale3D(FVector(1.078189, 3.469530, 0.314427));
//	HeartCollision->SetRelativeScale3D(FVector(1.078189, 3.469530, 0.658177));
//	RightKneeCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
//	LeftKneeCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
//	LeftElbowCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
//	RightElbowCollision->SetRelativeScale3D(FVector(0.953189, 3.469530, 0.658177));
//	LeftHandCollision->SetRelativeScale3D(FVector(1.515689, 3.469530, 0.658177));
//	RightHandCollision->SetRelativeScale3D(FVector(1.515689, 3.469530, 0.658177));
//	LeftKidneyCollision->SetRelativeScale3D(FVector(0.484439, 3.469530, 0.345677));
//	RightKidneyCollision->SetRelativeScale3D(FVector(0.484439, 3.469530, 0.345677));
//	DickCollision->SetRelativeScale3D(FVector(0.484439, 3.469530, 1.126927));
//
//	HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	NeckCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	HeartCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	RightKneeCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	LeftKneeCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	LeftElbowCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	RightElbowCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	LeftKidneyCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	RightKidneyCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	DickCollision->OnComponentBeginOverlap.AddDynamic(this, &UAC_BodyCollisionComponent::BodyPartOverlap);
//	Collisions = { HeadCollision, NeckCollision, HeartCollision, RightKneeCollision, LeftKneeCollision, LeftElbowCollision, RightElbowCollision, LeftHandCollision, RightHandCollision, LeftKidneyCollision, RightKidneyCollision, DickCollision };
//
//	// ...
//}
//
//void UAC_BodyCollisionComponent::SetPlayerScore(bool didHaveMaxBullets)
//{
//	if (PlayerState) PlayerState->Score += Score;
//	if (PlayerState) PlayerState->SetScoreDetails(didHaveMaxBullets, DrawSpeedTimer, ShootSpeedTimer, ShotAccuracy, Score, didHaveMaxBullets ? 1 : MatchTimer);
//}
//
//// Called when the game starts
//void UAC_BodyCollisionComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	APS_Menu* ps{ Cast<APS_Menu>(Cast<AP_VRPawn>(GetOwner())->PlayerState) };
//	if (ps) PlayerState = ps;
//
//	CharacterMesh->SetOwnerNoSee(true);
//	CharacterMesh->SetupAttachment(GetOwner()->GetRootComponent());
//
//	// ...
//	
//}
//
//
//// Called every frame
//void UAC_BodyCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	if (Cast<AP_VRPawn>(GetOwner())->GameMode) {
//		if (auto spawn{ Cast<AP_VRPawn>(GetOwner())->OwningSpawn }) {
//			if (spawn->GetSeconds() == 0 && ShotsFired < MaxBullets) {
//				MatchTimer += DeltaTime;
//			}
//
//			if (ShotsFired < 1 && spawn->GetSeconds() == 0) {
//				DrawSpeedTimer += DeltaTime;
//			}
//		}
//
//
//		if (ShotsFired > 0 && ShotsFired < MaxBullets) {
//			ShootSpeedTimer += DeltaTime;
//		}
//	}
//	// ...
//}
//
//#pragma region collision and score
//void UAC_BodyCollisionComponent::BodyPartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
//{
//	AA_Projectile* projectile{ Cast<AA_Projectile>(OtherActor) };
//	if (projectile) {
//		if (HeadCollision == OverlappedComponent) {
//			Score += 300;
//			return;
//		}
//		if (NeckCollision == OverlappedComponent) {
//			Score += 350;
//			return;
//		}
//		if (HeartCollision == OverlappedComponent) {
//			Score += 300;
//			return;
//		}
//		if (RightKneeCollision == OverlappedComponent) {
//			Score += 100;
//			return;
//		}
//		if (LeftKneeCollision == OverlappedComponent) {
//			Score += 100;
//			return;
//		}
//		if (LeftElbowCollision == OverlappedComponent) {
//			Score += 100;
//			return;
//		}
//		if (RightElbowCollision == OverlappedComponent) {
//			Score += 100;
//			return;
//		}
//		if (LeftHandCollision == OverlappedComponent) {
//			Score += 100;
//			return;
//		}
//		if (RightHandCollision == OverlappedComponent) {
//			Score += 100;
//			return;
//		}
//		if (LeftKidneyCollision == OverlappedComponent) {
//			Score += 400;
//			return;
//		}
//		if (RightKidneyCollision == OverlappedComponent) {
//			Score += 400;
//			return;
//		}
//		if (DickCollision == OverlappedComponent) {
//			Score += 200;
//			return;
//		}
//	}
//}
//
//void UAC_BodyCollisionComponent::PlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
//{
//	AA_Projectile* projectile{ Cast<AA_Projectile>(OtherActor) };
//	if (projectile) {
//		if (projectile->GetIsActive()) {
//			UBoxComponent* closestWeakpoint{ GetClosestWeakpoint(projectile) };
//			if (closestWeakpoint) {
//				Score = CurrentSmallestValue * 10;
//				ClosestWeakpoint = nullptr;
//				CurrentSmallestValue = 99999999.0;
//				/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("vital!"));*/
//			}
//			Score += 50;
//			Cast<AP_VRPawn>(GetOwner())->OwningSpawn->RegisterBullet(projectile);
//			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("hit!"));
//		}
//	}
//}
//
//UBoxComponent* UAC_BodyCollisionComponent::GetClosestWeakpoint(AActor* projectile)
//{
//	for (UBoxComponent* comp : Collisions) {
//		if (comp) {
//			double distance{ FVector::Distance(comp->GetComponentLocation(), projectile->GetActorLocation()) };
//			if (distance < CurrentSmallestValue) {
//				CurrentSmallestValue = distance;
//				ClosestWeakpoint = comp;
//			}
//		}
//	}
//
//	projectile->Destroy();
//	return ClosestWeakpoint;
//}
//#pragma endregion