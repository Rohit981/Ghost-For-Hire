// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontDoorCollider.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFrontDoorCollider::AFrontDoorCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFrontDoorCollider::BeginPlay()
{
	Super::BeginPlay();
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFrontDoorCollider::OnOverlapBegin);

	Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
}

void AFrontDoorCollider::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Enemy")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Entered Front Door Collider"));

			Enemy = Cast<AAverageJoeAI>(OtherActor);

			if (Enemy->ScareLevel >= 100)
			{
				Player->EnemyScareCount += 1;

				Enemy->isAlive = false;

				Enemy->SetActorHiddenInGame(true);
				Enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			}



		}
	}
}

// Called every frame
void AFrontDoorCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

