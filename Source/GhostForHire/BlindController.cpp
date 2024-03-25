// Fill out your copyright notice in the Description page of Project Settings.


#include "BlindController.h"
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
#include "AverageJoeAI.h"

ABlindController::ABlindController()
{
	
}

void ABlindController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBehaviorTree();

	
}

void ABlindController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HearingReaction();

	if (Enemy != nullptr)
	{

		if (Enemy->ScareLevel < 100)
		{
			for (int i = 0; i < Enemy->TaskActors.Num(); i++)
			{
				if (Enemy->TaskActors[i] != nullptr)
				{
					StartTask();

				}

			}

		}
	}

	if (Enemy->ScareLevel >= 100)
	{
		Enemy->CurrentTask = NULL;

		Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);
		Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);

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

	//TouchAbilityReaction();

	

	if (Enemy->SurveyLevel <= 3)
	{
		Enemy->canTrace = true;

		
		BlindPolterGiest();

	}
	else
	{
		Enemy->canTrace = false;
	}

	/*if (Player->DayNumber_ref <= 1)
	{
		KnockReaction();
	}*/

	//HitByPoltergeist();

	if (Player->DayNumber_ref >= 2)
	{
		QuirkPotency();

	}

	if (Player->DayNumber_ref <= 1)
	{
		SetQuirkBool();
	}
	
}

void ABlindController::AIPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	OnPosses(InPawn);
}

void ABlindController::BlindPolterGiest()
{
	if (Enemy->DetectedPoltergeist == true)
	{
		
		bstartPoltergeistWaitTime = true;

		PolterGeistCount++;

	}

	if (bstartPoltergeistWaitTime == true)
	{
		if (PolterGeistCount == 1)
		{
			Enemy->SurveyLevel += 1;

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->RattleVoiceLine, Enemy->GetActorLocation());


			QuirkCount();

		}
		
		PolterGeistReactionTime += GetWorld()->DeltaTimeSeconds;

		if (PolterGeistReactionTime >= 5.f)
		{
			
			Enemy->DetectedPoltergeist = false;

			bstartPoltergeistWaitTime = false;

			PolterGeistReactionTime = 0;

			PolterGeistCount = 0;

		}
	}
}
