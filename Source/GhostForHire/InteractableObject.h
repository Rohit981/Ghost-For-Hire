// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "InteractableObject.generated.h"

class UCurveFloat;

UCLASS()
class GHOSTFORHIRE_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float AngleAxis = 0;

	FVector Dimensions = FVector(-300, 0, 0);
	FVector AxisVector = FVector(0, 0, 1);
	float RotationMultiplier = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* VaseShatterSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ChairShatterSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* MetalShatterSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* FlowerShatterSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* RattleSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* ChairRattleSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* MetalRattleSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* FlowerRattleSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* HoverSound;

	int rattleCount;

	int poltergeistCount;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Actor UProperty ------------------------------------------------------|
	//UPROPERTY(VisibleAnywhere)
	//USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//class UBoxComponent* CollisionBox;

	// Widget UProperty -----------------------------------------------------|
	//UPROPERTY(EditAnywhere)
	//TSubclassOf <UUserWidget> WidgetClass;


	// Collision UFunctions -------------------------------------------------|
	/*UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void HighlightObject();
	UFUNCTION(BlueprintCallable)
	void DontHighlightObject();

	// Poltergeist UFunctions -------------------------------------------------|
	UFUNCTION()
	void PoltergeistI();

	UFUNCTION()
	void PoltergeistII(FVector playerLoc, FRotator playerRot);

	// Ability Variables ------------------------------------------------------|
	bool isObjHighlighted;

	bool isAbilityActive = false;

	bool isThrown = false;

	bool canThrow = false;

	bool canRotate = false;

	FVector PlayerLocation;

	bool isPoltergeistActive = false;

	void RotateObject();

	//Audio
	FVector AudioLocation;

	bool VaseRattle = false;
	bool ChairRattle = false;
	bool MetalRattle = false;
	bool FlowerRattle = false;

	

	int rattleTimer = 5;

	// Projectile Component ---------------------------------------------------|
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* Projectile;

	// Destructibles ----------------------------------------------------------|
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructibility")
	int destroyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructibility") 
	bool isFragile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructibility")
	bool isSturdy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructibility")
	bool isTough;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructibility")
	bool hasHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructibility")
	bool bEnemyHit;

	class AAverageJoeAI* AvgJoe;

	bool HitByPolt;

	

	 

private:
	int hitCount = 0;

	float PolterGeistTimer = 0.1f;

	int VaseAudioCount = 0;
	int ChairAudioCount = 0;

	int MetalAudioCount = 0;
	int FlowerAudioCount = 0;

	float PolterElapsedTime;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* PoltergeistVFX;

};
