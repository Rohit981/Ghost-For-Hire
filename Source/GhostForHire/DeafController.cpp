// Fill out your copyright notice in the Description page of Project Settings.


#include "DeafController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Math/UnrealMathUtility.h"


ADeafController::ADeafController()
{
	
}

void ADeafController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBehaviorTree();

	
}

void ADeafController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Enemy != nullptr)
	{
		for (int i = 0; i < Enemy->TaskActors.Num(); i++)
		{
			if (Enemy->TaskActors[i] != nullptr)
			{
				StartTask();

			}

		}
	}

	if (Enemy->TaskActors[0] != NULL)
	{
		InvestigateLights();

	}

	if (Enemy->ScareLevel >= 100)
	{
		Enemy->CurrentTask = NULL;

		Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);
		Blackboard->SetValueAsBool(FName(TEXT("is_Light")), false);
		Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);
		Blackboard->SetValueAsBool(FName(TEXT("CanAbility")), false);

		Enemy->bEctoAnim = false;
		Enemy->bPolterGiestAnim = false;
		Enemy->bHearingAnim = false;
		Enemy->bTouchAnim = false;
		Enemy->bHitPolterGiestAnim = false;


	}

	if (Enemy->bScared == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Can See Player!"));

		RandomScaredAudio();

		Blackboard->SetValueAsBool(FName(TEXT("CanSeePlayer")), true);
	}

	TouchAbilityReaction();

	if (Enemy->bPoltHit == false)
	{
		if (Enemy->ScareLevel < 100)
		{
			MoveToEcto();
		}
	}

	

	if (Enemy->ScareLevel < 100)
	{
		if (Enemy->isBlind == false)
		{
			PolterGeistReaction();

		}

	}

	//HitByPoltergeist();

	if (Player->DayNumber_ref >= 2)
	{
		QuirkPotency();

	}

	SetQuirkBool();
}

void ADeafController::AIPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	OnPosses(InPawn);
}


