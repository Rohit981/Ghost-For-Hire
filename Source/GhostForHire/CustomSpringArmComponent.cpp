// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSpringArmComponent.h"

FVector UCustomSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{

    if (bHitSomething)
    {
        FVector Interp = FMath::VInterpTo(PrevHitInterpLoc, TraceHitLocation, DeltaTime, HitInterpSpeed);

        PrevHitInterpLoc = Interp;

        CurrentHitReturnInterpTime = HitReturnInterpTime;

        return Interp;

    }

    if (CurrentHitReturnInterpTime > 0.0f)
    {
        CurrentHitReturnInterpTime -= DeltaTime;
        FVector Interp = FMath::VInterpTo(PrevHitInterpLoc, DesiredArmLocation, 1.0f - (CurrentHitReturnInterpTime / HitReturnInterpTime), 1.0f);
        PrevHitInterpLoc = Interp;
        return Interp;
    }

    PrevHitInterpLoc = DesiredArmLocation;
    return DesiredArmLocation;

}
