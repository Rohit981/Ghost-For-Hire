// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialAIController.h"
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

ATutorialAIController::ATutorialAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));

	SetupPerception();
}

void ATutorialAIController::AIPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard != nullptr && BehaviorTree != nullptr)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);


		Enemy = Cast<AAverageJoeAI>(InPawn);



	}
}

void ATutorialAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Enemy = Cast<AAverageJoeAI>(GetPawn());
}

void ATutorialAIController::SetupPerception()
{
	hearing_config = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	if (hearing_config)
	{

		hearing_config->DetectionByAffiliation.bDetectEnemies =
			hearing_config->DetectionByAffiliation.bDetectFriendlies =
			hearing_config->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		//GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ATutorialAIController::OnHearing);
		GetPerceptionComponent()->ConfigureSense(*hearing_config);
	}
}

void ATutorialAIController::OnHearing(TArray<AActor*> const& hearing_actors)
{
	for (int i = 0; i < hearing_actors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(hearing_actors[i], info);



		for (int j = 0; j < info.LastSensedStimuli.Num(); j++)
		{
			FAIStimulus const stim = info.LastSensedStimuli[j];

			
			if (stim.Tag == FName(TEXT("CanHear")))
			{

				if (Enemy->SurveyLevel < 3)
				{
					Enemy->SurveyLevel += 1;

					Enemy->isHearing = true;

				}
				else
				{
					Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), stim.WasSuccessfullySensed());
					Blackboard->SetValueAsVector(FName(TEXT("Location")), stim.StimulusLocation);

					Enemy->isHearing = true;
				}


			}

			

		}


	}
}

void ATutorialAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Enemy->SurveyLevel < 3)
	{
		KnockReaction();

	}

	HitByPoltergeist();

	HearingReaction();

	if (Enemy->bScared == true)
	{
		Blackboard->SetValueAsBool(FName(TEXT("CanSeePlayer")), true);

		Enemy->bScaredAnim = true;
	}

	
	
}

void ATutorialAIController::KnockReaction()
{
	if ( Enemy->isHearing == true)
	{
		KnockTimer += GetWorld()->DeltaTimeSeconds;

	}

	if (KnockTimer >= 2)
	{

		Player->AICanAudioMove = true;

		KnockTimer = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Knock Timer : %f"), KnockTimer));
	}

	if (Player->AICanAudioMove == true)
	{
		Enemy->isHearing = false;

		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Hearing set to false"));
	}
}

void ATutorialAIController::HitByPoltergeist()
{
	if (Enemy != NULL)
	{
		if (Enemy->bPoltHit)
		{
			PoltHitTimer += GetWorld()->GetDeltaSeconds();
			//Enemy->ScareLevel =  33.3f;

			Enemy->bHitPolterGiestAnim = true;

			//GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Cyan, TEXT("Spooker"));
		}
	}
	if (PoltHitTimer > 3.0f)
	{
		Enemy->bPoltHit = false;

		//Enemy->ScareLevel += 34.f;


		PoltHitTimer = 0;

		Enemy->bHitPolterGiestAnim = false;
	}
}

void ATutorialAIController::HearingReaction()
{
	if (Blackboard->GetValueAsBool(FName(TEXT("is_Hearing"))) == true)
	{
		isPolterGeistTimer = true;

		if (isPolterGeistTimer == true)
		{
			stopHearing += GetWorld()->DeltaTimeSeconds;
		}

		if (stopHearing >= 3)
		{
			Enemy->isHearing = false;
			Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);

			Enemy->ScareLevel += 34.f;

			isPolterGeistTimer = false;

			stopHearing = 0;
		}

	}

	

}
