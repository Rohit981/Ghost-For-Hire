// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GhostForHireCharacter.h"
#include "Animation/AnimMontage.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "AverageJoeAI.generated.h"


class UAnimMontage;
UCLASS()
class GHOSTFORHIRE_API AAverageJoeAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAverageJoeAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* ScareMeter_UI;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* SurveyEmote_UI;

	AGhostForHireCharacter* Player;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Search")
	float TraceRadius;

	void RandomizedQuirk();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", Meta = (MakeEditWidget = true))
	TArray<AActor*> TaskActors;

	AActor* CurrentTask;
	AActor* AbilityTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (AllowPrivateAccess = true))
	AActor* RunLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool is_Shivering = false;

	float StartTaskTime = 0;


	bool CanTask = false;

	bool enteredTrigger = false;

	bool DetectedEcto = false;

	bool DetectedPoltergeist = false;

	bool canEctoDetectTrace = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool isAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability")
	bool SurveyNonEffective = false;

	int taskNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (AllowPrivateAccess = true))
	int MaxTaskNumber;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		bool canTrace = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		bool isBlind = false;

	//Scare Level
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
		float ScareLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		float spookPotency = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float ColdTouchFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float PolterGeistFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float EctoPlasmFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float HearingScareFactor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		bool CanTouch = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		bool CanHear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	int SurveyLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bScaredState = false;

	//Ability Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ShiverSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* EctoPlasmSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ColdTouchSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* ScaredAnim;

	UAnimInstance* AnimInstance;

	//Quirks Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool TenseQuirk = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool MateriallisticQuirk = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool MuffledQuirk = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool GreasyQuirk = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool ComposedQuirk = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool ParanoidQuirk = false;

	UPROPERTY(VisibleAnywhere, Category = "Quirk")
	TArray<FString> AIQuirksName;

	//Randomized Quirk
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quirk")
	FString Quirk1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quirk")
	FString Quirk2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quirk")
	bool CanHave2Quirk;


	float WaitTime = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	bool isHearing = false;
	
	bool bEctoted = false;

	bool bScared = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool ColdHitAI = false;


	int LightScareNumber = 0;
	int ShiverScareNumber = 0;
	int HearingScareNumber = 0;

	void EctoSplatScareLevel();
	void AbilityDetection();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bPoltHit = false;

	//Animation Bool
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bEctoAnim = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bHearingAnim = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bPolterGiestAnim = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bTouchAnim = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bScaredAnim = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim")
	bool bHitPolterGiestAnim = false;

	



	//AI voice lines
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* SpookedAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	TArray<USoundBase*> ScaredAudio;

	USoundBase* RandomScareAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* RattleVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* KnockVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* ShiverVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* LightsVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* ToiletVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* FireplaceVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* GraveBellVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* OrganVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* EctoVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* PolterGeistVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* PolterGeistHitVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* PolterGeistBreakVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* ColdVoiceLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice", meta = (ALlowPrivateAccess = true))
	USoundBase* ScreechVoiceLine;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ShiverVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ColdVFX;




private:
	void PopulateQuirkList();

	float ectoTimer = 0;

	int EctoVoiceCount = 0;

	float NonEffectiveUITimer = 0;

};
