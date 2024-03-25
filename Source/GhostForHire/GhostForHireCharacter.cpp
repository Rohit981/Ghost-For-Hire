// Copyright Epic Games, Inc. All Rights Reserved.

#include "GhostForHireCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractableObject.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AverageJoeAI.h"
#include "Math/Vector.h"





//////////////////////////////////////////////////////////////////////////
// AGhostForHireCharacter

AGhostForHireCharacter::AGhostForHireCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // CHNGED IT HERE ======================================<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//CameraBoom = CreateDefaultSubobject<UCustomSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->SetupAttachment(CameraBoom, UCustomSpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	IntObjRef = NULL;

	endTimeline.BindUFunction(this, FName("ResetTimeline"));

	// CHANGE THIS LATER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	AB_PoltergeistI = true;

	//CharacterMesh = GetMesh();

	//AB_PoltergeistII = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGhostForHireCharacter::TimelineProgress(float Value)
{
	// Checks to see if Poltergeist 1 is unlocked and if SphereTrace is hitting something and if Q was pressed, and if it is then it will activate Poltergeist 1
	if (AB_PoltergeistI && IntObjRef2 != NULL && pressedQ) {
		IntObjRef2->PoltergeistI();
	}
	if (AB_PoltergeistII && IntObjRef2 != NULL && pressedQ) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AB2"));
		FVector CharacterRotation = UKismetMathLibrary::GetForwardVector(GetActorRotation());
		FVector loc = IntObjRef2->GetActorForwardVector();
		FRotator rot = loc.Rotation();
		IntObjRef2->PoltergeistII(GetActorLocation(), rot);
	}

}

void AGhostForHireCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	pressedQ = false;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (CurveFloat)
	{
		// Create the timeline float
		FOnTimelineFloat TimelineProgress;

		// Bind the function you want with TimelineProgress
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));

		// Interpolate between the curve and float
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);

		// Set the length - doesnt actually work rn because of CurveFloat
		CurveTimeline.SetTimelineLength(5.0f);

		// Can it loop? No
		CurveTimeline.SetLooping(false);

		// Set where the timeline should play
		CurveTimeline.PlayFromStart();

		// Tell the timeline what to do after it is finished
		CurveTimeline.SetTimelineFinishedFunc(endTimeline);

	}

	PlayerControllerRef = UGameplayStatics::GetPlayerController(this, 0);

	//K_Screech = 1;
	//K_PoltergeistIII = 2;
	//K_EctoHurl = 3;

	//K_1 = 7;
	//K_2 = 8;
	//K_3 = 4;
	//K_4 = 10;

	//GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	ToggleFlyingMode();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);

	canCameraTurn = true;
}

void AGhostForHireCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!InteractActive && !PoltergeistActive && !isColdTouchRunning) // This might break things <<<<<<<<<<<<<<<<<<<<<<<<<<<====================================================
	{
		SphereTrace();
	}

	KeyPressed();

	LightSwitchInteraction();

	// Check to see if the reference has been made and P3 is up as well as Q not being pressed
	if (AB_PoltergeistIII && IntObjRef2 != NULL && pressedQ) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::White, TEXT("AB3 first part"));
		IntObjRef2->PoltergeistII(GetActorLocation(), GetActorRotation());
		TimeCount += DeltaTime;
		if(TimeCount >= .5f)
			IntObjRef2->canThrow = true;
		if (IntObjRef2->canThrow) {
			PoltergeistIII();
		}
		if (IntObjRef2->isThrown)
		{
			if (IntObjRef2 != NULL) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Deactivated"));
				IntObjRef2->isAbilityActive = false;
				IntObjRef2->isThrown = false;
				IntObjRef2->canThrow = false;
				// new line seems to work for projectile
				IntObjRef2->Projectile->bSimulationEnabled = false;
				IntObjRef2 = NULL;
				pressedQ = false;
				TimeCount = 0;


			}
		}
	}
	if(BI || BII || BIII || BIV) 	CheckKeyAbilities();

	//if (BI && !InteractActive && !BII && !BIII)
	//{
	//	CheckKeyBindingsAbilities(1);
	//}

	//if (BII && !InteractActive && !BI && !BIII)
	//{
	//	CheckKeyBindingsAbilities(2);
	//}

	//if (BIII && !InteractActive && !BII && !BI)
	//{
	//	CheckKeyBindingsAbilities(3);
	//}
	// Handles the rattling interaction
	if (InteractActive && IntObjRef2 != NULL)
	{
		IntObjRef2->isObjHighlighted = true;
		IntObjRef2->PoltergeistI();
		RattleCount += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%d"), RattleCount));
	}
	if (RattleCount >= 5.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("We Done Rattling"));
		IntObjRef2->isPoltergeistActive = false;
		RattleCount = 0.0f;
		InteractActive = false;
		IntObjRef2->isAbilityActive = false;
		IntObjRef2->isObjHighlighted = false;
		IsRattleCooldown = true;
		if (IntObjRef2 != NULL)
		{
			IntObjRef2 = NULL;
		}

	}

	//if(isTouchingDoorOrWindow) //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("knock"));
	StartAbilityCooldown(DeltaTime);
	//P3_Cooldown += DeltaTime;
	//if (P3_Cooldown >= 5.0f)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("getting here"));
	//	P3_Cooldown = 0.f;
	//	IsP3Cooldown = false;
	//}
	CheckAbilityCooldown();
	
	if (IntObjRef2 != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("entered the loop"));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("hashit: %d"), IntObjRef2->hasHit));
		if (IntObjRef2->hasHit)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("We are hitting hasHit"));
			DestroyPoltergeistObject();
			IntObjRef2 = NULL;
		}
	}
	
	//echoLocationCharge = 100.f;
	EchoLocationRecharge();
	//if (echoLocationOn)
	//{
	//	echoLocationCharge -= 1.f;
	//}
	//GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Yellow, FString::Printf(TEXT("%d"), echoLocationCharge));

	checkWallDistance();

	if (cameraReady)
	{
		lerpCamera(DeltaTime);
	}

	if (resetArmLength)
	{
		float alength = CameraBoom->TargetArmLength;
		float arm = FMath::FInterpTo(alength, 400.f, DeltaTime, 10.0f);
		CameraBoom->TargetArmLength = arm;
		GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::White, TEXT("Getting here"));
		if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, 400.f, 10.f))
		{
			resetArmLength = false;
			GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Green, TEXT("Reset"));
		}

	}

	if (IsShiverCooldown)
	{
		GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Cyan, TEXT("Shivering is on cooldown"));
	}
	//GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Yellow, FString::Printf(TEXT("hitai: %d"), HitAI));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, FString::Printf(TEXT("camera position")));

	if (scaredState == true)
	{
		scareAnimTimer += DeltaTime;
	}

	if (scareAnimTimer >= 1.f)
	{
		scaredState = false;
		scareAnimTimer = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
// Input




void AGhostForHireCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		


		//Flying sop
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGhostForHireCharacter::StopFlying);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGhostForHireCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGhostForHireCharacter::Look);

		//Descending
		EnhancedInputComponent->BindAction(DescendAction, ETriggerEvent::Triggered, this, &AGhostForHireCharacter::DescendMovement);

		//EnhancedInputComponent->BindAction(DescendAction, ETriggerEvent::Started, this, &AGhostForHireCharacter::ToggleFlyingMode);

		//EnhancedInputComponent->BindAction(DescendAction, ETriggerEvent::Started, this, &AGhostForHireCharacter::StartFlying);

		//EnhancedInputComponent->BindAction(DescendAction, ETriggerEvent::Completed, this, &AGhostForHireCharacter::ToggleFlyingMode);

		//EnhancedInputComponent->BindAction(DescendAction, ETriggerEvent::Completed, this, &AGhostForHireCharacter::StopFlying);



		//Changes modes from Walking to Flying and vice versa
		//EnhancedInputComponent->BindAction(ToggleFlyAndWalk, ETriggerEvent::Started, this, &AGhostForHireCharacter::ToggleFlyingMode);

		//Flying
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGhostForHireCharacter::FlyMovement);

		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGhostForHireCharacter::ToggleFlyingMode);

		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGhostForHireCharacter::StartFlying);

		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGhostForHireCharacter::ToggleFlyingMode);

		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGhostForHireCharacter::StopFlying);


		//Play Audio
		EnhancedInputComponent->BindAction(PlayAudioInputAction, ETriggerEvent::Started, this, &AGhostForHireCharacter::PlayAudio);

		EnhancedInputComponent->BindAction(ScareInputAction, ETriggerEvent::Started, this, &AGhostForHireCharacter::ScareInteraction);
		

		//Changes modes from Walking to Flying and vice versa
		EnhancedInputComponent->BindAction(AbilityI, ETriggerEvent::Started, this, &AGhostForHireCharacter::Ability_I);
		//EnhancedInputComponent->BindAction(AbilityI, ETriggerEvent::Completed, this, &AGhostForHireCharacter::ResetKeyBooleans);

		//Changes modes from Walking to Flying and vice versa
		EnhancedInputComponent->BindAction(AbilityII, ETriggerEvent::Started, this, &AGhostForHireCharacter::Ability_II);
		//EnhancedInputComponent->BindAction(AbilityII, ETriggerEvent::Completed, this, &AGhostForHireCharacter::ResetKeyBooleans);

		//Changes modes from Walking to Flying and vice versa
		EnhancedInputComponent->BindAction(AbilityIII, ETriggerEvent::Started, this, &AGhostForHireCharacter::Ability_III);
		//EnhancedInputComponent->BindAction(AbilityIII, ETriggerEvent::Completed, this, &AGhostForHireCharacter::ResetKeyBooleans);

		//Ability 4
		EnhancedInputComponent->BindAction(AbilityIV, ETriggerEvent::Started, this, &AGhostForHireCharacter::Ability_IV);

		EnhancedInputComponent->BindAction(AbilityEchoLocation, ETriggerEvent::Started, this, &AGhostForHireCharacter::EchoLocation);
		EnhancedInputComponent->BindAction(AbilityEchoLocation, ETriggerEvent::Completed, this, &AGhostForHireCharacter::EchoLocationEnd);

	}

}

void AGhostForHireCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGhostForHireCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//Looker = Value.Get<FVector2D>();

	//mouseSense = 1.00f;
	//int v = 5;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("x: %f y: %f"), LookAxisVector.X, LookAxisVector.Y));
	//GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Yellow, FString::Printf(TEXT("hitai: %d"), HitAI));

	//FVector2D temp = LookAxisVector.Zero();

	float NewLookerX = LookAxisVector.X * mouseSense;
	float NewLookerY = LookAxisVector.Y * mouseSense;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		//AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y);

		//AddControllerYawInput(Looker.X);
		//AddControllerPitchInput(Looker.Y);

		AddControllerYawInput(NewLookerX);
		AddControllerPitchInput(NewLookerY);
	}
}


//AI stimuls Setup for Hearing
void AGhostForHireCharacter::SetupStimuls()
{
	stimulSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimul"));

	stimulSource->RegisterForSense(TSubclassOf<UAISense>());
	stimulSource->RegisterWithPerceptionSystem();
}

//Input Function for Playing Audio 
void AGhostForHireCharacter::PlayAudio()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Playing Audio"));

	isPlayingAudio = true;

	AudioLocation = GetActorLocation();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScreechSound, AudioLocation);

	UNiagaraFunctionLibrary::SpawnSystemAttached(ScreechVFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

	AICanAudioMove = true; 
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), AudioLocation, 1.f, this, 0.f, FName(TEXT("CanHear")));



}



//================================================================================Custom Code Starts Below Here=======================================|

void AGhostForHireCharacter::ToggleFlyingMode()
{
	// Changing the collisions profile |-------------------------------COME BACK TO THIS LATER WHEN COLLISION IS SETUP
	FName CustomPresetName = FName("NoFly");
	UCollisionProfile* CollisionProfile = UCollisionProfile::Get();

	if (isFlying)
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Walking)
		{

			// Change movement mode to flying
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);


			// Update character's collision settings for flying

			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);

			//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//GetCapsuleComponent()->SetSimulatePhysics(false);
			//GetCapsuleComponent()->setdisablecolli
			// Disable gravity for flying
			GetCharacterMovement()->GravityScale = 0.0f;

			GetCharacterMovement()->BrakingFriction = 5.0f;

			isFlying = true;
		}
		else if (GetCharacterMovement()->MovementMode == MOVE_Flying) //goBackToCol == false) ===== REMEMBER THIS, it is needed to get the toggle to NOT activate inside meshes <----------------
		{
			isFlying = false;

			// Change movement mode back to walking
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);

			//GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);


			// Enable gravity for walking
			GetCharacterMovement()->GravityScale = 1.75f;

			GetCharacterMovement()->BrakingFriction = 1.0f;



		}

	}
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);

	GetCharacterMovement()->GravityScale = 0.0f;

	GetCharacterMovement()->BrakingFriction = 10.0f;
	
}

void AGhostForHireCharacter::KnockOrScratch()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("knock"));

	AudioLocation = GetActorLocation();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), KnockSound, AudioLocation);

	AICanAudioMove = false;
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), AudioLocation, 1.f, this, 0.f, FName(TEXT("CanHear")));

}


void AGhostForHireCharacter::FlyMovement()
{

	if (GetCharacterMovement()->MovementMode == MOVE_Flying) //&& staminaProperty >= 1.0f
	//if (isFlying)
	{

		//GetCharacterMovement()->Velocity = FVector::ZeroVector;
		//GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 10.f);

		// =========================================== M E T H O D____1 ================================
		const FRotator Rotation = Controller->GetControlRotation();
		
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FRotator YawRotation(0, 0, 0);
		const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z); // rotation bug, but one problem at a time
		//const FVector UpDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);
		//const FVector UpDirection = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::Z);
		//const FVector UpDirection = FVector(0, 0, 1);

		//AddMovementInput(UpDirection, 1.0f);

		// =========================================== M E T H O D____2 ================================ THIS IS THE WAY THIS ONE IS WORKING

		//GetCharacterMovement()->AddImpulse(FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 300.f)); // doesnt work really

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf("%d", GetActorLocation()));

		FVector player = GetActorLocation();

		//FVector newpos = GetActorLocation() + FVector(0, 0, 10.f); // use for vinterp
		//FVector newpos = GetActorLocation() + FVector(0, 0, 1000.f); // use for Vinterpconstant
		FVector newpos = GetActorLocation() + FVector(0, 0, 10.f);

		//FVector placer = FMath::VInterpTo(player, newpos, GetWorld()->GetDeltaSeconds(), 50.f); // lower float = slower time
		//FVector placer = FMath::VInterpConstantTo(player, newpos, GetWorld()->GetDeltaSeconds(), 200.f); // higher float = faster time
		FVector placer = FMath::Lerp(player, newpos, .5f);
		//FMath::FInterpTo()
		
		//SetActorLocation(placer);
		//AddMovementInput(UpDirection, 1.0f);
		AddMovementInput(UpDirection, 1.0f);
		//SetActorRotation(GetActorRotation());
		//AddMovementInput(placer, 1.0f);
		isFlying = true;
		 

	}

	//isFlying = true;

}

