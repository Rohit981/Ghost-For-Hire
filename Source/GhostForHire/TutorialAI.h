// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "TutorialAI.generated.h"

UCLASS()
class GHOSTFORHIRE_API ATutorialAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATutorialAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* ScareMeter_UI;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* SurveyEmote_UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* AudioSpookAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* NeutralAnim;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Bool 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	int Surveylevel = 0;

	bool isHearing = false;

	bool bPoltHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	float ScareLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* HitPolterGeistPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AActor* EndLocation;

};
