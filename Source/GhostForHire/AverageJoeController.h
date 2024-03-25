// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AverageJoeAI.h"
#include "GhostForHireCharacter.h"
#include "AverageJoeController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API AAverageJoeController : public AAIController
{
	GENERATED_BODY()

public:
	AAverageJoeController();

	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBlackboardComponent* BlackBoardComponent;

	class UAISenseConfig_Hearing* hearing_config;

	class UAISenseConfig_Sight* sight_config;


	FTimerHandle tHandleTask;

	AAverageJoeAI* Enemy;


	/*UFUNCTION()
	void Update(TArray<AActor*> const& updateActors);*/


	void AIPossess(APawn* const InPawn);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void StartTask();

	void SetupPerception();

	UFUNCTION()
	void OnHearing(TArray<AActor*> const& hearing_actors);

	//UFUNCTION()
	//void OnTargetDetected(AActor* Actor, FAIStimulus stimulus);

	void SwitchToWanderingState();

	void InvestigateLights();

	void ShiveringState();

	void ColdTouch();

	void MoveToEcto();

	void PolterGeistReaction();

	void InitializeBehaviorTree();

	void OnPosses(APawn* const InPawn);

	void ScareLevel();

	void HitByPoltergeist();

	void KnockReaction();

	void QuirkPotency();

	void TouchAbilityReaction();

	void HearingReaction();

	void QuirkCount();

	void SetQuirkBool();

	void RandomScaredAudio();

	AGhostForHireCharacter* Player;

	float CurrentDistance;

	float ShiveringTime = 0;

	float TaskWaitTime;

	bool isScared = false;

	float scareWaitTime = 0;

	bool spectralChill = false;

	float ColdTouchTime = 0.f;

	float ectoWaitTime = 0;

	bool bstartEctoWaitTime = false;

	bool bstartPoltergeistWaitTime = false;

	float PolterGeistReactionTime;

	bool isPolterGeistTimer = false;
	float stopHearing = 0;

	float PoltHitTimer = 0.f;

	int coldTouchCount = 0;

	int PolterGeistCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShiverVFX;



private:
	float ShiverCoolDown = 25;
	bool bstartShiver = false;

	float KnockTimer = 0;

	bool isGreasy = false;

	float tenseSpookPotency = 1;

	float materialisticSpookPotency = 1;

	bool isSensitive = false;

	//Quirk Counts
	int TenseQuirkCount = 0;
	int ComposedQuirkCount = 0;
	int ParanoidQuirkCount = 0;
	int MaterialisticQuirkCount = 0;
	int MuffledQuirkCount = 0;
	int GreasyQuirkCount = 0;

	int SpookedVoiceCount = 0;
	int ScaredVoiceCount = 0;
	int KnockVoiceCount = 0;
	
	int PolterGiestVoiceCount = 0;
	int PolterGiestHitVoiceCount = 0;
	int PolterGiestBreakVoiceCount = 0;
	int ColdTouchVoiceCount = 0;
	int ScreechVoiceCount = 0;
	int LightsVoiceCount = 0;
	int ShiverSFXCount = 0;
	int toiletSFXCount = 0;




	
};
