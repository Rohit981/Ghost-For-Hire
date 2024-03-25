// Fill out your copyright notice in the Description page of Project Settings.


#include "AverageJoeAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InteractableObject.h"
#include "EctoSplatObject.h"



// Sets default values
AAverageJoeAI::AAverageJoeAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScareMeter_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	ScareMeter_UI->SetupAttachment(RootComponent);

	SurveyEmote_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("SurveyEmote"));
	SurveyEmote_UI->SetupAttachment(RootComponent);

	
	
}

// Called when the game starts or when spawned
void AAverageJoeAI::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAverageJoeAI::OnOverlapBegin);
	

	Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	taskNumber = FMath::RandRange(1,3);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("TaskNumber: %i"), taskNumber));

	PopulateQuirkList();
	RandomizedQuirk();
	

}





// Called every frame
void AAverageJoeAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInstance = GetMesh()->GetAnimInstance();

	if (CanTask == false )
	{
		StartTaskTime += DeltaTime;

	}

	if (ScareLevel < 100)
	{
		if (StartTaskTime >= 0.5f)
		{
		
			CanTask = true;

		}

	}

	if (taskNumber > MaxTaskNumber)
	{
		taskNumber = 1;

	}


	if (CanTask == true && Player->AI_InvestigateLight == false)
	{
		for (int i = 0; i < TaskActors.Num(); i++)
		{
			if (TaskActors[i] != nullptr)
			{

				CurrentTask = TaskActors[taskNumber];

			}
		}

	}

	/*if (Player->isPlayingAudio == false)
	{
		CurrentTask = TaskActors[taskNumber];

	}*/

	if (bEctoted == true)
	{
		
		EctoSplatScareLevel();

		ectoTimer += DeltaTime;

	}
	
	if (enteredTrigger == true)
	{
		if (Player->LightSwitch != NULL)
		{
			TaskActors[0] = Player->LightSwitch;

		}
	}
	else
	{
		for (int i = 0; i < TaskActors.Num(); i++)
		{
			if (TaskActors[i] != nullptr)
			{
				TaskActors[0] = 0;

			}
		}
	}


	
	if (canTrace == true)
	{
		AbilityDetection();

	}
	
	if (Player->isScared == false)
	{
		bScaredState = false;
	}

	//if (bScared == true)
	//{
	//	//AnimInstance->Montage_Play(ScaredPose, 1.f, EMontagePlayReturnType::Duration, 0.f);
	//	GetMesh()->PlayAnimation(ScaredAnim, true);

	//}

	if (isHearing == true)
	{
		//AnimInstance->Montage_Play(AudioSpookPose, 1.f, EMontagePlayReturnType::Duration, 0.f);

		if (CanHear == true)
		{
			bHearingAnim = true;

		}

	}

	if (isHearing == false)
	{
		bHearingAnim = false;

		//GetMesh()->PlayAnimation(NeutralAnim, isHearing);

	}

	if (SurveyNonEffective == true)
	{
		NonEffectiveUITimer += DeltaTime;
	}

	if (NonEffectiveUITimer >= 5)
	{
		SurveyNonEffective = false;

		NonEffectiveUITimer = 0;
	}

	//Maybe be Usefull Later
	/*bool IsHit;

	if (canTrace == true)
	{
		FVector Start = GetActorLocation();

		FVector End = (GetActorForwardVector() * 50) + Start;


		FHitResult OutHit;

		TArray<AActor*> ActorsToIgnore;

		ActorsToIgnore.Add(this);

		IsHit = false;

		IsHit = UKismetSystemLibrary::SphereTraceSingle(this->GetWorld(), Start, End, 50, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.f);

		if (IsHit && canTrace)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Doing Task"));

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


		}

		doingTaskTime += DeltaTime;
	}


	if (doingTaskTime >= 10)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Task Done"));


		canTrace = false;
		IsHit = false;

		taskTime = 0;
		doingTaskTime = 0;


	}*/

}



void AAverageJoeAI::EctoSplatScareLevel()
{
	if (Player->DayNumber_ref >= 2)
	{
		if (ectoTimer <= 2)
		{
			EctoVoiceCount += 1;


			if (EctoVoiceCount == 1)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EctoVoiceLine, GetActorLocation());
			}
			ScareLevel += spookPotency* EctoPlasmFactor * GetWorld()->DeltaTimeSeconds;

		}

		else
		{
			ectoTimer = 0;

			EctoVoiceCount = 0;

			bEctoAnim = false;
			bEctoted = false;

		}

	}
}