void AGhostForHireCharacter::DescendMovement()
{
	if (GetCharacterMovement()->MovementMode == MOVE_Flying) {
	//if (isFlying) {
		//GetCharacterMovement()->Velocity = FVector::ZeroVector;
		//GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, -320.f);

		// =========================================== M E T H O D____1 ================================
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FRotator YawRotation(0, 0, 0);
		const FVector DownDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

		AddMovementInput(DownDirection, -1.0f);

		//isFlying = true;



		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		//const FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

		//// Add movement in the Z-axis
		//AddMovementInput(UpDirection, -1.0f);

		isFlying = true;
	}

	//isFlying = true;
}

void AGhostForHireCharacter::StopFlying()
{
	isFlying = false;
	GetCharacterMovement()->MovementMode == MOVE_Walking;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("stopping fly"));
}

void AGhostForHireCharacter::StartFlying()
{
	//ToggleFlyingMode();
	isFlying = true;
	GetCharacterMovement()->MovementMode == MOVE_Flying;	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("started fly"));
}

void AGhostForHireCharacter::ScareInteraction()
{
	if (AvgJoe != nullptr)
	{
		if (isScared == true)
		{
				AvgJoe->bScared = true;

				scaredState = true;
		}
	}
}



void AGhostForHireCharacter::SphereTrace()
{
	// Gets the location of the player
	FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();
	//FVector StartLocation = FollowCamera->GetComponentLocation();

	// Sets the location of how far the sphere trace will be
	FVector EndLocation = StartLocation + (FollowCamera->GetForwardVector() * 250.0f);

	// Create an array to ignore actors, really for just the player
	TArray<AActor*> ActorsToIgnore;

	// This ignores the player character touching the player
	ActorsToIgnore.Add(this);

	// Here is where we will get a reference to our hit actor
	FHitResult OutHit;

	// Magic happens here, it will get a SINGLE SPHERE TRACE and convert that information into a boolean
	Hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true, FLinearColor::Red, FLinearColor::Blue, 60.0f);

	if (Hit) 
	{
		CanInteract = true;

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Thats a hit"));
		IntObjRef = Cast<AInteractableObject>(OutHit.GetActor());

		hitActorString = OutHit.GetActor()->GetName();

		if (OutHit.GetActor()->ActorHasTag(FName("LampPost1")))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("LampPost"));
			if (BI || BII || BIII)
			{
				LP1 = true;
				LP2 = false;
				LP3 = false;
				LP4 = false;
				LP5 = false;
				LP6 = false;

			}
			//OutHit.GetActor()->
		}
		else if (OutHit.GetActor()->ActorHasTag(FName("LampPost2")))
		{
			if (BI || BII || BIII)
			{
				LP1 = false;
				LP2 = true;
				LP3 = false;
				LP4 = false;
				LP5 = false;
				LP6 = false;

			}
		}
		else if (OutHit.GetActor()->ActorHasTag(FName("LampPost3")))
		{
			if (BI || BII || BIII)
			{
				LP1 = false;
				LP2 = false;
				LP3 = true;
				LP4 = false;
				LP5 = false;
				LP6 = false;

			}
		}
		else if (OutHit.GetActor()->ActorHasTag(FName("LampPost4")))
		{
			if (BI || BII || BIII)
			{
				LP1 = false;
				LP2 = false;
				LP3 = false;
				LP4 = true;
				LP5 = false;
				LP6 = false;

			}
		}
		else if (OutHit.GetActor()->ActorHasTag(FName("LampPost5")))
		{
			if (BI || BII || BIII)
			{

				LP1 = false;
				LP2 = false;
				LP3 = false;
				LP4 = false;
				LP5 = true;
				LP6 = false;
			}
		}
		else if (OutHit.GetActor()->ActorHasTag(FName("LampPost6")))
		{
			if (BI || BII || BIII)
			{

				LP1 = false;
				LP2 = false;
				LP3 = false;
				LP4 = false;
				LP5 = false;
				LP6 = true;
			}
		}

		// If there is a HIT and Q has not been pressed or is currently false then set the 2nd Interactable Object References to the other one, it will store this so we can refer to it instead of a new one
		if (!pressedQ) {
			IntObjRef2 = IntObjRef;
			//OutHitActorTemp = OutHitActor;
		}

		// Highlights the object that the sphere trace touches always
		if (IntObjRef) 
		{
			IntObjRef->HighlightObject();
			//IntObjRef->isObjHighlighted = true;
		}

		AvgJoe = Cast<AAverageJoeAI>(OutHit.GetActor());
		//FString Hitter = FString::Printf(TEXT("Hit actor: %s"), *OutHit.GetActor()->GetName());
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, Hitter);
		if (AvgJoe)
		{

			if (AvgJoe->ScareLevel >= 100)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reached the Scare Level"));

				AvgJoe->bScaredState = true;

				isScared = true;
			}
			
			// Magic Formula
			float direction = AvgJoe->GetActorTransform().InverseTransformPosition(GetActorLocation()).X;

			if (direction < 0.0f)
			{
				canColdTouch = true;
				
				GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::White, TEXT("Thats a hitpawn"));
				//KeyPressed();
			}
			else 
			{
				
				canColdTouch = false;
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Thats NOT a hitpawn"));
			}
		}

		

		
		interactableObject = Cast<AAI_Interactable>(OutHit.GetActor());

		//if (isInteracting == true)
		//{
		//	LightSwitchInteraction(AIInteractable_Object);  // this is crashing my thing i think: It's crashing because we have to set the trace channels

		//}
		if(interactableObject != NULL)
			vfxLocation = interactableObject->GetActorLocation();

		if (IntObjRef2 != NULL)
		{
			if (OutHit.GetActor()) 
			{
				isTouchingDoorOrWindow = true;
			}
			else
			{
				isTouchingDoorOrWindow = false;
			}
		}
		UPrimitiveComponent* HitComponent = OutHit.Component.Get();
		if (HitComponent->ComponentHasTag(FName("Surface")))
		{
			isTouchingDoorOrWindow = true;
			//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::White, TEXT("Thats a hitpawn"));
		}

		if (interactableObject != NULL)
		{
			interactableObject->Highlight();
		}

		if (HitComponent->ComponentHasTag(FName("Chair")))
		{
			IntObjRef->ChairRattle = true;
			IntObjRef->VaseRattle = false;
			IntObjRef->MetalRattle = false;
			IntObjRef->FlowerRattle = false;
		}

		if (HitComponent->ComponentHasTag(FName("Vase")))
		{
			IntObjRef->ChairRattle = false;
			IntObjRef->VaseRattle = true;
			IntObjRef->MetalRattle = false;
			IntObjRef->FlowerRattle = false;
		}

		if (HitComponent->ComponentHasTag(FName("Metal")))
		{
			IntObjRef->ChairRattle = false;
			IntObjRef->VaseRattle = false;
			IntObjRef->MetalRattle = true;
			IntObjRef->FlowerRattle = false;
		}

		if (HitComponent->ComponentHasTag(FName("Flower")))
		{
			IntObjRef->ChairRattle = false;
			IntObjRef->VaseRattle = false;
			IntObjRef->MetalRattle = false;
			IntObjRef->FlowerRattle = true;
		}
	
	} 
	else 
	{
		CanInteract = false;
		isTouchingDoorOrWindow = false;
		hitActorString = "";
		isScared = false;
		HitAI = false;
		canColdTouch = false;
		if (interactableObject != NULL)
		{
			interactableObject->DontHighlight();
		}
	}
	//CanInteract = true; // I CHANGE IT HERE BTW <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<=================================================

	// If there was a reference created to the alternative IOR and Interact Key is NOT pressed yet, then do the Poltergeist
	if (IntObjRef2)
	{

		if (pressedQ) { // need Hit here to check something
			//CurveTimeline.TickTimeline(GetWorld()->GetDeltaSeconds()); // used this formerly to get it done, however im using dopoltergeistability() now
			//IntObjRef2->isObjHighlighted = true;
			if (AB_PoltergeistI || AB_PoltergeistII)
			{
				DoPoltergeistAbility();
			}

		}
	}
}

