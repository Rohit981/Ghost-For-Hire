// Fill out your copyright notice in the Description page of Project Settings.


#include "EctoSplatObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "UObject/Object.h"
#include "AverageJoeAI.h"
#include "Kismet/GameplayStatics.h"
#include "AverageJoeController.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEctoSplatObject::AEctoSplatObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));

	//========================================================================
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); // this one
	CollisionSphere->SetSphereRadius(100.f); // this one
	CollisionSphere->SetupAttachment(Mesh);
	//CollisionSphere->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform); // this one
	//CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Mesh->SetupAttachment(CollisionSphere); // this one
	Mesh->SetupAttachment(RootComponent);
	//CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Mesh->AttachToComponent(CollisionSphere, FAttachmentTransformRules::KeepRelativeTransform);
	//========================================================================
	//CollisionSphere->SetHiddenInGame(false);
	//CollisionSphere->bHiddenInGame = false;

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	

	Projectile->InitialSpeed = 1000.0f;
	Projectile->MaxSpeed = 1000.0f;
	Projectile->bShouldBounce = true;
	Projectile->Bounciness = 0.3f;
	Projectile->ProjectileGravityScale = 0.5f;
	Projectile->bSimulationEnabled = true;


}

// Called when the game starts or when spawned
void AEctoSplatObject::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEctoSplatObject::OnOverlapBegin);
	//CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AEctoSplatObject::OnOverlapEnd);
	//CollisionSphere->OnComponentHit.AddDynamic(this, &AEctoSplatObject::OnComponentHit);
	//CollisionSphere->OnComponentHit.AddDynamic(this, &AEctoSplatObject::OnHit);
	Projectile->OnProjectileStop.AddDynamic(this, &AEctoSplatObject::OnProjectileStop);
	//Projectile->OnProjectileBounce.AddDynamic(this, &AEctoSplatObject::OnProjectileBounce);
	//this->OnActorHit.AddDynamic(this, &AEctoSplatObject::OnActorHit);
	//CollisionSphere->OnComponentHit.AddDynamic(this, &AEctoSplatObject::OnActorHit);
	//CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APassiveObject::OnOverlapBegin);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EctoSplatEjectionSound, GetActorLocation());



	//LaunchAngleInRadians = CalculateLaunchAngle(1000.f, 500.f, 10000.f);

	//LaunchVelocity.X = FMath::Cos(LaunchAngleInRadians) * InitialSpeed;
	//LaunchVelocity.Y = FMath::Sin(LaunchAngleInRadians) * InitialSpeed;

	//Projectile->Velocity = LaunchVelocity;
}

// Called every frame
void AEctoSplatObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InterpLocation();

	if (isEctoActive == true)
	{
		ectoDestroyTimer += DeltaTime;

		
	}

	if (ectoDestroyTimer >= 3)
	{
		ectoDestroyTimer = 0;

		

		isEctoActive = false;

		Destroy();
		squishing = false;
	}

}

void AEctoSplatObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if ( OtherActor->ActorHasTag(FName("Enemy"))) {
			AAverageJoeAI* Enemy = Cast<AAverageJoeAI>(OtherActor);
			//Enemy->bEctoted = true;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Enemy COllided with ecto plasm"));

			Enemy->bEctoted = true;

			isEctoActive = true;

			//Enemy->ScareLevel += Enemy->HearingScareFactor * GetWorld()->DeltaTimeSeconds;
		}
		if (OtherComp->ComponentHasTag(FName("Floor")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Actor Hit component"));

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EctoSplatSettleSound, GetActorLocation());

		}
	}

}

void AEctoSplatObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Enemy")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Enemy COllision Ended with ecto plasm"));

			AAverageJoeAI* Enemy = Cast<AAverageJoeAI>(OtherActor);
			
		}
	}*/
}

void AEctoSplatObject::OnProjectileBounce(const FHitResult& HitResult, const FVector& ImpactVelocity)
{
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();

	if (HitResult.IsValidBlockingHit())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Here"));
		if (HitComponent->ComponentHasTag(FName("Floor"))) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("GETTING HERE IN SPLATOBJ"));
			Projectile->Velocity = FVector::ZeroVector;
		}

		Projectile->ProjectileGravityScale = 1;
	}
}

void AEctoSplatObject::OnProjectileStop(const FHitResult& HitResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Here"));
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	if (HitComponent->ComponentHasTag(FName("Floor")))
	{
		Projectile->bShouldBounce = false;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Actor Hit in PStop"));

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EctoSplatSettleSound, GetActorLocation());
		hasLanded = true;
		EctoSplatActive = true;
	}
}

void AEctoSplatObject::InterpLocation()
{
	// Method 1 = bounces once and will drop to the ground
	if (EctoHurlActive) {
		float InterpSpeed = 10.0f;
		FVector realLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0);
		FVector SmoothLocation = FMath::VInterpTo(GetActorLocation(), realLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Here"));
		SetActorLocation(SmoothLocation);
	}

	if (hasLanded || Projectile->Velocity == FVector::ZeroVector)
	{
		squishing = true;
		//FTransform ActorTransform = GetActorTransform();
		//FVector Location = ActorTransform.GetLocation();
		//FRotator Rotation = ActorTransform.GetRotation().Rotator();
		//FVector Scale(1.0f, 1.0f, 0.1f);

		//FTransform CombinedTransform(Rotation.Quaternion(), Location, Scale);

		//float InterpSpeed = 5.0f;

		//FTransform NewTransform = FMath::Lerp(GetActorTransform(), CombinedTransform, FMath::Clamp(InterpSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f));

		//// Set the actor's transform
		//SetActorTransform(NewTransform);

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Here"));

		FVector Squished(1.0f, 1.0f, .1f);

		float LerpSpeed = 5.0f;

		FVector ballScale = Mesh->GetRelativeScale3D();
		FVector SmoothSquish = FMath::Lerp(ballScale, Squished, LerpSpeed * GetWorld()->GetDeltaSeconds());

		Mesh->SetRelativeScale3D(SmoothSquish);
		CollisionSphere->SetRelativeScale3D(FVector(1.0f, 1.0f, 10.0f));
	}
}





