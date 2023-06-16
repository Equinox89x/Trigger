// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Spawn.h"
#include "UObject/UObjectGlobals.h"
#include <GS_Menu.h>
#include <P_VRPawn.h>
#include <A_Score.h>

// Sets default values
AA_Spawn::AA_Spawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bNetLoadOnClient = true;
	bAlwaysRelevant = true;

	Comp = CreateDefaultSubobject<USceneComponent>(TEXT("component"));
	Blocker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blocker"));
	Spawn1Comp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Spawn1"));
	Spawn2Comp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Spawn2"));
	TimerFront = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimerFront"));
	TimerBack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimerBack"));
	Overlapper = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlapper"));

	Blocker->SetupAttachment(Comp);
	Spawn1Comp->SetupAttachment(Comp);
	Spawn2Comp->SetupAttachment(Comp);
	TimerFront->SetupAttachment(Comp);
	TimerBack->SetupAttachment(Comp);
	Overlapper->SetupAttachment(Comp);

	RootComponent = Comp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> plane(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> material(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/UI/MI_Blocker.MI_Blocker'"));
	Blocker->SetStaticMesh(plane.Object);
	Blocker->SetMaterial(0, material.Object);

	Blocker->SetRelativeRotation(FRotator(0.000001, 630.000001, 450.000000));
	Blocker->SetRelativeScale3D(FVector(5, 5, 5));
	Blocker->SetRelativeLocation(FVector(0, 0, 250));

	TimerBack->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TimerBack->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TimerBack->SetRelativeLocation(FVector(85.000000, 0.000000, 245.000000));
	TimerBack->SetRelativeScale3D(FVector(4,4,4));
	TimerBack->SetIsReplicated(true);

	TimerFront->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TimerFront->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TimerFront->SetRelativeRotation(FRotator(0, 180, 0));
	TimerFront->SetRelativeLocation(FVector(-85.000000, 0.000000, 245.000000));
	TimerFront->SetRelativeScale3D(FVector(4,4,4));
	TimerFront->SetIsReplicated(true);

	Overlapper->SetBoxExtent(FVector(500, 200, 200));
	Overlapper->AddRelativeLocation(FVector(0, 0, 200));
	Overlapper->OnComponentEndOverlap.AddDynamic(this, &AA_Spawn::OnOverlap);

	Spawn1Comp->SetMobility(EComponentMobility::Movable);
	Spawn2Comp->SetMobility(EComponentMobility::Movable);
	TimerBack->SetMobility(EComponentMobility::Static);
	TimerFront->SetMobility(EComponentMobility::Static);
	Blocker->SetMobility(EComponentMobility::Static);
	Comp->SetMobility(EComponentMobility::Static);

}

void AA_Spawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Spawn1Comp->CreateChildActor();
	Spawn1Comp->SetRelativeRotation(FRotator(0, 180, 0));
	Spawn2Comp->CreateChildActor();
	
}

// Called when the game starts or when spawned
void AA_Spawn::BeginPlay()
{
	Super::BeginPlay();

	OriginalTime = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	UGI_Network* gi{ Cast<UGI_Network>(GetWorld()->GetGameInstance()) };
	if (gi) {
		GameInstance = gi;
	}

	Spawn1 = Cast<APlayerStart>(Spawn1Comp->GetChildActor());
	Spawn2 = Cast<APlayerStart>(Spawn2Comp->GetChildActor());

	if (GameInstance) {
		distance = GameInstance->Distance == 0 ? 600 : GameInstance->Distance;
	}
	Spawn1Comp->SetRelativeLocation(FVector(600 / 2, 0, 0));
	Spawn2Comp->SetRelativeLocation(FVector(-600 / 2, 0, 0));
}

void AA_Spawn::SetSeconds_Implementation(int seconds)
{
	TimerBack->SetText(FText::FromString(FString::FromInt(seconds)));
	TimerFront->SetText(FText::FromString(FString::FromInt(seconds)));
}

// Called every frame
void AA_Spawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Seconds == 0) {
		ElapsedTime += DeltaTime;
	}
}

void AA_Spawn::Countdown()
{
	TimerBack->SetText(FText::FromString(FString::FromInt(Seconds)));
	TimerFront->SetText(FText::FromString(FString::FromInt(Seconds)));
	if (Seconds == 0) {
		/*if (ChosenSpawn) {
			ChosenSpawn->Destroy();
		}*/
		Blocker->SetHiddenInGame(true);
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
	else {
		Seconds--;
	}
}

#pragma region Bullet Hit Registration
//check if bullet has hit the player or if the bullet was a missed shot
void AA_Spawn::RegisterBullet(AA_Projectile* projectile)
{
	if (projectile->GetPlayer()) {
		if (projectile->GetPlayer() == Player1) {
			Player1Bullets++;
		}	
		else if (projectile->GetPlayer() == Player2) {
			Player2Bullets++;
		}
		projectile->Destroy();

		if(Player1Bullets == GameInstance->MaxBullets || Player2Bullets == GameInstance->MaxBullets){
			EndMatch();
		}
	}
}

//void AA_Spawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
void AA_Spawn::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto projectile{ Cast<AA_Projectile>(OtherActor) }) {
		RegisterBullet(projectile);
	}
}
#pragma endregion

#pragma region End of match
void AA_Spawn::EndMatch()
{
	//TODO notify end of match
	//1. Disable movement
	DisableInput(Cast<APlayerController>(Player1->GetController()));
	DisableInput(Cast<APlayerController>(Player2->GetController()));
	//2. Slow down game
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTime/10);
	//3. Show score screen
	//Player State is nullptr on simulation (because no controller to keep track of it)
	Player1->SetPlayerScore(Player1Bullets == GameInstance->MaxBullets);
	Player2->SetPlayerScore(Player2Bullets == GameInstance->MaxBullets);
	AA_Score* scoreUI{ Cast<AA_Score>(UGameplayStatics::GetActorOfClass(GetWorld(), AA_Score::StaticClass())) };
	scoreUI->SetHidden(false);
	//4. Let a few seconds pass (15)
	FTimerHandle outHandle;
	GetWorldTimerManager().SetTimer(outHandle, this, &AA_Spawn::RemoveScoreScreen, 15, false);
}

void AA_Spawn::RemoveScoreScreen() {
	//5. Remove score screen
	AA_Score* scoreUI{ Cast<AA_Score>(UGameplayStatics::GetActorOfClass(GetWorld(), AA_Score::StaticClass())) };
	scoreUI->SetHidden(true);
	//6. Reset time
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTime);
	//7. Start new match
	//TODO notify start of match

}
#pragma endregion

void AA_Spawn::SetPlayer(AP_VRPawn* player)
{
	if (!Player1) Player1 = player;
	if (!Player2) Player2 = player;

	if (Player1 && Player2) {
		FTimerHandle timer;
		GetWorldTimerManager().SetTimer(timer, this, &AA_Spawn::Countdown, 1.f, true, 0);
		//TODO notify start of match
	}
}

//required for replication
void AA_Spawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(AA_Spawn, TimerFront);
	DOREPLIFETIME(AA_Spawn, TimerBack);
	DOREPLIFETIME(AA_Spawn, Owner);
}



