// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeAI.h"
#include "DeafAI.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API ADeafAI : public AAverageJoeAI
{
	GENERATED_BODY()

public:
	ADeafAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/


	
};
