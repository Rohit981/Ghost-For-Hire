// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Interactable.h"

// Sets default values
AAI_Interactable::AAI_Interactable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));

	Object->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RootComponent = Object;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(1.f, 3.f, 3.f));
	//RootComponent = CollisionBox;
	//CollisionBox->SetupAttachment(Object);
	//CollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//CollisionBox->AttachToComponent(Object, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetupAttachment(Object);
}

// Called when the game starts or when spawned
void AAI_Interactable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DontHighlight();
}

void AAI_Interactable::Highlight()
{
	if (Object)
	{
		Object->SetRenderCustomDepth(true);
		isHighlighted = true;
	}
}

void AAI_Interactable::DontHighlight()
{
	if (Object)
	{
		Object->SetRenderCustomDepth(false);
		isHighlighted = false;
	}
}

