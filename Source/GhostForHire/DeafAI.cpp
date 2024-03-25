// Fill out your copyright notice in the Description page of Project Settings.


#include "DeafAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InteractableObject.h"

ADeafAI::ADeafAI()
{
}

void ADeafAI::BeginPlay()
{
	Super::BeginPlay();

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAverageJoeAI::OnOverlapBegin);
	//EnableOverlap();
}

void ADeafAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player->LightSwitch != nullptr && enteredTrigger == true)
	{
		TaskActors[0] = Player->LightSwitch;
	}

	
}

//void ADeafAI::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != this)
//	{
//		if (OtherComp->ComponentHasTag(FName("Player")))
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Shivering State"));
//			is_Shivering = true;
//
//		}
//	}
//}

