// Fill out your copyright notice in the Description page of Project Settings.


#include "AverageJoeController.h"
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

AAverageJoeController::AAverageJoeController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));

	SetupPerception();

	
}

void AAverageJoeController::AIPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	OnPosses(InPawn);
}

void AAverageJoeController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBehaviorTree();



	
}

void AAverageJoeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Enemy = Cast<AAverageJoeAI>(GetPawn());

	if (Enemy->ScareLevel < 100)
	{
		if (Enemy->CanHear == true)
		{
			HearingReaction();
		}
	}

	
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
	
	if(Enemy->ScareLevel >= 100)
	{
		Enemy->CurrentTask = NULL;
		SpookedVoiceCount += 1;
		

		Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);
		Blackboard->SetValueAsBool(FName(TEXT("is_Light")), false);
		Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);
		Blackboard->SetValueAsBool(FName(TEXT("CanAbility")), false);

		//Set Animations to false
		Enemy->bEctoAnim = false;
		Enemy->bPolterGiestAnim = false;
		Enemy->bHearingAnim = false;
		Enemy->bTouchAnim = false;
		Enemy->bHitPolterGiestAnim = false;

		
	}

	if (SpookedVoiceCount == 1)
	{
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->SpookedAudio, Enemy->GetActorLocation());

	}

	if (Enemy->bScared == true)
	{
		Enemy->bScaredAnim = true;
		RandomScaredAudio();

		ScaredVoiceCount += 1;


		Blackboard->SetValueAsBool(FName(TEXT("CanSeePlayer")), true);
	}

	if (ScaredVoiceCount == 1)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->RandomScareAudio, Enemy->GetActorLocation());

	}
	


	if (Enemy->TaskActors[0] != NULL && Enemy->enteredTrigger == true)
	{
		/*if (Enemy->Quirk1 == "TenseQuirk" || Enemy->Quirk1 == "ParanoidQuirk" || Enemy->Quirk1 == "MaterialisticQuirk" || Enemy->Quirk1 == "MuffledQuirk"
			|| Enemy->Quirk2 == "TenseQuirk" || Enemy->Quirk2 == "ParanoidQuirk" || Enemy->Quirk2 == "MaterialisticQuirk" || Enemy->Quirk2 == "MuffledQuirk")
		{*/
			InvestigateLights();
		/*}*/


	}

	
	TouchAbilityReaction();
	


	if (Enemy->bPoltHit == false)
	{
		/*if (Enemy->ScareLevel < 100)
		{*/
			MoveToEcto();
		/*}*/
	}

	
	if (Enemy->isBlind == false)
	{
		PolterGeistReaction();

			

	}

	if (Player->DayNumber_ref >= 2)
	{
		HitByPoltergeist();
	}


	if (Player->DayNumber_ref <= 1)
	{
		
		KnockReaction();
		
		

	}

	if (Player->DayNumber_ref >= 2)
	{
		QuirkPotency();

	}
	
	if (Player->DayNumber_ref <= 1)
	{
		SetQuirkBool();

	}
}

void AAverageJoeController::StartTask()
{

	if (Enemy->CanTask == true)
	{
		Blackboard->SetValueAsBool(FName(TEXT("CanTask")), true);

		if (Enemy->CurrentTask != nullptr)
		{
			FVector Direction = Enemy->CurrentTask->GetActorLocation() - Enemy->GetActorLocation();

			float TotalDistance = Direction.Size();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Enemy Task Actor Distance: %f"), TotalDistance));


			if (TotalDistance <= 300)
			{

				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Reached Task Location"));

				TaskWaitTime += GetWorld()->DeltaTimeSeconds;

				if (Enemy->taskNumber == 1)
				{
					Enemy->WaitTime = 10;

					//Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), FRotator(0, -90, 0), GetWorld()->DeltaTimeSeconds, 10));

				
				}

				if (Enemy->taskNumber == 2)
				{
					Enemy->WaitTime = 10;

				}

				if (Enemy->taskNumber == 3)
				{
					Enemy->WaitTime = 10;

				}

				if (Enemy->taskNumber == 4)
				{
					Enemy->WaitTime = 10;

				}


				if (TaskWaitTime >= Enemy->WaitTime)
				{
					Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);

					Enemy->StartTaskTime = 0;

					Enemy->taskNumber = Enemy->taskNumber + 1;
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("TaskNumber: %i"), Enemy->taskNumber));

					/*if (Enemy->taskNumber <= Enemy->MaxTaskNumber)
					{
						Enemy->taskNumber++;

					}*/

					Enemy->CanTask = false;


					TaskWaitTime = 0;
				}

			}

			
		}

	}

	
}

