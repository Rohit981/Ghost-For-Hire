// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "CustomSpringArmComponent.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = Camera, meta = (BlueprintSpawnableComponent), hideCategories = (Mobility))
class GHOSTFORHIRE_API UCustomSpringArmComponent : public USpringArmComponent

{
    GENERATED_BODY()

private:
    FVector PrevHitInterpLoc;

    float CurrentHitReturnInterpTime;
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings, meta = (ClampMin = "0.0", ClampMax = "1000.0", EditCondition = EnableCollision))

    float HitInterpSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings, meta = (EditCondition = EnableCollision))

    float HitReturnInterpTime;


protected:

    virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime) override;

};



