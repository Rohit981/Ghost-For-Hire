// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeAI.h"
#include "HellenKellerAI.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API AHellenKellerAI : public AAverageJoeAI
{
	GENERATED_BODY()

public:
	AHellenKellerAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