void AAverageJoeController::SetupPerception()
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
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAverageJoeController::OnHearing);
		GetPerceptionComponent()->ConfigureSense(*hearing_config);
	}

}

void AAverageJoeController::OnHearing(TArray<AActor*> const& hearing_actors)
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

					if (Player->AICanAudioMove == true)
					{
						if (Enemy->CanHear == true)
						{
							Enemy->SurveyLevel += 1;

							Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), stim.WasSuccessfullySensed());
							Blackboard->SetValueAsVector(FName(TEXT("Location")), stim.StimulusLocation);

							Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);

							Enemy->CanTask = false;
							isScared = true;
							Enemy->isHearing = true;

						}

					

					}
					else
					{
						if (Player->DayNumber_ref <= 1)
						{
							if (Enemy->Quirk1 == "ParanoidQuirk" || Enemy->Quirk1 == "TenseQuirk" || Enemy->Quirk1 == "MuffledQuirk" || Enemy->Quirk1 == "GreasyQuirk"
								|| Enemy->Quirk2 == "ParanoidQuirk" || Enemy->Quirk2 == "TenseQuirk" || Enemy->Quirk2 == "MuffledQuirk" || Enemy->Quirk2 == "GreasyQuirk")
							{
								Enemy->SurveyLevel += 1;

								Enemy->isHearing = true;
								QuirkCount();

							}
							else
							{
								Enemy->SurveyNonEffective = true;
							}


						}


					}
				}

			}

		}


}


//void AAverageJoeController::OnTargetDetected(AActor* Actor, FAIStimulus stimulus)
//{
//	if (AGhostForHireCharacter* character = Cast<AGhostForHireCharacter>(Actor))
//	{
//		if (Enemy->ScareLevel >= 100)
//		{
//			//Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);
//			Blackboard->SetValueAsBool(FName(TEXT("CanSeePlayer")), stimulus.WasSuccessfullySensed());
//
//		}
//	}
//}

void AAverageJoeController::SwitchToWanderingState()
{
	float Direction = FVector::Dist( Player->AudioLocation , Enemy->GetActorLocation());

	//float EnemyDirection = FVector::Distance(Enemy->GetActorLocation(), Player->AudioLocation);

	//float TotalDistance = Direction.Normalize();

	//FVector Direction = Player->AudioLocation - Enemy->GetActorLocation();

	//float TotalDistance = Direction.Length();

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hearing Distance: %f"), Direction));

	

	if (Direction <= 200)
	{

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Reached Audio LOC"));
		Player->AudioLocation = FVector::ZeroVector;

		

		if (Enemy->SurveyLevel >= 3)
		{
			Enemy->LightScareNumber = 0;
			Enemy->HearingScareNumber += 1;
			Enemy->ShiverScareNumber = 0;

		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("HearingScareNumber: %i"), Enemy->HearingScareNumber));

		Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);

		isScared = false;

		Enemy->isHearing = false;


	}
	if (Player->AudioLocation == FVector(0, 0, 0))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Audio LOC is NULL"));

		isPolterGeistTimer = true;
	}
}

