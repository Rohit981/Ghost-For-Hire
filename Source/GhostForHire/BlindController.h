// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeController.h"
#include "BlindController.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API ABlindController : public AAverageJoeController
{
	GENERATED_BODY()

public:
	ABlindController();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void AIPossess(APawn* const InPawn);

	void BlindPolterGiest();

	float BlindPolterTimer;

	
	
};