void AGhostForHireCharacter::DoPoltergeistAbility()
{
	CurveTimeline.TickTimeline(GetWorld()->GetDeltaSeconds());
}

void AGhostForHireCharacter::KeyPressed()
{
	// I believe its IntObjRef not IntObjRef2
	if (!pressedQ && PlayerControllerRef->WasInputKeyJustPressed(EKeys::Q)) {

		// If P1 is active
		if (AB_PoltergeistI) 
		{
			// And if the reference is NOT NULL and the reference is highlighted or hit
			if (IntObjRef2 != NULL && IntObjRef2->isObjHighlighted)
			{
				pressedQ = true;
			}
		}
		// If P2 is active
		else if (AB_PoltergeistII)
		{
			// And if the reference is NOT NULL and the reference is highlighted or hit
			if (IntObjRef2 != NULL && IntObjRef2->isObjHighlighted)
			{
				pressedQ = true;
			}
		}
		// If P3 is active
		else if (AB_PoltergeistIII)
		{
			// And if the reference is NOT NULL and the reference is highlighted or hit
			if (IntObjRef2 != NULL && IntObjRef2->isObjHighlighted)
			{
				pressedQ = true;
			}
		}
		// If P4 is active
		else if (AB_PoltergeistIV)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("interactable Key Pressed Q pressed"));

			PoltergeistIV();
		}
		else if (AB_PoltergeistV) 
		{
			PoltergeistV();
		}
		else if (AB_EctoHurl)
		{
			EctoHurl();
		}
		else if (AB_EctoSplat)
		{
			EctoSplat();
		}
	}

	if (!pressedQ && PlayerControllerRef->WasInputKeyJustPressed(EKeys::Q) && HitAI)
	{
		pressedQ = true;
	}
}

void AGhostForHireCharacter::EctoHurl()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Spawned a Mesh"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FVector Location = GetActorLocation(); //+ FVector(100.f, 100.f, 0);
		//FVector ForwardVector = GetActorForwardVector() * 100.f; //+ FVector(100.f, 100.f, 0);
		FVector ForwardVector = FollowCamera->GetForwardVector() * 100.f;
		FVector ForwardLocation = Location + ForwardVector;
		FRotator Rotation = GetActorRotation();

		AEctoSplatObject* Hurl = World->SpawnActor<AEctoSplatObject>(EctoSplatClass, ForwardLocation, Rotation);

		//Hurl->Mesh->SetSimulatePhysics(true);
		//Hurl->Mesh->AddImpulse(ForwardVector * 100.f);
		Hurl->Projectile->AddForce(ForwardVector * 100.f);
		//Hurl->OnOverlapBegin
	}
}

void AGhostForHireCharacter::EctoSplat()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("EctoSplat"));

	UWorld* const World = GetWorld();
	//AEctoSplatObject* Hurl;
	if (World != nullptr)
	{
		FVector Location = GetActorLocation(); //+ FVector(100.f, 100.f, 0);
		Location = FVector(GetActorLocation().X, GetActorLocation().Y, (GetActorLocation().Z - 100.f));
		FVector ForwardVector = GetActorForwardVector() * 100.f; //+ FVector(100.f, 100.f, 0);
		//FVector ForwardVector = FollowCamera->GetForwardVector() * 100.f;
		FVector ForwardLocation = Location + ForwardVector;
		FRotator Rotation = GetActorRotation();

		
		AEctoSplatObject* Hurl = World->SpawnActor<AEctoSplatObject>(EctoSplatClass, ForwardLocation, Rotation);
		Hurl->Projectile->ProjectileGravityScale = 1;


		//Hurl->EctoSplatActive = true;

		//Hurl->Projectile->InitialSpeed = 0;
		//Hurl->Projectile->MaxSpeed = 0;

		//Hurl->Mesh->SetSimulatePhysics(true);
		//Hurl->Mesh->AddImpulse(ForwardVector * 100.f);
		//Hurl->Projectile->AddForce(ForwardVector * 1.f);
		//Hurl->OnOverlapBegin
	}
}

void AGhostForHireCharacter::LightSwitchInteraction()
{
	if (interactableObject != nullptr)
	{
		if (interactableObject->Label == "AIInteractable" && isInteracting == true)
		{
			LightState = false;
			LightSwitch = interactableObject;

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Light Switch Value: %s"), *LightSwitch->GetFName().ToString()));

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Light OFF"));

			AI_InvestigateLight = true;

		}


	}
}


void AGhostForHireCharacter::PoltergeistIV()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Spawned a Mesh"));

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();

		AWaveMesh* Wave = World->SpawnActor<AWaveMesh>(WaveMeshClass, Location, Rotation);



	}

}

void AGhostForHireCharacter::PoltergeistV()
{
	FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();

	// Sets the location of how far the sphere trace will be
	FVector EndLocation = StartLocation + (GetActorForwardVector() * 100.0f);

	// Create an array to ignore actors, really for just the player
	TArray<AActor*> ActorsToIgnore;

	// This ignores the player character touching the player
	ActorsToIgnore.Add(this);

	// Here is where we will get a reference to our hit actor
	TArray<FHitResult> OutHit;

	bool hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, EndLocation, PolterGeistVRadius,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true,
		FLinearColor::Red, FLinearColor::Green, 1);

	for (int i = 0; i < OutHit.Num(); i++)
	{
		AInteractableObject* InteractableObj = Cast<AInteractableObject>(OutHit[i].GetActor());

		if (hit)
		{
			if (OutHit[i].GetActor() == InteractableObj)
			{

				InteractableObj->PlayerLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 250, GetActorLocation().Z);

				/*FVector InteractableLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 250, GetActorLocation().Z);

				OutHit[0].GetActor()->SetActorLocation(InteractableLocation);*/

				//InteractableObj->PlayerLocation = OutHit[0].GetActor()->GetActorLocation();


				InteractableObj->canRotate = true;
			}

		}
	}

}

