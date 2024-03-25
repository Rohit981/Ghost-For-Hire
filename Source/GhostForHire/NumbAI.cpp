// Fill out your copyright notice in the Description page of Project Settings.


#include "NumbAI.h"

ANumbAI::ANumbAI()
{
}

void ANumbAI::BeginPlay()
{
	Super::BeginPlay();
}

void ANumbAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player->LightSwitch != nullptr && enteredTrigger == true)
	{
		TaskActors[0] = Player->LightSwitch;
	}

	
}