void AAverageJoeController::InvestigateLights()
{
	if (Player->AI_InvestigateLight == true && Enemy->enteredTrigger == true)
	{
		//Enemy->CurrentTask = NULL;

		
		if (Enemy->Quirk1 == "TenseQuirk" || Enemy->Quirk1 == "ParanoidQuirk" || Enemy->Quirk1 == "MaterialisticQuirk" || Enemy->Quirk1 == "MuffledQuirk"
			|| Enemy->Quirk2 == "TenseQuirk" || Enemy->Quirk2 == "ParanoidQuirk" || Enemy->Quirk2 == "MaterialisticQuirk" || Enemy->Quirk2 == "MuffledQuirk")
		{
			Enemy->CanTask = false;
			Blackboard->SetValueAsBool(FName(TEXT("is_Light")), true);
			Enemy->CurrentTask = Enemy->TaskActors[0];

			Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);

			LightsVoiceCount += 1;

			if (LightsVoiceCount == 1)
			{
				if (Player->LightsOFF == true)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->LightsVoiceLine, Enemy->GetActorLocation());

				}

				if (Player->FireplaceOFF == true)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->FireplaceVoiceLine, Enemy->GetActorLocation());

				}

			}
		}
		else
		{
			Enemy->SurveyNonEffective = true;
		}

		float Direction = FVector::Dist(Enemy->CurrentTask->GetActorLocation(), Enemy->GetActorLocation());

	


		if (Direction <= 250)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Reached Light Switch LOC"));

			Player->AI_InvestigateLight = false;
			Player->LightState = true;


			if (Enemy->Quirk1 == "TenseQuirk" || Enemy->Quirk1 == "ParanoidQuirk" || Enemy->Quirk1 == "MaterialisticQuirk" || Enemy->Quirk1 == "MuffledQuirk"
				|| Enemy->Quirk2 == "TenseQuirk" || Enemy->Quirk2 == "ParanoidQuirk" || Enemy->Quirk2 == "MaterialisticQuirk" || Enemy->Quirk2 == "MuffledQuirk")
			{
				Enemy->SurveyLevel += 1;

				
				LightsVoiceCount = 0;

				QuirkCount();

			}
			
			Player->LightsOFF = false;
			Player->ToiletOFF = false;
			Player->FireplaceOFF = false;
			Player->GravebellOFF = false;

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Light ON"));

			Blackboard->SetValueAsBool(FName(TEXT("is_Light")), false);
			isScared = false;

			Player->isTampering = false;
			Player->LightsAreOn = true;

		}

		

	}
	else
	{
		Blackboard->SetValueAsBool(FName(TEXT("is_Light")), false);



	}
}



void AAverageJoeController::ShiveringState()
{
	if ((Player->K_1 == 11) || (Player->K_2 == 11) || (Player->K_3 == 11) && Player->IsShiverCooldown == false) // need to go to avgjoeai and add conditions to onoverlapbegin
	{
		//GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Cyan, TEXT("Shivering Bool Works"));
		
			if (Enemy->is_Shivering == true)
			{
				ShiveringTime += GetWorld()->DeltaTimeSeconds;
				if (Enemy->Quirk1 == "ComposedQuirk" || Enemy->Quirk2 == "GreasyQuirk" || Enemy->Quirk2 == "ComposedQuirk" || Enemy->Quirk1 == "GreasyQuirk")
				{
					//BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
					if (Enemy->CanTouch == true)
					{
						bShiverVFX = true;

						Enemy->GetCharacterMovement()->MaxWalkSpeed = 100;


						ShiverSFXCount += 1;


						// added by joe as of 1/25/2024
						Player->IsShiverCooldown = true;
						Player->Shiver_Cooldown = ShiveringTime;
						//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Cyan, TEXT("Shivering"));
						GEngine->AddOnScreenDebugMessage(-1, .01f, FColor::Cyan, TEXT("Shivering Bool Works"));

						Enemy->bTouchAnim = true;

						//UNiagaraFunctionLibrary::SpawnSystemAttached(Enemy->ShiverVFX, Enemy->GetCapsuleComponent(), NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

						//if (bShiverVFX)
						//{
						//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Enemy->ShiverVFX, Enemy->GetActorLocation(), FRotator(0.f), FVector(1.f), true, true);
						//	//bShiverVFX = false;
						//}


						//Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

						if (Enemy->ShiverScareNumber <= 1 && Player->isFlying == false) // added && condition by joe
						{
							//Enemy->ScareLevel += Enemy->spookPotency* Enemy->ShiverScareFactor * GetWorld()->DeltaTimeSeconds;

							//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Scare Level: %f"), Enemy->ScareLevel));

							isScared = true;

						}
						// added by joe 
						else if (Enemy->ShiverScareNumber <= 1 && Player->isFlying == true && spectralChill == false)
						{
							//Enemy->ScareLevel = 25.0f;
							isScared = true;
							spectralChill = true;
						}
						if (ShiverSFXCount == 1)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->ShiverSound, Enemy->GetActorLocation());
						}
					}

				}
				else
				{
					Enemy->SurveyNonEffective = true;
				}
			}
		

	
		if (ShiveringTime >= 5.f)
		{
			Enemy->is_Shivering = false;

			Enemy->LightScareNumber = 0;
			Enemy->HearingScareNumber = 0;
			Enemy->ShiverScareNumber += 1;

			if (Enemy->Quirk1 == "ComposedQuirk" || Enemy->Quirk2 == "GreasyQuirk" || Enemy->Quirk2 == "ComposedQuirk" || Enemy->Quirk1 == "GreasyQuirk")
			{

				Enemy->SurveyLevel += 1;

				if (Enemy->CanTouch == true)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->ShiverVoiceLine, Enemy->GetActorLocation());
				}


				QuirkCount();

			}

			Enemy->GetCharacterMovement()->MaxWalkSpeed = 300;

			Enemy->bTouchAnim = false;

		
			ShiverSFXCount = 0;

			// added by joe as of 1/25/2024
			Player->IsShiverCooldown = false;

			//BehaviorTreeComponent->StartTree(*BehaviorTree);
			Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ShiverScareLevel: %i"), Enemy->ShiverScareNumber));

			bstartShiver = true;

			isScared = false;
			ShiveringTime = 0;
			Player->Shiver_Cooldown = ShiveringTime;
			bShiverVFX = false;
			//Player->ResetKeyBooleans();

		}

	}
	//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Orange, TEXT("Shivering Bool Getting here"));
}

