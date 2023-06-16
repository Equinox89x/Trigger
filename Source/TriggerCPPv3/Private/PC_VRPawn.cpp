// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_VRPawn.h"

APC_VRPawn::APC_VRPawn()
{
	bNetLoadOnClient = true;
	NetDormancy = ENetDormancy::DORM_Awake;
	bOnlyRelevantToOwner = true;

	#pragma region inputs
	static ConstructorHelpers::FObjectFinder<UInputAction> input(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Grab_Left.IA_Grab_Left'"));
	GrabLeft = input.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input2(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Grab_Right.IA_Grab_Right'"));
	GrabRight = input2.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input3(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	Movement = input3.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input4(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Trigger_Right.IA_Trigger_Right'"));
	TriggerRight = input4.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input5(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Trigger_Left.IA_Trigger_Left'"));
	TriggerLeft = input5.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input6(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Rotate.IA_Rotate'"));
	Rotate = input6.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input7(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Reload_Left.IA_Reload_Left'"));
	ReloadLeftKey = input7.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> input8(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Reload_Right.IA_Reload_Right'"));
	ReloadRightKey = input8.Object;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> context(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_VR.IMC_VR'"));
	inputMappingContext = context.Object;
	#pragma endregion
}

void APC_VRPawn::BeginPlay()
{
	Super::BeginPlay();

	FString mapname{ GetWorld()->GetMapName() };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, mapname);
	if (IsLocalPlayerController()) {
		SRSpawnPlayer();
		Player = Cast<AP_VRPawn>(GetPawn());
	}
}

void APC_VRPawn::SetupInputComponent()
{
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (subsystem) {
		subsystem->ClearAllMappings();
		subsystem->AddMappingContext(inputMappingContext, 0);
	}

	if (UEnhancedInputComponent* enhancedComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		//enhancedComponent->BindAction("GoToThere", EInputEvent::IE_Pressed, this, &AP_VRPawn::GoToThere);

		if (GrabRight) {
			enhancedComponent->BindAction(GrabRight, ETriggerEvent::Started, this, &APC_VRPawn::GrabRightPressed);
			enhancedComponent->BindAction(GrabRight, ETriggerEvent::Completed, this, &APC_VRPawn::GrabRightReleased);
		}
		if (GrabLeft) {
			enhancedComponent->BindAction(GrabLeft, ETriggerEvent::Started, this, &APC_VRPawn::GrabLeftPressed);
			enhancedComponent->BindAction(GrabLeft, ETriggerEvent::Completed, this, &APC_VRPawn::GrabLeftReleased);
		}

		if (TriggerLeft) {
			enhancedComponent->BindAction(TriggerLeft, ETriggerEvent::Started, this, &APC_VRPawn::ShootGunLeft);
			enhancedComponent->BindAction(TriggerLeft, ETriggerEvent::Completed, this, &APC_VRPawn::ShootGunReleasedLeft);
			enhancedComponent->BindAction(TriggerLeft, ETriggerEvent::Canceled, this, &APC_VRPawn::ShootGunReleasedLeft);
		}
		if (TriggerRight) {
			enhancedComponent->BindAction(TriggerRight, ETriggerEvent::Started, this, &APC_VRPawn::ShootGunRight);
			enhancedComponent->BindAction(TriggerRight, ETriggerEvent::Completed, this, &APC_VRPawn::ShootGunReleasedRight);
			enhancedComponent->BindAction(TriggerRight, ETriggerEvent::Canceled, this, &APC_VRPawn::ShootGunReleasedRight);
		}

		if (Movement) {
			enhancedComponent->BindAction(Movement, ETriggerEvent::Triggered, this, &APC_VRPawn::Move);
		}
		if (Rotate) {
			enhancedComponent->BindAction(Rotate, ETriggerEvent::Triggered, this, &APC_VRPawn::Turn);
		}

		if (ReloadLeftKey) {
			enhancedComponent->BindAction(ReloadLeftKey, ETriggerEvent::Triggered, this, &APC_VRPawn::ReloadLeft);
		}
		if (ReloadRightKey) {
			enhancedComponent->BindAction(ReloadRightKey, ETriggerEvent::Triggered, this, &APC_VRPawn::ReloadRight);
		}
	}

}

#pragma region Movement
void APC_VRPawn::Move(const FInputActionValue& value) {
	if (value.GetMagnitude() != 0.f) {
		Player->AddActorWorldOffset(Player->GetActorForwardVector() * value[1] * MoveSpeed);
		Player->AddActorWorldOffset(Player->GetActorRightVector() * value[0] * MoveSpeed);
	}
}

void APC_VRPawn::Turn(const FInputActionValue& value) {
	Player->AddActorWorldRotation(FRotator(0, value[0] * RotationSpeed, 0));
	Player->AddActorWorldRotation(FRotator(0, value[1] * RotationSpeed, 0));
}
#pragma endregion

#pragma region Input functions
void APC_VRPawn::ShootGunLeft() {Player->ShootGun(false);}
void APC_VRPawn::ShootGunRight() {Player->ShootGun(true);}
void APC_VRPawn::ShootGunReleasedLeft() {Player->ShootGunReleased(false);}
void APC_VRPawn::ShootGunReleasedRight() {Player->ShootGunReleased(true);}

void APC_VRPawn::ReloadRight() {Player->Reload(true);}
void APC_VRPawn::ReloadLeft() {Player->Reload(false);}

void APC_VRPawn::GrabRightReleased() {Player->LetGo(true);}
void APC_VRPawn::GrabLeftReleased() { Player->LetGo(false); }
void APC_VRPawn::GrabRightPressed() { Player->GrabPressed(true);}
void APC_VRPawn::GrabLeftPressed() {Player->GrabPressed(false);}
#pragma endregion

void APC_VRPawn::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	//if it is any mode than the menugamemode
	/*AGM_Menu* gm{ Cast<AGM_Menu>(GetWorld()->GetAuthGameMode()) };
	if (!gm) {
		DisableInput(GetWorld()->GetFirstPlayerController());
	}*/
	
}

void APC_VRPawn::SRSpawnPlayer_Implementation()
{

	AGM_Menu* gm{ Cast<AGM_Menu>(GetWorld()->GetAuthGameMode()) };
	if (gm) {
		gm->SpawnPlayer(this);
		return;
	}
	AGM_Versus* gm2{ Cast<AGM_Versus>(GetWorld()->GetAuthGameMode()) };
	if (gm2) {
		gm2->SpawnPlayer(this);
		return;
	}
}
