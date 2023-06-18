// Fill out your copyright notice in the Description page of Project Settings.

#include "A_Weapon.h"
#include <TriggerCPPv3/Actors/Character/P_VRPawn.h>

// Sets default values
AA_Weapon::AA_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    #pragma region Init
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("scene"));
    weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weapon"));
    Revolver = CreateDefaultSubobject<USkeletalMesh>(TEXT("revolver"));
    interactionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("InteractionComp"));
    arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow"));

    MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
    HammerEnterCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerEnterCollision"));
    HammerLeaveCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerLeaveCollision"));
    HammerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HammerMesh"));
    Hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));
    HammerArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("HammerArrow"));

    arrow->SetupAttachment(interactionComp);
    HammerArrow->SetupAttachment(Hammer);
    weapon->SetupAttachment(Scene);
    MotionController->SetupAttachment(Scene);
    HammerEnterCollision->SetupAttachment(Scene);
    HammerLeaveCollision->SetupAttachment(Scene);
    HammerMesh->SetupAttachment(Scene);
    Hammer->SetupAttachment(Scene);
    #pragma endregion

    #pragma region weapon
    static ConstructorHelpers::FObjectFinder<UStaticMesh> obj(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));
    HammerMesh->SetStaticMesh(obj.Object);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> hammer(TEXT("/Script/Engine.StaticMesh'/Game/Weapons/Colt/Mesh/hammer.hammer'"));
    Hammer->SetStaticMesh(hammer.Object);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> loadedObject(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Colt/Mesh/skeletalgun2.skeletalgun2'"));
    Revolver = loadedObject.Object;
    weapon->SetSkeletalMesh(Revolver);
    /*weapon->SetWorldScale3D(FVector(0.0175f, 0.0175f, 0.0175f));*/
    weapon->SetGenerateOverlapEvents(true);
    weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    weapon->SetCollisionProfileName(FName{ "PhysicsActor" });
    weapon->SetMobility(EComponentMobility::Movable);
    weapon->SetCanEverAffectNavigation(false);
    //2kg weapon + 2.160kg of bullets
    weapon->SetMassOverrideInKg("RootBone", 4.160f, true);
    weapon->SetMassOverrideInKg("HingeBone", 1.2f, true);
    FBodyInstance* instance{ weapon->GetBodyInstance("HingeBone") };
    if (instance) {
        instance->SetInstanceSimulatePhysics(false);
    }
    weapon->SetMassOverrideInKg("HammerBone", 1.2f, true);
    FBodyInstance* instance2{ weapon->GetBodyInstance("HammerBone") };
    if (instance2) {
        instance2->SetInstanceSimulatePhysics(true);
    }
    weapon->SetSimulatePhysics(true);
    #pragma endregion
   
    arrow->ArrowLength = 3000;

    interactionComp->SetRelativeRotation(FRotator(0,180,0));
    interactionComp->SetRelativeLocation(FVector(0.f, -35.f, 0.f));
    interactionComp->TraceChannel = ECollisionChannel::ECC_WorldDynamic;
    interactionComp->bShowDebug = true;
    interactionComp->PointerIndex = 3;

    #pragma region hammer
    HammerMesh->SetRelativeLocation(FVector(1350, -400, -270));
    HammerMesh->SetRelativeScale3D(FVector(5, 1, 5));

    HammerEnterCollision->SetRelativeLocation(FVector(1460.000000, -150, 0));
    HammerEnterCollision->SetBoxExtent(FVector(400, 600, 400));
    
    HammerLeaveCollision->SetRelativeLocation(FVector(1090, 260, 0));
    HammerLeaveCollision->SetBoxExtent(FVector(400, 400, 400));
    HammerLeaveCollision->SetRelativeScale3D(FVector(2.25, 2, 2.25));

    Hammer->SetupAttachment(Scene);
    Hammer->SetRelativeLocation(FVector(620, 65, 0));

    HammerArrow->SetRelativeLocation(FVector(80, -0.000002, 0));
    HammerArrow->SetRelativeRotation(FRotator(0, 90, 0));
    HammerArrow->SetRelativeScale3D(FVector(0.5, 2, 2));
    HammerArrow->ArrowSize = 10;


    HammerEnterCollision->OnComponentBeginOverlap.AddDynamic(this, &AA_Weapon::HammerEnter);
    HammerEnterCollision->OnComponentEndOverlap.AddDynamic(this, &AA_Weapon::HandLeave);
    HammerLeaveCollision->OnComponentEndOverlap.AddDynamic(this, &AA_Weapon::HammerLeave);
    #pragma endregion

    RootComponent = Scene;
    Scene->SetWorldScale3D(FVector(0.0175f, 0.0175f, 0.0175f));
}