void AGhostForHireCharacter::DestroyPoltergeistObject()
{
	// Dont need to clear references
	if (IntObjRef2->isFragile)
	{
		IntObjRef2->Destroy();
	}
	if (IntObjRef2->isSturdy)
	{
		IntObjRef2->destroyCount += 1;
		if (IntObjRef2->destroyCount >= 3) {
			IntObjRef2->Destroy();

		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("destroy count: %d"), IntObjRef2->destroyCount));
	}
	if (IntObjRef2->isTough)
	{
		IntObjRef2->destroyCount += 1;
		if (IntObjRef2->destroyCount >= 5) {
			IntObjRef2->Destroy();

		}
	}
}

// This will reset the Timeline changes for Poltergeist 1 2 and 3 and clear all pointers created
void AGhostForHireCharacter::ResetTimeline()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Q FALSE"));
	pressedQ = false;
	CurveTimeline.SetNewTime(0.0f);
	CurveTimeline.PlayFromStart();
	//IntObjRef2 = NULL;

	// Enables physics
	if (AB_PoltergeistII && IntObjRef2 != NULL)
	{
		IntObjRef2->Mesh->SetSimulatePhysics(true);
		//IntObjRef2->Root->SetupAttachment(IntObjRef->Mesh);
	}

	// Clear the cast
	if (IntObjRef != NULL) {
		IntObjRef->isAbilityActive = false;
		IntObjRef = NULL;
	}

	// Clear the temporary cast
	if (IntObjRef2 != NULL) {
		IntObjRef2->isAbilityActive = false;
		IntObjRef2 = NULL;
	}



}

// This will allow the player to pickup the item and throw it
void AGhostForHireCharacter::PoltergeistIII()
{
	
	//if (PlayerControllerRef->WasInputKeyJustPressed(EKeys::Q) && !IntObjRef2->isThrown) {
	//	FVector Direction = GetActorForwardVector();
	//	FVector noSpeed = FVector::ZeroVector;
	//	float velocity = 1000000.f;
	//	IntObjRef2->isThrown = true;
	//	// new line i created
	//	IntObjRef2->Projectile->bSimulationEnabled = true;
	//	IntObjRef2->Mesh->SetSimulatePhysics(true);

	//	if (this->GetVelocity() != noSpeed) {
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("moving"));
	//		IntObjRef2->Mesh->AddImpulse(Direction * 300000.f);
	//	}
	//	else {
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("not moving"));
	//		//IntObjRef2->Mesh->AddImpulse(Direction * 100000.f + this->GetVelocity());
	//		IntObjRef2->Mesh->AddImpulse(Direction * 100000.f);
	//	}

	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Playing this"));
	//}

	if (((K_1 == 1 && PlayerControllerRef->WasInputKeyJustPressed(EKeys::One)) || (K_2 == 1 && PlayerControllerRef->WasInputKeyJustPressed(EKeys::Two)) || (K_3 == 1 && PlayerControllerRef->WasInputKeyJustPressed(EKeys::Three))) || (K_4 == 1 && PlayerControllerRef->WasInputKeyJustPressed(EKeys::Four)) && !IntObjRef2->isThrown) {
	//if (PlayerControllerRef->WasInputKeyJustPressed(EKeys::Two) && !IntObjRef2->isThrown) { // fix this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		FVector Direction = GetActorForwardVector();
		FVector noSpeed = FVector::ZeroVector;
		float velocity = 1000000.f;
		IntObjRef2->isThrown = true;
		// new line i created
		IntObjRef2->Projectile->bSimulationEnabled = true;
		IntObjRef2->Mesh->SetSimulatePhysics(true);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PoltergeistThrowSound, AudioLocation);


		if (this->GetVelocity() != noSpeed) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("moving"));
			IntObjRef2->Mesh->AddImpulse(Direction * 300000.f);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("not moving"));
			//IntObjRef2->Mesh->AddImpulse(Direction * 100000.f + this->GetVelocity());
			IntObjRef2->Mesh->AddImpulse(Direction * 100000.f);
		}
	}

}


void AGhostForHireCharacter::Ability_I()
{
	if(K_1 == 1 && !IsP3Cooldown && IntObjRef2 != NULL)
	{
		BI = true;
	} 
	else if (K_1 == 2 && !IsScreechCooldown)
	{
		BI = true;
	}
	else if (K_1 == 3 && !IsP4Cooldown)
	{
		BI = true;
	}
	else if (K_1 == 4 && !IsEctoHurlCooldown)
	{
		BI = true;
	}
	else if (K_1 == 5 && !IsEctoSplatCooldown)
	{
		BI = true;
	}
	else if (K_1 == 6 && !IsTornadoCooldown)
	{
		BI = true;
	}
	else if (K_1 == 7 && !IsColdTouchCooldown && AvgJoe != NULL && canColdTouch)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ColdTouchSound, GetActorLocation());
		AvgJoe->ColdHitAI = true;
		BI = true;
		//isColdTouchRunning = true;
	}
	else if (K_1 == 8 && !IsKnockCooldown)
	{
		BI = true;
	}
	else if (K_1 == 9 && !IsRattleCooldown)
	{
		BI = true;
	}
	else if (K_1 == 10 && !IsTamperCooldown)
	{
		BI = true;
	}
	else if (K_1 == 11 && !IsShiverCooldown)
	{
		BI = true;
	}

}

void AGhostForHireCharacter::Ability_II()
{
	
	//if (K_2 == 0) 
	//{
	//	if (IntObjRef2 != NULL)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("AB2"));
	//		BII = true;
	//	}
	//}
	//else {
	//	BII = true;
	//}
	//BII = true; // when the ABII is hit, it turns true, need to make sure it is false somehow that way it doesnt auto move
	if (K_2 == 1 && !IsP3Cooldown && IntObjRef2 != NULL && Hit && !InteractActive)
	{
		BII = true;
	}
	else if (K_2 == 2 && !IsScreechCooldown)
	{
		BII = true;
	}
	else if (K_2 == 3 && !IsP4Cooldown)
	{
		BII = true;
	}
	else if (K_2 == 4 && !IsEctoHurlCooldown)
	{
		BII = true;
	}
	else if (K_2 == 5 && !IsEctoSplatCooldown)
	{
		BII = true;
	}
	else if (K_2 == 6 && !IsTornadoCooldown)
	{
		BII = true;
	}
	else if (K_2 == 7 && !IsColdTouchCooldown && AvgJoe != NULL && canColdTouch)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ColdTouchSound, GetActorLocation());
		AvgJoe->ColdHitAI = true;

		BII = true;
		//isColdTouchRunning = true;
	}
	else if (K_2 == 8 && !IsKnockCooldown)
	{
		BII = true;
	}
	else if (K_2 == 9 && !IsRattleCooldown && IntObjRef2 != NULL && Hit)
	{
		BII = true;
	}
	else if (K_2 == 10 && !IsTamperCooldown)
	{
		BII = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("AB2"));
	}
	else if (K_2 == 11 && !IsShiverCooldown)
	{
		BII = true;
	}
}

