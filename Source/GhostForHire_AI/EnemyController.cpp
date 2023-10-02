// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"



AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));

	SetupPerception();
}

void AEnemyController::AIPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard != nullptr && BehaviorTree != nullptr)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);


		Enemy = Cast<AEnemy>(InPawn);



	}
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	Player = Cast<AGhostForHire_AICharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Enemy = Cast<AEnemy>(GetPawn());
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (Blackboard->GetValueAsBool(FName(TEXT("is_Hearing"))) == true)
	{
		//CurrentDistance = Enemy->GetActorForwardVector().Size();

		SwitchToWanderingState();
		

	}

	
	InvestigateLights();

	ShiveringState();
	
	if (isScared == false)
	{
		if (Enemy->ScareLevel > 0)
		{
			Enemy->ScareLevel -= Enemy->LightScareFactor * GetWorld()->DeltaTimeSeconds;
			Enemy->ScareLevel -= Enemy->ShiverScareFactor * GetWorld()->DeltaTimeSeconds;
			Enemy->ScareLevel -= Enemy->HearingScareFactor * GetWorld()->DeltaTimeSeconds;
		}

	}

	if (isHearing == true && Enemy->HearingScareNumber <=1)
	{
		Enemy->ScareLevel += Enemy->HearingScareFactor * GetWorld()->DeltaTimeSeconds;

	}

	//StartTask();
}

void AEnemyController::StartTask()
{
	

	if (Enemy->canTrace == false)
	{
		Enemy->taskTime += GetWorld()->DeltaTimeSeconds;


		if (Enemy->taskTime >= 15)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Moving to Task"));
			Blackboard->SetValueAsBool(FName(TEXT("CanTask")), true);

			Enemy->canTrace = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Wandering"));

			Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);
		}

	}
}

void AEnemyController::SetupPerception()
{
	hearing_config = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	if (hearing_config)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		hearing_config->HearingRange = 3000.0f;
		
		hearing_config->DetectionByAffiliation.bDetectEnemies =
			hearing_config->DetectionByAffiliation.bDetectFriendlies =
			hearing_config->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		//GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::OnHearing);
		GetPerceptionComponent()->ConfigureSense(*hearing_config);
	}
}

void AEnemyController::OnHearing(TArray<AActor*> const& hearing_actors)
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
				Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), stim.WasSuccessfullySensed());
				Blackboard->SetValueAsVector(FName(TEXT("Location")), stim.StimulusLocation);

				isScared = true;
				isHearing = true;

				
			}
		}
	}
}

void AEnemyController::SwitchToWanderingState()
{

	FVector Direction = Player->AudioLocation - Enemy->GetActorLocation();

	float TotalDistance = Direction.Size();

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Total Distance: %f"), TotalDistance));

	if (TotalDistance <= 69)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Reached Audio LOC"));

		Enemy->LightScareNumber = 0;
		Enemy->HearingScareNumber += 1;
		Enemy->ShiverScareNumber = 0;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("HearingScareNumber: %i"), Enemy->HearingScareNumber));

		Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);

		isScared = false;

		isHearing = false;


	}
}

void AEnemyController::InvestigateLights()
{
	if (Player->AI_InvestigateLight == true)
	{
		Blackboard->SetValueAsBool(FName(TEXT("is_Light")), true);

		FVector Direction = Enemy->TaskActor->GetActorLocation() - Enemy->GetActorLocation();

		float TotalDistance = Direction.Size();

		if (Enemy->LightScareNumber <= 1)
		{
			Enemy->ScareLevel += Enemy->LightScareFactor * GetWorld()->DeltaTimeSeconds;

			isScared = true;
		}



		if (TotalDistance <= 190)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Reached Light Switch LOC"));

			Player->AI_InvestigateLight = false;
			Player->LightState = true;

			Enemy->LightScareNumber += 1;
			Enemy->HearingScareNumber = 0;
			Enemy->ShiverScareNumber = 0;
			

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Light ON"));

			Blackboard->SetValueAsBool(FName(TEXT("is_Light")), false);
			isScared = false;
			
		}

	}
	else
	{
		Blackboard->SetValueAsBool(FName(TEXT("is_Light")), false);

		

	}
}

void AEnemyController::ShiveringState()
{
	if (Enemy->is_Shivering == true)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);

		ShiveringTime += GetWorld()->DeltaTimeSeconds;

		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (Enemy->ShiverScareNumber <= 1)
		{
			Enemy->ScareLevel += Enemy->ShiverScareFactor * GetWorld()->DeltaTimeSeconds;
			isScared = true;

		}

	}
	else
	{
		BehaviorTreeComponent->StartTree(*BehaviorTree);
		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		
	}

	if (ShiveringTime >= 5.f)
	{
		Enemy->is_Shivering = false;

		Enemy->LightScareNumber = 0;
		Enemy->HearingScareNumber = 0;
		Enemy->ShiverScareNumber += 1;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ShiverScareLevel: %i"), Enemy->ShiverScareNumber));

		isScared = false;
		ShiveringTime = 0;

	}
}

