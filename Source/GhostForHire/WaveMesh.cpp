// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveMesh.h"

// Sets default values
AWaveMesh::AWaveMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	ColliderPlane->SetupAttachment(RootComponent);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	

}

// Called when the game starts or when spawned
void AWaveMesh::BeginPlay()
{
	Super::BeginPlay();

	//Added Comment 
	ColliderPlane->OnComponentBeginOverlap.AddDynamic(this, &AWaveMesh::OnOverlapBegin);

	
}

// Called every frame
void AWaveMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lifeSpawn += DeltaTime;

	MeshSize +=  0.05*DeltaTime;

	ColliderPlane->SetRelativeScale3D(ColliderPlane->GetRelativeScale3D() + MeshSize);

	if (lifeSpawn >= 2)
	{
		Destroy();

	}

}

void AWaveMesh::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (OtherComp->ComponentHasTag(FName("Interactable")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Collided with interactable Object"));
			InteractableObject = Cast<AInteractableObject>(OtherActor);

			InteractableObject->Mesh->SetSimulatePhysics(true);

			InteractableObject->Mesh->AddImpulse(InteractableObject->GetActorLocation() * impluseValue);

		}
	}
}