void AGhostForHireCharacter::Ability_III()
{
	if (K_3 == 1 && !IsP3Cooldown && IntObjRef2 != NULL && Hit)
	{
		BIII = true;
	}
	else if (K_3 == 2 && !IsScreechCooldown)
	{
		BIII = true;
	}
	else if (K_3 == 3 && !IsP4Cooldown)
	{
		BIII = true;
	}
	else if (K_3 == 4 && !IsEctoHurlCooldown)
	{
		BIII = true;
	}
	else if (K_3 == 5 && !IsEctoSplatCooldown)
	{
		BIII = true;
	}
	else if (K_3 == 6 && !IsTornadoCooldown)
	{
		BIII = true;
	}
	else if (K_3 == 7 && !IsColdTouchCooldown && AvgJoe != NULL && canColdTouch)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ColdTouchSound, GetActorLocation());
		AvgJoe->ColdHitAI = true;

		BIII = true;
		//isColdTouchRunning = true;
	}
	else if (K_3 == 8 && !IsKnockCooldown)
	{
		BIII = true;
	}
	else if (K_3 == 9 && !IsRattleCooldown && IntObjRef2 != NULL && Hit)
	{
		BIII = true;
	}
	else if (K_3 == 10 && !IsTamperCooldown)
	{
		BIII = true;
	}
	else if (K_3 == 11 && !IsShiverCooldown)
	{
		BIII = true;
	}
}

void AGhostForHireCharacter::Ability_IV()
{
	if (K_4 == 1 && !IsP3Cooldown && IntObjRef2 != NULL && Hit)
	{
		BIV = true;
	}
	else if (K_4 == 2 && !IsScreechCooldown)
	{
		BIV = true;
	}
	else if (K_4 == 3 && !IsP4Cooldown)
	{
		BIV = true;
	}
	else if (K_4 == 4 && !IsEctoHurlCooldown)
	{
		BIV = true;
	}
	else if (K_4 == 5 && !IsEctoSplatCooldown)
	{
		BIV = true;
	}
	else if (K_4 == 6 && !IsTornadoCooldown)
	{
		BIV = true;
	}
	//else if (K_1 == 7 && !IsColdTouchCooldown)
	//{
	//	BI = true;
	//}
	//else if (K_1 == 8 && !IsScreechCooldown)
	//{
	//	BI = true;
	//}
	else if (K_4 == 9 && !IsRattleCooldown && IntObjRef2 != NULL && Hit)
	{
		BIV = true;
	}
	else if (K_4 == 10 && !IsTamperCooldown)
	{
		BIV = true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("AB4"));
}


void AGhostForHireCharacter::ResetKeyBooleans()
{
	BI = false;
	BII = false;
	BIII = false;
	BIV = false;
}
// 0 - 
void AGhostForHireCharacter::CheckKeyAbilities()
{

	if ((K_1 == 1 && BI) || (K_2 == 1 && BII) || (K_3 == 1 && BIII) || (K_4 == 1 && BIV))
	{
		//IntObjRef2->PoltergeistII(GetActorLocation(), GetActorRotation());
		//PoltergeistIII();
		//ResetKeyBooleans();
		//InteractActive = true;
		if (IntObjRef2 != NULL)
		{
			if (!InteractActive && !IsP3Cooldown)
			{
				PoltergeistActive = true;
				IntObjRef2->PoltergeistII(GetActorLocation(), GetActorRotation());
				TimeCount += GetWorld()->GetDeltaSeconds();
				if (TimeCount >= .5f)
					IntObjRef2->canThrow = true;
				if (IntObjRef2->canThrow)
				{
					PoltergeistIII();
				}
				if (IntObjRef2->isThrown)
				{
					if (IntObjRef2 != NULL)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Deactivated"));
						IntObjRef2->isAbilityActive = false;
						IntObjRef2->isThrown = false;
						IntObjRef2->canThrow = false;
						PoltergeistActive = false;
						//InteractActive = false;
						// new line seems to work for projectile
						IntObjRef2->Projectile->bSimulationEnabled = false;
						//IntObjRef2 = NULL;
						pressedQ = false;
						TimeCount = 0;
						IsP3Cooldown = true;
						ResetKeyBooleans();

					}
				}
			}
		}

	}
	else if ((K_1 == 2 && BI) || (K_2 == 2 && BII) || (K_3 == 2 && BIII) || (K_4 == 2 && BIV))
	{
		if (!IsScreechCooldown)
		{
			PlayAudio();
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Gtshere"));
			IsScreechCooldown = true;
			ResetKeyBooleans();
		}
	}
	else if ((K_1 == 3 && BI) || (K_2 == 3 && BII) || (K_3 == 3 && BIII) || (K_4 == 3 && BIV))
	{
		if (!IsP4Cooldown)
		{
			PoltergeistIV();
			IsP4Cooldown = true;
			ResetKeyBooleans();
		}

	}
	else if ((K_1 == 4 && BI) || (K_2 == 4 && BII) || (K_3 == 4 && BIII) || (K_4 == 4 && BIV))
	{
		if (!IsEctoHurlCooldown)
		{
			EctoHurl();
			IsEctoHurlCooldown = true;
			ResetKeyBooleans();
		}

	}
	else if ((K_1 == 5 && BI) || (K_2 == 5 && BII) || (K_3 == 5 && BIII) || (K_4 == 5 && BIV))
	{
		if (!IsEctoSplatCooldown) 
		{
			EctoSplat();
			IsEctoSplatCooldown = true;
			ResetKeyBooleans();

		}
	}
	else if ((K_1 == 6 && BI) || (K_2 == 6 && BII) || (K_3 == 6 && BIII) || (K_4 == 6 && BIV))
	{
		if (!IsTornadoCooldown)
		{
			PoltergeistV();
			IsTornadoCooldown = true;
			ResetKeyBooleans();
		}

	}
	else if ((K_1 == 8 && BI) || (K_2 == 8 && BII) || (K_3 == 8 && BIII) || (K_4 == 8 && BIV))
	{
		if (isTouchingDoorOrWindow)
		{
			KnockOrScratch();
			IsKnockCooldown = true;
		}
		ResetKeyBooleans();
	}
	else if ((K_1 == 9 && BI) || (K_2 == 9 && BII) || (K_3 == 9 && BIII) || (K_4 == 9 && BIV))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Getting in here"));
		if (!IsRattleCooldown)
		{
			if (Hit && IntObjRef2 != NULL) {
				InteractActive = true;
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), RattleVFX, IntObjRef2->GetActorLocation(), FRotator(0.f), FVector(1.f), true, true);
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Rattling"));
			ResetKeyBooleans();
		}

	}
	else if ((K_1 == 10 && BI) || (K_2 == 10 && BII) || (K_3 == 10 && BIII) || (K_4 == 10 && BIV))
	{

		//LightSwitchInteraction();
		//if (interactableObject != nullptr) 	GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Can Tampering"));
		if (!IsTamperCooldown)
		{
			if (interactableObject != nullptr)
			{
				if (interactableObject->Label == "AIInteractableLightSwitch" && Hit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Shold be Tampering"));
					LightState = false;
					LightsOFF = true;

					LightSwitch = interactableObject;

					AudioLocation = interactableObject->GetActorLocation();

					UGameplayStatics::PlaySoundAtLocation(GetWorld(), LightSound, AudioLocation);

					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Light Switch Value: %s"), *LightSwitch->GetFName().ToString()));

					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Light OFF"));

					IsTamperCooldown = true;

					isTampering = true;

					LightsAreOn = false;

				}

				if (interactableObject->Label == "AIInteractableFireplace" && Hit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Shold be Tampering"));
					LightState = false;
					FireplaceOFF = true;

					LightSwitch = interactableObject;

					AudioLocation = interactableObject->GetActorLocation();

					UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireplaceSound, AudioLocation);

					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Light Switch Value: %s"), *LightSwitch->GetFName().ToString()));

					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Light OFF"));

					//UNiagaraFunctionLibrary::SpawnSystemAttached(FireplaceVFX, vfxLocation, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
					if (interactableObject->ActorHasTag(FName("Candle")))
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CandleVFX, vfxLocation, FRotator(0.f), FVector(1.f), true, true);
					} 
					else
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireplaceVFX, vfxLocation, FRotator(0.f), FVector(1.f), true, true);
					}


					IsTamperCooldown = true;

					isTampering = true;

					LightsAreOn = false;

				}

				if (interactableObject->Label == "AIInteractableToilet" && Hit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Tampered Audio Object"));

					ToiletOFF = true;

					AudioLocation = interactableObject->GetActorLocation();

					UGameplayStatics::PlaySoundAtLocation(GetWorld(), ToiletSound, AudioLocation);

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ToiletVFX, vfxLocation, FRotator(0.f), FVector(1.f), true, true);

					AICanAudioMove = true;
					UAISense_Hearing::ReportNoiseEvent(GetWorld(), AudioLocation, 1.f, this, 0.f, FName(TEXT("CanHear")));
				}

				if (interactableObject->Label == "AIInteractableGraveBell" && Hit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Tampered Audio Object"));

					GravebellOFF = true;

					AudioLocation = interactableObject->GetActorLocation();

					UGameplayStatics::PlaySoundAtLocation(GetWorld(), GraveBellSound, AudioLocation);

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OrganVFX, vfxLocation, FRotator(0.f), FVector(1.f), true, true);

					AICanAudioMove = true;
					UAISense_Hearing::ReportNoiseEvent(GetWorld(), AudioLocation, 1.f, this, 0.f, FName(TEXT("CanHear")));
				}

				if (interactableObject->Label == "AIInteractableOrgan" && Hit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("Tampered Audio Object"));

					AudioLocation = interactableObject->GetActorLocation();

					UGameplayStatics::PlaySoundAtLocation(GetWorld(), OrganSound, AudioLocation);

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OrganVFX, vfxLocation, FRotator(0.f), FVector(1.f), true, true);

					AICanAudioMove = true;
					UAISense_Hearing::ReportNoiseEvent(GetWorld(), AudioLocation, 1.f, this, 0.f, FName(TEXT("CanHear")));
				}

				if (interactableObject->Label == "AIInteractableLantern" && Hit)
				{
					//AILantern = true;

					GEngine->AddOnScreenDebugMessage(-1, 5.10f, FColor::Purple, TEXT("lantern"));

					AICanAudioMove = true;
				}

			}
			ResetKeyBooleans();

		}
		//ResetKeyBooleans();
	}
	//else if ((K_1 == 11 && BI) || (K_2 == 11 && BII) || (K_3 == 11 && BIII) || (K_4 == 11 && BIV))
	//{
	//	if (!IsShiverCooldown)
	//	{
	//		IsShiverCooldown = true;
	//	}

	//}

}

