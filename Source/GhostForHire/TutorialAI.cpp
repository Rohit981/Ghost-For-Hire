// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATutorialAI::ATutorialAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScareMeter_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	ScareMeter_UI->SetupAttachment(RootComponent);

	SurveyEmote_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("SurveyEmote"));
	SurveyEmote_UI->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATutorialAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutorialAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isHearing == true)
	{
		//AnimInstance->Montage_Play(AudioSpookPose, 1.f, EMontagePlayReturnType::Duration, 0.f);
		GetMesh()->PlayAnimation(AudioSpookAnim, isHearing);
	}

	if (isHearing == false)
	{
		GetMesh()->PlayAnimation(NeutralAnim, isHearing);

	}

}


