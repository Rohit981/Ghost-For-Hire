// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InteractableObject.h"
#include "WaveMesh.h"
#include "AI_Interactable.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "EctoSplatObject.h"
//#include "CustomSpringArmComponent.h"
#include "GhostForHireCharacter.generated.h"

class UNiagaraSystem;

UCLASS(config=Game)
class AGhostForHireCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	//class UCustomSpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Flying Inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleFlyAndWalk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DescendAction;

	/** Audio Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PlayAudioInputAction;

	/** Light Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PlayLightInputAction;

	/** Scare Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ScareInputAction ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AbilityI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AbilityII;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AbilityIII;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AbilityIV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AbilityEchoLocation;




public:
	AGhostForHireCharacter();
	
	// TimelineProgress declaration
	UFUNCTION()
	void TimelineProgress(float Value);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	//Audio
	void PlayAudio();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ScreechSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* KnockSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ToiletSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* GraveBellSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* LightSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* FireplaceSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* OrganSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* PoltergeistThrowSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ColdTouchSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* ScreechVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* FireplaceVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* ToiletVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* OrganVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* RattleVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
		UNiagaraSystem* CandleVFX;

	UPROPERTY(EditAnywhere)
		FVector vfxLocation;

	//AI
	class UAIPerceptionStimuliSourceComponent* stimulSource;

	void SetupStimuls();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AWaveMesh> WaveMeshClass;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	float PolterGeistVRadius;

	// Timeline Protected Variables
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	FTimerHandle MyTimerHandle;

	AAI_Interactable* interactableObject;

	


	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

private:
	bool canColdTouch = false;
			

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FORCEINLINE class UCustomSpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Skeletal Mesh Component -------------------------------------------------------------------------|

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* CharacterMesh;

	// Timeline Variables Public -----------------------------------------------------------------------|

	// Declare the timeline variable we are going to use
	FTimeline CurveTimeline;

	// Timeline event that is fired once the timeline ends
	FOnTimelineEvent endTimeline;

	// This will reset the changes done in the Timeline
	UFUNCTION()
	void ResetTimeline();

	// Abilities UProperties and UFunctions ------------------------------------------------------------|

	UFUNCTION(BlueprintCallable, Category = "Character")
	void ToggleFlyingMode();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void KnockOrScratch();

	// Abilities normal variables ----------------------------------------------------------------------|

	// boolean values for abilities 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool isFlying = false;
	bool isDraining = false;
	bool isTouchingDoorOrWindow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isTampering;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LightsAreOn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString hitActorString;

	// Flying functions --------------------------------------------------------------------------------|

	// Function to set the movement of the flying
	void FlyMovement();
	void DescendMovement();
	void StopFlying();
	void StartFlying();

	//Scare interaction
	void ScareInteraction();
	

	float scareAnimTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isScared = false;

	class AAverageJoeAI* AvgJoe;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
		bool scaredState = false;

	//AI Light and Audio Variables
	FVector AudioLocation;

	//Light Bool
	bool is_Light = false;

	bool LightsOFF = false;
	bool FireplaceOFF = false;
	bool GravebellOFF = false;
	bool ToiletOFF = false;


	bool AI_InvestigateLight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LightState = true;

	//Interactable Objects Function and Variables ------------------------------------------------------|
	void SphereTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Hit;

	UPROPERTY(EditAnywhere, Category = "Sphere Trace Radius")
	float TraceRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyScareCount = 0;

	AInteractableObject* IntObjRef;

	//Boolean for Audio
	bool isPlayingAudio = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool AICanAudioMove = true;

	// Ability booleans --------------------------------------------------------------------------------|
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_Fly;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_PoltergeistI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_PoltergeistII;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_PoltergeistIII;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_PoltergeistIV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_PoltergeistV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_EctoHurl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_EctoSplat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_ColdTouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_Screech;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_Rattle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool AB_Tamper;

	// Choose Poltergeist ------------------------------------------------------------------------------|
	UFUNCTION(BlueprintCallable)
	void DoPoltergeistAbility();

	void PoltergeistIII();

	void PoltergeistIV();

	void PoltergeistV();

	void DestroyPoltergeistObject();

	// Check if Key is Pressed -------------------------------------------------------------------------|
	void KeyPressed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool pressedQ;

	APlayerController* PlayerControllerRef;

	// InteractableObject References -------------------------------------------------------------------|
	AInteractableObject* IntObjRef2;

	// Tick Time Count ---------------------------------------------------------------------------------|
	float TimeCount;

	float RattleCount;

	bool InteractActive = false;

	bool PoltergeistActive = false;

	// Average Joe AI ----------------------------------------------------------------------------------|
	bool HitAI = false;

	AActor* LightSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
		AActor* LightSwitchTestActor;

	// EctoHurl and Splat ------------------------------------------------------------------------------|
	void EctoHurl();

	void EctoSplat();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Physics, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEctoSplatObject> EctoSplatClass;


	// New Interactions added by Rohit -----------------------------------------------------------------|
	void LightSwitchInteraction();

	bool CanInteract = false;

	bool isInteracting = false;

	bool AILantern = false;

	// Player Currencies and Tracking Numbers ----------------------------------------------------------|
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency")
	int GPM_Funds = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency")
	float Reputation_XP = 75.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency")
	int Spirit_Coin;

	// Ability Loadout Functions -----------------------------------------------------------------------|
	void Ability_I();

	bool BI;

	void Ability_II();

	bool BII;

	void Ability_III();

	bool BIII;

	void Ability_IV();

	bool BIV;

	//void CheckKeyBindingsAbilities(int key);

	UFUNCTION(BlueprintCallable)
	void ResetKeyBooleans();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_Rattle = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_PoltergeistIII = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_Screech = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_PoltergeistIV = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_EctoHurl = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_EctoSplat = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	//int K_PoltergeistV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	int K_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	int K_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	int K_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding Abilities")
	int K_4;

	void CheckKeyAbilities();

	// Ability Cooldown Variables --------------------------------------------------------------------|

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float P3_Cooldown = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float P4_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float Tornado_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float EctoHurl_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float EctoSplat_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float Screech_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float Tamper_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float Rattle_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float ColdTouch_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float Shiver_Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	float Knock_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsP3Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsP4Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsTornadoCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsEctoHurlCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsEctoSplatCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsScreechCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsTamperCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsRattleCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsColdTouchCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsKnockCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown")
	bool IsShiverCooldown;

	void StartAbilityCooldown(float time);

	void CheckAbilityCooldown();

	UFUNCTION(BlueprintCallable)
	void ResetAbilityCooldowns();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_P3 = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_P4 = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_Tornado = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_EctoHurl = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_EctoSplat = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_Screech = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_Tamper = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_Rattle = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_ColdTouch = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_Shiver = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Cooldown Duration")
	float cd_Knock = 5.0f;


	bool isColdTouchRunning;

	// Camera Function ----------------------------------------------------------------------------|

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool collidingWithWall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector leavingWallPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector wallDistance;

	void checkWallDistance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool cameraReady;

	void lerpCamera(float DeltaTime);

	bool canCameraTurn;

	bool resetArmLength;

	// Echo Location Ability ----------------------------------------------------------------------------|

	void EchoLocation();

	void EchoLocationEnd();

	void EchoLocationRecharge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool echoLocationOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float echoLocationCharge = 100.0f;

	bool echoPress;

	// Day Night Cycle Ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int DayNumber_ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float timer;

	// Mouse Sensitivity
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mouseSense = 1.00f;

	// LampPost
	UPROPERTY(BlueprintReadWrite)
	bool LP1;

	UPROPERTY(BlueprintReadWrite)
	bool LP2;

	UPROPERTY(BlueprintReadWrite)
	bool LP3;

	UPROPERTY(BlueprintReadWrite)
	bool LP4;

	UPROPERTY(BlueprintReadWrite)
	bool LP5;

	UPROPERTY(BlueprintReadWrite)
	bool LP6;
};

