// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScareMeter_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	ScareMeter_UI->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	
}



// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool IsHit;

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


	}

	

}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherComp->ComponentHasTag(FName("Player")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Shivering State"));
			is_Shivering = true;

		}
	}
}