void AAverageJoeController::ColdTouch()
{
	/*if (Enemy->SurveyLevel >= 3)
	{*/
	if (Enemy->ColdHitAI && ((Player->K_1 == 7 && Player->BI) || (Player->K_2 == 7 && Player->BII) || (Player->K_3 == 7 && Player->BIII))) // this kinda works
	{
		ColdTouchTime += GetWorld()->GetDeltaSeconds();
		if (Enemy->CanTouch == true)
		{
			Player->isColdTouchRunning = true;

			if (isSensitive == false)
			{
				BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
				Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			}
			isScared = true;
			Player->IsColdTouchCooldown = true;

			Enemy->bTouchAnim = true;

			//Player->ResetKeyBooleans(); // if i use reset here then it stays frozen forever and does NOT go up

			Enemy->ScareLevel += tenseSpookPotency * Enemy->spookPotency * Enemy->ColdTouchFactor * GetWorld()->DeltaTimeSeconds;

			ColdTouchVoiceCount += 1;
			//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::White, TEXT("GWe getting here"));
		}

		if (ColdTouchVoiceCount == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->ColdVoiceLine, Enemy->GetActorLocation());

			UNiagaraFunctionLibrary::SpawnSystemAttached(Enemy->ColdVFX, Enemy->GetRootComponent(), NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

		}

	}

	if (ColdTouchTime >= 5.f)
	{
		ColdTouchTime = 0;
		isScared = false;
		BehaviorTreeComponent->StartTree(*BehaviorTree);
		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		Enemy->bTouchAnim = false;



		Enemy->ColdHitAI = false;

		Player->HitAI = false;

		coldTouchCount = 0;

		ColdTouchVoiceCount = 0;

		Player->ResetKeyBooleans(); // if i use reset here then it cant be activated a 2nd time, if i comment it out, it will forever go up

		Player->isColdTouchRunning = false;


	}
}

	
	/*}*/


void AAverageJoeController::MoveToEcto()
{
	if (Enemy->AbilityTask != nullptr)
	{
		if (Enemy->DetectedEcto == true)
		{
			if (isGreasy == false)
			{
				Blackboard->SetValueAsBool(FName(TEXT("CanAbility")), true);


				Blackboard->SetValueAsBool(FName(TEXT("CanTask")), false);
			}


			float Direction = FVector::Dist( Enemy->AbilityTask->GetActorLocation() , Enemy->GetActorLocation());

			//float TotalDistance = Direction.Length();
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf( TEXT("Ecto Distance : %f"), Direction));

			if (Direction <= 120)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Entered Ecto splat"));
				if (Enemy->bEctoted == true)
				{
					
					Enemy->bEctoAnim = true;
					
					
					bstartEctoWaitTime = true;

				

				}

			}



		}
	
		
			if (bstartEctoWaitTime == true)
			{

				ectoWaitTime += GetWorld()->DeltaTimeSeconds;

				if (Enemy->GreasyQuirk == false)
				{
					BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
					Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				}


				if (ectoWaitTime >= 2)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Task ended"));

					BehaviorTreeComponent->StartTree(*BehaviorTree);
					Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);




					Blackboard->SetValueAsBool(FName(TEXT("CanAbility")), false);

					Enemy->DetectedEcto = false;

					Enemy->canEctoDetectTrace = true;



				}
			}

			if (ectoWaitTime >= 4.f)
			{
				Enemy->canEctoDetectTrace = false;
					

				Enemy->bEctoted = false;

				Enemy->bEctoAnim = false;

				bstartEctoWaitTime = false;
				ectoWaitTime = 0;
				

			}

			

			

		

	}

}

