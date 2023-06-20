// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerCPPv3/UI/A_StartButton.h"
#include <TriggerCPPv3/Actors/Weapon/A_Projectile.h>
#include <TriggerCPPv3/Game/GM_Lobby.h>
#include <Misc/OutputDeviceNull.h>

// Sets default values
AA_StartButton::AA_StartButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("comp"));
	button = CreateDefaultSubobject<UWidgetComponent>(TEXT("button"));
	button2 = CreateDefaultSubobject<UWidgetComponent>(TEXT("button2"));
	Overlapper = CreateDefaultSubobject<USphereComponent>(TEXT("Overlapper"));
	RootComponent = Scene;

	button->SetupAttachment(Scene);
	button2->SetupAttachment(Scene);
	Overlapper->SetupAttachment(Scene);

	Overlapper->SetSphereRadius(100);
	Overlapper->OnComponentEndOverlap.AddDynamic(this, &AA_StartButton::OnOverlap);

}

void AA_StartButton::SetStartBtnActive(bool isActive)
{
	if (button) {
		FOutputDeviceNull output;
		FString str{ "SwitchTexture " };
		str.Append(isActive ? "true" : "false");
		button->GetWidget()->CallFunctionByNameWithArguments(*str, output, nullptr, true);
	}
}

// Called when the game starts or when spawned
void AA_StartButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_StartButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA_StartButton::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto bullet{ Cast<AA_Projectile>(OtherActor) }) {
		if (AGM_Lobby * gm{ Cast<AGM_Lobby>(GetWorld()->GetAuthGameMode()) }) {
			gm->StartGame();
		}
	}
}

