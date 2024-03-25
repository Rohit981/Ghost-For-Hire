// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeController.h"
#include "HellenKellerController.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API AHellenKellerController : public AAverageJoeController
{
	GENERATED_BODY()

public:
	AHellenKellerController();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void AIPossess(APawn* const InPawn);
	
};
