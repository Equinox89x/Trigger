// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Collectible.h"

AA_Collectible::AA_Collectible() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObject(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'"));
    GetStaticMeshComponent()->SetStaticMesh(loadedObject.Object);
    GetStaticMeshComponent()->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));


    static ConstructorHelpers::FObjectFinder<UMaterialInstance> loadedMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LevelPrototyping/Materials/MI_Solid_Blue.MI_Solid_Blue'"));
    GetStaticMeshComponent()->SetMaterial(0, loadedMaterial.Object);

    GetStaticMeshComponent()->SetSimulatePhysics(true);
    SetMobility(EComponentMobility::Movable);

    GetStaticMeshComponent()->SetCanEverAffectNavigation(false);
    GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
    GetStaticMeshComponent()->SetMassOverrideInKg("Name_None", 5.f);

    //GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

    ////needed for magnetic grab
    //GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);
    //GetStaticMeshComponent()->SetCollisionProfileName("PhysicsActor");
}