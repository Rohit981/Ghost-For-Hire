// Fill out your copyright notice in the Description page of Project Settings.


#include "HellenKellerController.h"

AHellenKellerController::AHellenKellerController()
{
}

void AHellenKellerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBehaviorTree();
}

void AHellenKellerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartTask();

	
}

void AHellenKellerController::AIPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	OnPosses(InPawn);
}
