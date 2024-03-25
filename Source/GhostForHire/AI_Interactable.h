// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "AI_Interactable.generated.h"

UCLASS()
class GHOSTFORHIRE_API AAI_Interactable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAI_Interactable();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Object;

	UPROPERTY(EditAnywhere, Category = "Name")
	FString Label;

	UPROPERTY(EditAnywhere,BluePrintReadWrite)
	bool wasHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Highlight();

	UFUNCTION(BlueprintCallable)
	void DontHighlight();

	UPROPERTY(BlueprintReadWrite)
	bool isHighlighted;


};
