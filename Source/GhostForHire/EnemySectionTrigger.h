// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AverageJoeAI.h"
#include "GhostForHireCharacter.h"
#include "GameFramework/Actor.h"
#include "EnemySectionTrigger.generated.h"

UCLASS()
class GHOSTFORHIRE_API AEnemySectionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySectionTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	TArray<AActor*> InteractableObjects;

	AAverageJoeAI* Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	AActor* ActiveInteractableObjects1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	AActor* ActiveInteractableObjects2;

	AGhostForHireCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	bool isActive = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
