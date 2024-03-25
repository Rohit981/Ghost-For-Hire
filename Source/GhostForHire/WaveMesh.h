// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "InteractableObject.h"
#include "GameFramework/Actor.h"
#include "WaveMesh.generated.h"

UCLASS()
class GHOSTFORHIRE_API AWaveMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveMesh();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ColliderPlane;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AInteractableObject* InteractableObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Force, meta = (AllowPrivateAccess = "true"))
		float impluseValue;

	float lifeSpawn;

	float MeshSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
