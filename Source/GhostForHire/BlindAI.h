// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeAI.h"
#include "BlindAI.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API ABlindAI : public AAverageJoeAI
{
	GENERATED_BODY()

public:
	ABlindAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