void AA_Weapon::BeginPlay()
{
    Super::BeginPlay();
    HammerInitialRelativeTransform = HammerMesh->GetRelativeTransform();
    HammerInitialWorldTransform = HammerMesh->GetComponentTransform();
}


void AA_Weapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Check if the magazine is back into its place
    //if so, lock the magazine again
    if (canLock) {
        FVector transform{ weapon->GetSocketLocation("CenterRefferenceDynamic") };
        FVector transform1{ weapon->GetSocketLocation("CenterRefferenceStatic") };
        if (FVector::Distance(transform, transform1) < 0.01f ) {
            LockBodies();
        }
    }

    if (isTracking) {
        HandleTriggerMovement();
    }
}

void AA_Weapon::HandleTriggerMovement()
{
    //get the distance between the inital location of the mesh and the current position in the world (defined by your hand because attachment)
    double z{ (HammerInitialWorldTransform.GetLocation().Z - HammerMesh->GetComponentLocation().Z) * 5 };
    FRotator relativeRot{ HammerInitialRelativeTransform.GetRotation() };
    //clamp the rotation to max 70 degrees (pin all the way back) and apply it, keeping the other original axis values
    z = std::clamp(z, 0.0, 70.0);
    Hammer->SetRelativeRotation(FRotator(relativeRot.Pitch, z, relativeRot.Yaw));

    //return and shoot when trigger is pulled back
    if (z >= 69.0) {
        canShoot = true;
        ReturnTrigger();
    }
}

void AA_Weapon::HammerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (isHeld) {
        AP_VRPawn* pawn{ Cast<AP_VRPawn>(OtherActor) };
        if (pawn) {
            //set current world start position of the mesh for accurate distance calculation
            if(!isTracking) HammerInitialWorldTransform = HammerMesh->GetComponentTransform();
            //detach from scene and attach to hand
            HammerMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            HammerMesh->AttachToComponent(!pawn->IsGunRight() ? pawn->RightController : pawn->LeftController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            isTracking = true;
        }
    }
}

void AA_Weapon::HammerLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (isHeld) {
        AP_VRPawn* pawn{ Cast<AP_VRPawn>(OtherActor) };
        if (pawn) {
            ReturnTrigger();
        }
    }
}

void AA_Weapon::HandLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (isHeld) {
        AP_VRPawn* pawn{ Cast<AP_VRPawn>(OtherActor) };
        if (pawn) {
            ReturnTrigger();
        }
    }
}

void AA_Weapon::ReturnTrigger()
{
    //detach from hand and attach to scene and put the mesh back to original position
    //and hammer back to original rotation
    HammerMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    HammerMesh->AttachToComponent(Scene, FAttachmentTransformRules::KeepWorldTransform);
    HammerMesh->SetRelativeTransform(HammerInitialRelativeTransform);
    FRotator relativeRot{ HammerInitialRelativeTransform.GetRotation() };
    Hammer->SetRelativeRotation(FRotator(relativeRot.Pitch, 0, relativeRot.Yaw));
    if (canShoot) Shoot(Player);
    canShoot = false;
    isTracking = false;
}

void AA_Weapon::LockBodies() {
    FBodyInstance* instance{ weapon->GetBodyInstance("HingeBone") };
    if (instance) {
        instance->SetInstanceSimulatePhysics(false);
    }
    canLock = false;
}

const USkeletalMeshSocket* AA_Weapon::GetCurrentSocket() const
{
    FString name{ "bulletSocket" };
    name.AppendInt(socketIndex);
    const USkeletalMeshSocket* socket{ weapon->GetSocketByName(FName{name}) };
    return socket;
}

void AA_Weapon::HandleAttachment(USceneComponent* component)
{
    weapon->SetSimulatePhysics(false);
    AttachToComponent(component, FAttachmentTransformRules::KeepWorldTransform);
    SetActorLocation(component->GetComponentLocation());

    FRotator rot{ component->GetComponentRotation() };
    SetActorRotation(FRotator(360 - rot.Pitch, 90, -90));
}

void AA_Weapon::Shoot(AP_VRPawn* player)
{
    //fire the weapon if enough bullets are in it
    if (!bullets.IsEmpty()) {
        const FVector location{ GetActorLocation() };
        const FRotator rotation{ GetActorRotation() };
        AA_Projectile* projectile{ static_cast<AA_Projectile*>(GetWorld()->SpawnActor(AA_Projectile::StaticClass(), &location, &rotation)) };
        projectile->bulletMesh->AddImpulse(-projectile->GetActorForwardVector() * 500.f);
        projectile->SetIsActive(true);
        projectile->SetPlayer(player);


        //keep track of bullet removal and which socket to fill
        projectile = bullets.Last();
        projectile->Destroy();
        bullets.Pop(true);
        if(socketIndex != 1){
            socketIndex--;
        }
    }
}