void AAverageJoeController::PolterGeistReaction()
{
	if (Enemy->DetectedPoltergeist == true)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
		Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bstartPoltergeistWaitTime = true;

		PolterGeistCount++;

		if (Player->DayNumber_ref >= 2)
		{
			Enemy->ScareLevel += Enemy->spookPotency * Enemy->PolterGeistFactor * GetWorld()->DeltaTimeSeconds;

			PolterGiestVoiceCount += 1;

		}

		if (PolterGiestVoiceCount == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->PolterGeistVoiceLine, Enemy->GetActorLocation());

		}

		Enemy->bPolterGiestAnim = true;


		
	}

	if (bstartPoltergeistWaitTime == true)
	{
		if (PolterGeistCount == 1)
		{
			if (Enemy->Quirk1 == "ComposedQuirk" || Enemy->Quirk1 == "TenseQuirk" || Enemy->Quirk1 == "MaterialisticQuirk" || Enemy->Quirk1 == "MuffledQuirk"
				|| Enemy->Quirk2 == "TenseQuirk" || Enemy->Quirk2 == "ComposedQuirk" || Enemy->Quirk2 == "MaterialisticQuirk" || Enemy->Quirk2 == "MuffledQuirk")
			{
				Enemy->SurveyLevel += 1;

				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->RattleVoiceLine, Enemy->GetActorLocation());


				QuirkCount();

			}
			else
			{
				if (Player->DayNumber_ref <= 1)
				{
					Enemy->SurveyNonEffective = true;

				}
			}

		}

		PolterGeistReactionTime += GetWorld()->DeltaTimeSeconds;

		if (PolterGeistReactionTime >= 8.f)
		{
			BehaviorTreeComponent->StartTree(*BehaviorTree);
			Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


			Enemy->bPolterGiestAnim = false;


			Enemy->DetectedPoltergeist = false;

			bstartPoltergeistWaitTime = false;

			PolterGeistReactionTime = 0;

			PolterGeistCount = 0;
		}

	}
}

void AAverageJoeController::InitializeBehaviorTree()
{
	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

	Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Enemy = Cast<AAverageJoeAI>(GetPawn());
}

void AAverageJoeController::OnPosses(APawn* const InPawn)
{
	if (Blackboard != nullptr && BehaviorTree != nullptr)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);


		Enemy = Cast<AAverageJoeAI>(InPawn);



	}
}

void AAverageJoeController::ScareLevel()
{

	if (Enemy->ScareLevel < 100)
	{
		if (Enemy->ScareLevel > 0 && Enemy->ScareLevel < 100)
		{
			scareWaitTime += GetWorld()->DeltaTimeSeconds;
		}

		if (Enemy->ScareLevel <= 0)
		{
			scareWaitTime = 0;
		}

		if (scareWaitTime >= 10)
		{
			if (Enemy->ScareLevel > 0)
			{
				Enemy->bEctoted = false;

				/*Enemy->ScareLevel -= Enemy->ScareDepletionFactor * Enemy->DecayScareFactor * GetWorld()->DeltaTimeSeconds;
				Enemy->ScareLevel -= Enemy->ScareDepletionFactor * Enemy->DecayScareFactor * GetWorld()->DeltaTimeSeconds;
				Enemy->ScareLevel -= Enemy->ScareDepletionFactor * Enemy->DecayScareFactor * GetWorld()->DeltaTimeSeconds;*/

			}

		}

	}


	
}

