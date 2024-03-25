// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeAI.h"
#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"

UCLASS()
class GHOSTFORHIRE_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AIRandomization();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAverageJoeAI> AIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<class AAverageJoeAI>> RandomAIClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
