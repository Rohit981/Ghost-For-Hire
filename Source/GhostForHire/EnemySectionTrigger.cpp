// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySectionTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySectionTrigger::AEnemySectionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemySectionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemySectionTrigger::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AEnemySectionTrigger::OnOverlapEnd);

	Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	//BoxCollider->OnComponentHit.AddDynamic(this, &AEnemySectionTrigger::OnCompHit);

}


// Called every frame
void AEnemySectionTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActive == true)
	{
		
			if (InteractableObjects.Num() <= 1)
			{
				
				ActiveInteractableObjects1 = InteractableObjects[0];
	
			}
			else
			{
				ActiveInteractableObjects1 = InteractableObjects[0];
				ActiveInteractableObjects2 = InteractableObjects[1];
				
			}

		if (Player->LightSwitch == ActiveInteractableObjects1 || Player->LightSwitch == ActiveInteractableObjects2)
		{
			if ((ActiveInteractableObjects1 != NULL || ActiveInteractableObjects2 != NULL) && Player->LightState == false)
			{
				Player->AI_InvestigateLight = true;
				Enemy->enteredTrigger = true;

			}
		}
	}
	
	
	
}

void AEnemySectionTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Enemy")))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Entered Trigger Collider"));

			Enemy = Cast<AAverageJoeAI>(OtherActor);

			isActive = true;

			

		}
	}
}

void AEnemySectionTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Enemy")))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Exited Trigger Collider"));

			Enemy = Cast<AAverageJoeAI>(OtherActor);

			//Enemy->enteredTrigger = false;

			isActive = false;

			Enemy->TaskActors[0] = NULL;

		}
	}
}