void AAverageJoeController::HitByPoltergeist()
{
	/*if (Enemy->SurveyLevel >= 3) {*/
		if (Enemy != NULL)
		{
			if (Enemy->bPoltHit)
			{
				PoltHitTimer += GetWorld()->GetDeltaSeconds();
				Enemy->ScareLevel += Enemy->PolterGeistFactor * Enemy->spookPotency * GetWorld()->DeltaTimeSeconds;

				
				PolterGiestHitVoiceCount++;

				Enemy->bHitPolterGiestAnim = true;

				//GEngine->AddOnScreenDebugMessage(-1, 0.10f, FColor::Cyan, TEXT("Spooker"));
			}


		}

		if (PolterGiestHitVoiceCount == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->PolterGeistHitVoiceLine, Enemy->GetActorLocation());
		}

		if (PoltHitTimer > 3.0f)
		{
			Enemy->bPoltHit = false;
			PolterGiestHitVoiceCount = 0;
			Enemy->bHitPolterGiestAnim = false;

			PoltHitTimer = 0;
		}
	/*}*/
}

void AAverageJoeController::KnockReaction()
{
	if (Player->AICanAudioMove == false && Enemy->isHearing == true)
	{
		KnockTimer += GetWorld()->DeltaTimeSeconds;

		if (Enemy->Quirk1 == "ParanoidQuirk" || Enemy->Quirk1 == "TenseQuirk" || Enemy->Quirk1 == "MuffledQuirk" || Enemy->Quirk1 == "GreasyQuirk"
			|| Enemy->Quirk2 == "ParanoidQuirk" || Enemy->Quirk2 == "TenseQuirk" || Enemy->Quirk2 == "MuffledQuirk" || Enemy->Quirk2 == "GreasyQuirk")
		{
			KnockVoiceCount += 1;

			if (KnockVoiceCount == 1)
			{
				if (Enemy->CanHear == true)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->KnockVoiceLine, Enemy->GetActorLocation());

				}

			}
		}

		

	}



	if (KnockTimer >= 3)
	{

		Player->AICanAudioMove = true;

		

		KnockTimer = 0;
		KnockVoiceCount = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Knock Timer : %f"), KnockTimer));
	}

	if (Player->AICanAudioMove == true)
	{
		Enemy->isHearing = false;

		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Hearing set to false"));
	}
}

void AAverageJoeController::QuirkPotency()
{
	
	if (Enemy->TenseQuirk == true)
	{
		Enemy->spookPotency = 1;

		tenseSpookPotency = 1.5;

		materialisticSpookPotency = 1;

	}

	if (Enemy->ComposedQuirk == true)
	{
		Enemy->spookPotency = 0.85;
		tenseSpookPotency = 1;

		materialisticSpookPotency = 1;

	}

	if (Enemy->ParanoidQuirk == true)
	{
		Enemy->spookPotency = 1.5;
		tenseSpookPotency = 1;

		materialisticSpookPotency = 1;

	}

	if (Enemy->MateriallisticQuirk == true)
	{
		Enemy->spookPotency = 1;
		tenseSpookPotency = 1;

		materialisticSpookPotency = 1.5;



	}

	if (Enemy->MuffledQuirk == true)
	{
		hearing_config->HearingRange = 1500;

		GetPerceptionComponent()->ConfigureSense(*hearing_config);

		tenseSpookPotency = 1;

		materialisticSpookPotency = 1;


	}

	if (Enemy->GreasyQuirk == true)
	{
		isGreasy = true;
		tenseSpookPotency = 1;

		materialisticSpookPotency = 1;

	}


		
}

void AAverageJoeController::TouchAbilityReaction()
{
	
	if (Player->DayNumber_ref <= 1)
	{
		//if (ShiverCoolDown >= 25 && bstartShiver == false)
		//{
		//	
		//		ShiveringState();
		//	}

		//}

		//if (bstartShiver == true)
		//{
		//	ShiverCoolDown -= GetWorld()->DeltaTimeSeconds;
		//}

		//if (ShiverCoolDown <= 0)
		//{
		//	bstartShiver = false;
		//	ShiverCoolDown = 25;
		//}

		
		ShiveringState();

		

	}

	if (Enemy->CanTouch == true)
	{
		ColdTouch();
	}

	
}