void AGhostForHireCharacter::StartAbilityCooldown(float time)
{
	if (IsP3Cooldown)
	{
		P3_Cooldown += time;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("%d"), P3_Cooldown));
		GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Yellow, TEXT("p3 on cooldown"));
	}
	if (IsP4Cooldown)
	{
		P4_Cooldown += time;
	}
	if (IsTornadoCooldown)
	{
		Tornado_Cooldown += time;
	}
	if (IsEctoHurlCooldown)
	{
		EctoHurl_Cooldown += time;
		GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Yellow, TEXT("HURl on cooldown"));
	}
	if (IsEctoSplatCooldown)
	{
		EctoSplat_Cooldown += time;
	}
	if (IsScreechCooldown)
	{
		//GEngine->AddOnScreenDebugMessage(-1, .05f, FColor::Yellow, TEXT("Screech on CD"));
		Screech_Cooldown += time;
	}
	if (IsTamperCooldown)
	{
		Tamper_Cooldown += time;
	}
	if (IsRattleCooldown)
	{
		Rattle_Cooldown += time;
		GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Yellow, TEXT("RATTLE on cooldown"));
	}
	if (IsColdTouchCooldown)
	{
		ColdTouch_Cooldown += time;
	}
	if (IsShiverCooldown)
	{
		Shiver_Cooldown += time;
	}
	if (IsKnockCooldown)
	{
		Knock_Cooldown += time;
	}
	//if (P3_Cooldown >= 5.0f)
	//{
	//	IsP3Cooldown = false;
	//	P3_Cooldown = 0.0f;
	//	GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Green, TEXT("p3 over"));
	//}
	//if (P4_Cooldown >= 30.0f)
	//{
	//	IsP4Cooldown = false;
	//	P4_Cooldown = 0.0f;
	//}
	//if (Tornado_Cooldown >= 300.0f)
	//{
	//	IsTornadoCooldown = false;
	//	Tornado_Cooldown = 0.0f;
	//}
	//if (EctoHurl_Cooldown >= 15.0f)
	//{
	//	IsEctoHurlCooldown = false;
	//	EctoHurl_Cooldown = 0.0f;
	//}
	//if (EctoSplat_Cooldown >= 15.0f)
	//{
	//	IsEctoSplatCooldown = false;
	//	EctoSplat_Cooldown = 0.0f;
	//}
	//if (Screech_Cooldown >= 30.0f)
	//{
	//	IsScreechCooldown = false;
	//	Screech_Cooldown = 0.0f;
	//}
	//if (Tamper_Cooldown >= 15.0f)
	//{
	//	IsTamperCooldown = false;
	//	Tamper_Cooldown = 0.0f;
	//}
}

