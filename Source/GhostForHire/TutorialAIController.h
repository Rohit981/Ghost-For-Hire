// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AverageJoeAI.h"
#include "GhostForHireCharacter.h"
#include "TutorialAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API ATutorialAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATutorialAIController();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
	UBlackboardComponent* BlackBoardComponent;

	class UAISenseConfig_Hearing* hearing_config;

	void AIPossess(APawn* const InPawn);

	virtual void BeginPlay() override;

	void SetupPerception();

	UFUNCTION()
	void OnHearing(TArray<AActor*> const& hearing_actors);

	virtual void Tick(float DeltaTime) override;

	AAverageJoeAI* Enemy;

	AGhostForHireCharacter* Player;

private:
	void KnockReaction();

	void HitByPoltergeist();

	void HearingReaction();

	float KnockTimer = 0;

	float PoltHitTimer = 0;

	bool isPolterGeistTimer = false;
	float stopHearing = 0;

	
	
};