void AAverageJoeController::HearingReaction()
{
	if (Blackboard->GetValueAsBool(FName(TEXT("is_Hearing"))) == true)
	{

		SwitchToWanderingState();

		if (isPolterGeistTimer == true)
		{
			stopHearing += GetWorld()->DeltaTimeSeconds;

			PolterGiestBreakVoiceCount += 1;
		}
		else
		{
			if (Player->DayNumber_ref >= 2)
			{
				ScreechVoiceCount += 1;

				if (ScreechVoiceCount == 1)
				{
					if (Enemy->CanHear == true)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->ScreechVoiceLine, Enemy->GetActorLocation());
					}

				}

			}
		}

		if (Player->DayNumber_ref <= 1)
		{
			toiletSFXCount += 1;

			if (Player->ToiletOFF == true)
			{
				if (toiletSFXCount == 1)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->ToiletVoiceLine, Enemy->GetActorLocation());

				}

			}

			if (Player->GravebellOFF == true)
			{
				if (toiletSFXCount == 1)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->GraveBellVoiceLine, Enemy->GetActorLocation());

				}
			}

		}

		if (PolterGiestBreakVoiceCount == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Enemy->PolterGeistBreakVoiceLine, Enemy->GetActorLocation());

		}

		if (stopHearing >= 5)
		{
			Enemy->isHearing = false;
			Blackboard->SetValueAsBool(FName(TEXT("is_Hearing")), false);

			isPolterGeistTimer = false;

			ScreechVoiceCount = 0;

			toiletSFXCount = 0;

			PolterGiestBreakVoiceCount = 0;

			stopHearing = 0;
		}

	}

	if (Player->DayNumber_ref >= 2)
	{
		//ScareLevel();

		if (Enemy->isHearing == true && Enemy->HearingScareNumber <= 1)
		{
			Enemy->ScareLevel += Enemy->spookPotency * Enemy->HearingScareFactor * GetWorld()->DeltaTimeSeconds;

		}
	}

	if (Player != NULL)
	{
		if (Player->DayNumber_ref >= 2)
		{
			hearing_config->HearingRange = 3000.0f;

			GetPerceptionComponent()->ConfigureSense(*hearing_config);

		}
	}
}

void AAverageJoeController::QuirkCount()
{
	if (Enemy->Quirk1 == Enemy->AIQuirksName[0] || Enemy->Quirk2 == Enemy->AIQuirksName[0])
	{
		TenseQuirkCount++;
	}

	if (Enemy->Quirk1 == Enemy->AIQuirksName[1] || Enemy->Quirk2 == Enemy->AIQuirksName[1])
	{
		MaterialisticQuirkCount++;
	}

	if (Enemy->Quirk1 == Enemy->AIQuirksName[2] || Enemy->Quirk2 == Enemy->AIQuirksName[2])
	{
		MuffledQuirkCount++;
	}

	if (Enemy->Quirk1 == Enemy->AIQuirksName[3] || Enemy->Quirk2 == Enemy->AIQuirksName[3])
	{
		GreasyQuirkCount++;
	}

	if (Enemy->Quirk1 == Enemy->AIQuirksName[4] || Enemy->Quirk2 == Enemy->AIQuirksName[4])
	{
		ComposedQuirkCount++;
	}

	if (Enemy->Quirk1 == Enemy->AIQuirksName[5] || Enemy->Quirk2 == Enemy->AIQuirksName[5])
	{
		ParanoidQuirkCount++;
	}
}

void AAverageJoeController::SetQuirkBool()
{
	if (TenseQuirkCount >= 3)
	{
		Enemy->TenseQuirk = true;
	}

	if (MaterialisticQuirkCount >= 3)
	{
		Enemy->MateriallisticQuirk = true;

	}

	if (MuffledQuirkCount >= 3)
	{
		Enemy->MuffledQuirk = true;

	}

	if (GreasyQuirkCount >= 3)
	{
		Enemy->GreasyQuirk = true;

	}

	if (ComposedQuirkCount >= 3)
	{
		Enemy->ComposedQuirk = true;

	}

	if (ParanoidQuirkCount >= 3)
	{
		Enemy->ParanoidQuirk = true;

	}

	
}

void AAverageJoeController::RandomScaredAudio()
{

	int RandNum;

	for (int i = 0; i < Enemy->ScaredAudio.Num(); i++)
	{
		RandNum = FMath::RandRange(0, i);
		
	}

	Enemy->RandomScareAudio = Enemy->ScaredAudio[RandNum];
}


