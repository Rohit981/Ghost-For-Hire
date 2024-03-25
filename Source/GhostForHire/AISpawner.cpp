// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawner.h"


// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	AIRandomization();
	
}

void AAISpawner::AIRandomization()
{
	int RanNum;
	for (int i = 0; i < RandomAIClass.Num(); i++)
	{
		RanNum = FMath::RandRange(0, i);
	}
	AIClass = RandomAIClass[RanNum];

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AI is Randomized"));

	UWorld* const World = GetWorld();
	//AEctoSplatObject* Hurl;
	if (World != nullptr)
	{
		FVector Location = GetActorLocation(); //+ FVector(100.f, 100.f, 0);
		Location = FVector(GetActorLocation().X, GetActorLocation().Y, (GetActorLocation().Z));
		FVector ForwardVector = GetActorForwardVector() * 100.f; //+ FVector(100.f, 100.f, 0);
		//FVector ForwardVector = FollowCamera->GetForwardVector() * 100.f;
		FVector ForwardLocation = Location + ForwardVector;
		FRotator Rotation = GetActorRotation();


		if (AIClass != nullptr)
		{
			AAverageJoeAI* RandomAI = World->SpawnActor<AAverageJoeAI>(AIClass, ForwardLocation, Rotation);

		}

	}
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

