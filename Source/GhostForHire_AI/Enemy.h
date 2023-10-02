// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Enemy.generated.h"

UCLASS()
class GHOSTFORHIRE_AI_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float doingTaskTime = 0;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* ScareMeter_UI;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
		AActor* TaskActor;

	float taskTime = 0;

	bool canTrace = false;

	bool is_Shivering = false;

	//Scare Level
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
		float ScareLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float LightScareFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float ShiverScareFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare Level", meta = (AllowPrivateAccess = true))
		float HearingScareFactor;


	int LightScareNumber = 0;
	int ShiverScareNumber = 0;
	int HearingScareNumber = 0;

		
	
};