void AAverageJoeAI::AbilityDetection()
{
	if (canEctoDetectTrace == false)
	{
		// Gets the location of the player
		FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();

		// Sets the location of how far the sphere trace will be
		FVector EndLocation = StartLocation + (GetActorForwardVector() * 700.0f);

		// Create an array to ignore actors, really for just the player
		TArray<AActor*> ActorsToIgnore;

		// This ignores the player character touching the player
		ActorsToIgnore.Add(this);

		// Here is where we will get a reference to our hit actor
		FHitResult OutHit;

		// Magic happens here, it will get a SINGLE SPHERE TRACE and convert that information into a boolean
		bool Hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, TraceRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
			true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true, FLinearColor::Red, FLinearColor::Blue, 60.0f);

		AInteractableObject* InteractableOBJ = Cast<AInteractableObject>(OutHit.GetActor());

		AEctoSplatObject* EctoSplatOBJ = Cast<AEctoSplatObject>(OutHit.GetActor());


		if (Hit == true)
		{
			if (InteractableOBJ != NULL)
			{
				if (InteractableOBJ->isPoltergeistActive == true)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Is PolterGeist Active"));

					DetectedPoltergeist = true;

				}
				else
				{
					DetectedPoltergeist = false;

				}

			}

			/*if (isBlind == false)
			{*/
				if (EctoSplatOBJ != NULL)
				{
					if (EctoSplatOBJ->EctoSplatActive == true)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Detected Ecto Splat"));
						DetectedEcto = true;
						AbilityTask = OutHit.GetActor();

					}

				}

			/*}*/

		}
		else
		{
			DetectedPoltergeist = false;
			DetectedEcto = false;
			AbilityTask = nullptr;
		}

		

	}
}

void AAverageJoeAI::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Player")) && Player->DayNumber_ref <= 1 && Player->IsShiverCooldown == false)
		{

			is_Shivering = true;
			//if(Player)
			if ((Player->K_1 == 11) || (Player->K_2 == 11) || (Player->K_3 == 11) && Player->IsShiverCooldown == false)
				UNiagaraFunctionLibrary::SpawnSystemAttached(ShiverVFX, RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d"), is_Shivering));

		}

		if (OtherComp->ComponentHasTag(FName("Interactable")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enemy Hit"));
			bPoltHit = true;
		}

		
	}
}



void AAverageJoeAI::PopulateQuirkList()
{
	AIQuirksName.Add("TenseQuirk");
	AIQuirksName.Add("MaterialisticQuirk");
	AIQuirksName.Add("MuffledQuirk");
	AIQuirksName.Add("GreasyQuirk");
	AIQuirksName.Add("ComposedQuirk");
	AIQuirksName.Add("ParanoidQuirk");
	
	
}


void AAverageJoeAI::RandomizedQuirk()
{
	int RanNum1;
	int RanNum2;

	if (CanHave2Quirk == true)
	{
		for (int i = 0; i < AIQuirksName.Num(); i++)
		{
			RanNum1 = FMath::RandRange(0, i);
			RanNum2 = FMath::RandRange(0, i);
		}

		if (RanNum1 != RanNum2)
		{
			Quirk1 = AIQuirksName[RanNum1];
			Quirk2 = AIQuirksName[RanNum2];

		}
		else
		{
			for (int i = 0; i < AIQuirksName.Num(); i++)
			{
				RanNum1 = FMath::RandRange(0, i);
				RanNum2 = FMath::RandRange(0, i);
			}

			Quirk1 = AIQuirksName[RanNum1];
			Quirk2 = AIQuirksName[RanNum2];
		}

	}
	else
	{
		for (int i = 0; i < AIQuirksName.Num(); i++)
		{
			RanNum1 = FMath::RandRange(0, i);
			
		}

		Quirk1 = AIQuirksName[RanNum1];
			

		
	}

	if ((Quirk1 == "ParanoidQuirk" || Quirk2 == "ParanoidQuirk") && (Quirk1 == "ComposedQuirk" || Quirk2 == "ComposedQuirk"))
	{
		for (int i = 0; i < AIQuirksName.Num(); i++)
		{
			RanNum1 = FMath::RandRange(0, i);
			RanNum2 = FMath::RandRange(0, i);
		}

		Quirk1 = AIQuirksName[RanNum1];
		Quirk2 = AIQuirksName[RanNum2];
	}

}

