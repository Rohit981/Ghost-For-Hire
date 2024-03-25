// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EctoSplatObject.generated.h"

UCLASS()
class GHOSTFORHIRE_API AEctoSplatObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEctoSplatObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* EctoSplatEjectionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* EctoSplatSettleSound;

private:
	float ectoDestroyTimer = 0;
	bool isEctoActive = false;

	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Actor UProperty ------------------------------------------------------|
	//UPROPERTY(VisibleAnywhere)
	//USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	// Projectile Component ---------------------------------------------------|
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* Projectile;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool EctoSplatActive;
	bool EctoHurlActive;
	bool hasLanded;

	UFUNCTION()
	void OnProjectileBounce(const FHitResult& HitResult, const FVector& ImpactVelocity);

	UFUNCTION()
	void OnProjectileStop(const FHitResult& HitResult);

	void InterpLocation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool squishing;

	//UFUNCTION()
	//void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);



	//UFUNCTION(BlueprintCallable, Category = "Projectile")
	//float CalculateLaunchAngle(float InitialVelocity, float Range, float Height);

	//float LaunchAngleInRadians;

	//float InitialSpeed = 1000.f;

	//FVector LaunchVelocity = FVector::ZeroVector;

};