void AGhostForHireCharacter::CheckAbilityCooldown()
{
	//float cd_P3 = 15.0f;
	//float cd_P4 = 30.0f;
	//float cd_Tornado = 300.0f;
	//float cd_EctoHurl = 15.0f;
	//float cd_EctoSplat = 15.0f;
	//float cd_Screech = 30.0f;
	//float cd_Tamper = 15.0f;
	//float cd_Rattle = 5.0f;
	//float cd_ColdTouch = 20.0f;

	if (P3_Cooldown >= cd_P3)
	{
		IsP3Cooldown = false;
		P3_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("p3 over"));
	}
	if (P4_Cooldown >= cd_P4)
	{
		IsP4Cooldown = false;
		P4_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("p4 over"));
	}
	if (Tornado_Cooldown >= cd_Tornado)
	{
		IsTornadoCooldown = false;
		Tornado_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Nado over"));
	}
	if (EctoHurl_Cooldown >= cd_EctoHurl)
	{
		IsEctoHurlCooldown = false;
		EctoHurl_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("HURl over"));
	}
	if (EctoSplat_Cooldown >= cd_EctoSplat)
	{
		IsEctoSplatCooldown = false;
		EctoSplat_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Splat over"));
	}
	if (Screech_Cooldown >= cd_Screech)
	{
		IsScreechCooldown = false;
		Screech_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Screech over"));
	}
	if (Tamper_Cooldown >= cd_Tamper)
	{
		IsTamperCooldown = false;
		Tamper_Cooldown = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Tamper over"));
	}
	if (Rattle_Cooldown >= cd_Rattle)
	{
		IsRattleCooldown = false;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("rattle over"));
		Rattle_Cooldown = 0.0f;
	}
	if (ColdTouch_Cooldown >= cd_ColdTouch)
	{
		IsColdTouchCooldown = false;
		GEngine->AddOnScreenDebugMessage(-1, 10.10f, FColor::Green, TEXT("cold touch over in GFH Character"));
		ColdTouch_Cooldown = 0.0f;
	}
	if (Shiver_Cooldown >= cd_Shiver)
	{
		IsShiverCooldown = false;
		GEngine->AddOnScreenDebugMessage(-1, 10.10f, FColor::Green, TEXT("Shiver over"));
		Shiver_Cooldown = 0.0f;
	}
	if (Knock_Cooldown >= cd_Knock)
	{
		IsKnockCooldown = false;
		GEngine->AddOnScreenDebugMessage(-1, 10.10f, FColor::Green, TEXT("Knock over"));
		Knock_Cooldown = 0.0f;
	}
}

void AGhostForHireCharacter::ResetAbilityCooldowns()
{
	IsP3Cooldown = false;
	IsP4Cooldown = false;
	IsTornadoCooldown = false;
	IsEctoHurlCooldown = false;
	IsEctoSplatCooldown = false;
	IsScreechCooldown = false;
	IsTamperCooldown = false;
	IsRattleCooldown = false;
	IsShiverCooldown = false;
	IsKnockCooldown = false;
	//===========================
	P3_Cooldown, P4_Cooldown, Tornado_Cooldown, EctoHurl_Cooldown, EctoSplat_Cooldown, Screech_Cooldown, Tamper_Cooldown, Rattle_Cooldown, Shiver_Cooldown, Knock_Cooldown = 0.0f;
}

void AGhostForHireCharacter::checkWallDistance()
{
	if (leavingWallPosition != FVector::Zero())
	{
		//wallDistance = leavingWallPosition - GetActorLocation();
		float distance = FVector::Distance(GetActorLocation(), leavingWallPosition);
		//FString dis = leavingWallPosition.ToString();
		wallDistance = leavingWallPosition;
		//GEngine->AddOnScreenDebugMessage(-1, 1.10f, FColor::Green, dis);
		if (distance >= 100.f)
		{
			cameraReady = true;
			//GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Green, TEXT("Distance reached"));
			//leavingWallPosition = FVector::Zero();
		}
	}


}

void AGhostForHireCharacter::lerpCamera(float DeltaTime)
{
	// Need to change it to cameraboom

	//canCameraTurn = false;
	//FVector CameraCurrentLocation = FollowCamera->GetComponentLocation();
	//FVector CameraCurrentLocation = CameraBoom->GetComponentLocation();

	float interpSpeed = 5.0f;

	//FVector CameraLocation = FMath::VInterpTo(CameraCurrentLocation, wallDistance, DeltaTime, interpSpeed);

	FVector CameraLocation = FMath::VInterpTo(FollowCamera->GetComponentLocation(), leavingWallPosition, DeltaTime, interpSpeed);

	//FVector CameraLocation = FMath::Lerp(CameraCurrentLocation, leavingWallPosition, interpSpeed);

	//FollowCamera->SetWorldLocation(CameraLocation);
	//CameraBoom->SetWorldLocation(CameraLocation);

	//FString dis = distance.ToString();
	//GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Green, dis);

	//float distance = FVector::Distance(FollowCamera->GetComponentLocation(), leavingWallPosition);

	//float distance = FVector::Distance(CameraBoom->TargetArmLength, leavingWallPosition);

	//USpringArmComponent::

	//GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Cyan, FString::Printf(TEXT("distance: %d"), CameraBoom->TargetArmLength));
	//if (distance <= 30.0f) 
	//{
	//	cameraReady = false;
	//	GEngine->AddOnScreenDebugMessage(-1, .10f, FColor::Orange, FString::Printf(TEXT("distance: %d"), distance));
	//	//if (!cameraReady) leavingWallPosition = FVector::Zero();
	//	leavingWallPosition = FVector::Zero();
	//	canCameraTurn = true;
	//}
	//else 
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.10f, FColor::Red, TEXT("Nope"));
	//}

	float desiredArmLength = 300.f;


	// camera boom shit
	//float arm = FMath::Lerp(CameraBoom->TargetArmLength, 100.f, 0.50f);
	if (cameraReady)
	{
		float alength = CameraBoom->TargetArmLength;
		float arm = FMath::FInterpTo(alength, desiredArmLength, DeltaTime, interpSpeed);
		CameraBoom->TargetArmLength = arm;

	}

	if (FMath::IsNearlyEqual(CameraBoom->TargetArmLength, desiredArmLength, 10.f))
	{
		//Mesh->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Block);
		//CameraBoom->TargetArmLength = 400.f;
		GEngine->AddOnScreenDebugMessage(-1, 1.10f, FColor::Purple, TEXT("Boom here"));
		//canCameraTurn = true;
		cameraReady = false;
		leavingWallPosition = FVector::Zero();
		resetArmLength = true;
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, .05f, FColor::Red, TEXT("Nope"));
	}

	//FVector loc;
	//bool bol;
	//CameraBoom->TargetArmLength = USpringArmComponent::BlendLocations(leavingWallPosition, loc, bol, DeltaTime);
	//USpringArmComponent::updatedesi
}

void AGhostForHireCharacter::EchoLocation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.10f, FColor::Orange, TEXT("Echo Location"));
	//if (echoLocationCharge == 100.f)
	//{
	//	echoLocationOn = true;
	//	echoLocationCharge -= 1.0f;
	//}
	echoLocationOn = true;
	if (echoLocationCharge == 0.f)
	{
		echoLocationOn = false;
	}

	if (echoLocationCharge >= 100000.f)
	{
		echoPress = true;
	}
}

void AGhostForHireCharacter::EchoLocationEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.10f, FColor::Yellow, TEXT("Echo Location"));
	echoLocationOn = false;
	//if()
}

void AGhostForHireCharacter::EchoLocationRecharge()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.10f, FColor::Yellow, FString::Printf(TEXT("%d"), echoLocationCharge));



	if (echoLocationOn == true && echoPress == true)
	{
		echoLocationCharge -= 1.0f;
		if (echoLocationCharge <= 0.f)
		{
			echoLocationOn = false;
		}
	}
	if (echoLocationOn == false && echoLocationCharge < 100.f)
	{
		echoLocationCharge += 1.0f;
	}
}





