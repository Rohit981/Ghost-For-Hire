// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostForHire_AICharacter.h"
#include "Enemy.h"
#include "EnemyController.generated.h"


class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_AI_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBlackboardComponent* BlackBoardComponent;

	class UAISenseConfig_Hearing* hearing_config;

	AEnemy* Enemy;

	FTimerHandle tHandleTask;

	/*UFUNCTION()
	void Update(TArray<AActor*> const& updateActors);*/


	void AIPossess(APawn* const InPawn);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void StartTask();

	void SetupPerception();

	UFUNCTION()
	void OnHearing(TArray<AActor*> const& hearing_actors);

	void SwitchToWanderingState();

	void InvestigateLights();

	void ShiveringState();

	AGhostForHire_AICharacter* Player;

	float CurrentDistance;

	float ShiveringTime = 0;

	bool isScared = false;

	bool isHearing = false;

	
	
};
