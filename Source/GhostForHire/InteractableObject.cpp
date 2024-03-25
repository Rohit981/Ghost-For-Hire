// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "GhostForHireCharacter.h"
#include "Engine/Engine.h"
#include <Kismet/GameplayStatics.h>
#include "Perception/AISense_Hearing.h"
#include "AverageJoeAI.h"


// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This is problematic, most likely gone
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;

	// Create Static Mesh Component ============= This works but nathan would just have to lay everything out again
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(Mesh); // works
	//FAtt
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//CollisionBox->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	//CollisionBox->SetupAttachment(RootComponent);

	//Mesh->SetupAttachment(RootComponent);
	//Mesh->SetRelativeLocation(FVector(1, 1, 1));
	//Mesh->SetupAttachment(Root);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));



	Projectile->InitialSpeed = 3000.0f;
	Projectile->MaxSpeed = 3000.0f;
	Projectile->bShouldBounce = true;
	Projectile->Bounciness = 0.3f;
	Projectile->ProjectileGravityScale = 0.5f;
	Projectile->bSimulationEnabled = false;
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &AInteractableObject::OnHit);

}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DontHighlightObject();

	if (canRotate == true)
	{
		RotateObject();
	}

	if (hitCount == 1)
	{
		hasHit = true;
	}

	if (hasHit == true)
	{

		PolterGeistTimer -= DeltaTime;

		if (PolterGeistTimer <= 0)
		{
			
			this->Destroy();

			

			hasHit = false;
		}


	}
	
	


}

void AInteractableObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && hitCount == 0)
	{

		AudioLocation = GetActorLocation();

		isPoltergeistActive = false;

		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PoltergeistVFX, GetActorLocation(), FRotator(0.f), FVector(1.f), true, true);

		

		UAISense_Hearing::ReportNoiseEvent(GetWorld(), AudioLocation, 1.f, this, 0.f, FName(TEXT("CanHear")));

		

		hitCount++;

		

		
	}

	

}


void AInteractableObject::HighlightObject()
{
	if (Mesh)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("it gets here shold be highlighted"));
		Mesh->SetRenderCustomDepth(true);
		isObjHighlighted = true;
		Mesh->SetCustomDepthStencilValue(1);
		//Mesh->SetSimulatePhysics(false);
/*		FVector currLoc = GetActorLocation();
		FVector newPos = FVector(currLoc.X, currLoc.Y, currLoc.Z + 100.f);
		SetActorLocation(newPos)*/;
		//Mesh->SetCustomDepthStencilValue(1);
	}
}

void AInteractableObject::DontHighlightObject()
{
	if (Mesh && !isObjHighlighted) // this is messing up the highlight? its fixed here but idk
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("it gets here"));

		Mesh->SetRenderCustomDepth(false);
		//isObjHighlighted = false;
		//Mesh->SetCustomDepthStencilValue(1);
	}
	if (isAbilityActive)
	{
		isObjHighlighted = true;
	}
	else
	{
		isObjHighlighted = false;
	}
}

void AInteractableObject::PoltergeistI()
{

	//FVector currLoc = GetActorLocation();
	//FVector newPos = FVector(currLoc.X, currLoc.Y, currLoc.Z + 100.f);
	//SetActorLocation(newPos);
	//AGhostForHireCharacter* GFH = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


	//if (GFH->hitter == this) {
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Actor: %s"), GFH->hitter));
	//	HighlightObject();
	//}
	if (isObjHighlighted) {
		isAbilityActive = true;
		FRotator OGRotation = GetActorRotation();

		


		//float RandomPitch = FMath::RandRange(-100.0f, 100.0f);
		//float RandomYaw = FMath::RandRange(-100.0f, 100.0f);
		//float RandomRoll = FMath::RandRange(-100.0f, 100.0f);

		float RandomPitch = FMath::RandRange(-10.0f, 10.0f);
		float RandomYaw = FMath::RandRange(-10.0f, 10.0f);
		float RandomRoll = FMath::RandRange(-10.0f, 10.0f);

		//FRotator Jitter(RandomPitch, RandomYaw, RandomRoll);
		//FRotator Jitter(GetActorRotation().Pitch, RandomYaw, GetActorRotation().Roll);
		FRotator Jitter(RandomPitch, GetActorRotation().Yaw, RandomRoll);

		// Calculate the target rotation by adding the random rotation
		FRotator TargetRotation = OGRotation + Jitter;

		// Interpolate smoothly over time and adjust the speed
		float InterpSpeed = 5.0f;
		FRotator SmoothRotation = FMath::RInterpTo(OGRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		isPoltergeistActive = true;

		// Set the new rotation to your actor
		Mesh->SetWorldRotation(SmoothRotation);

		rattleCount++;


	}

	if (rattleCount <= 1)
	{
		if(VaseRattle == true)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RattleSound, GetActorLocation());

		if(ChairRattle == true)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChairRattleSound, GetActorLocation());

		if (MetalRattle == true)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MetalRattleSound, GetActorLocation());

		if (FlowerRattle == true)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlowerRattleSound, GetActorLocation());

	}




}

void AInteractableObject::PoltergeistII(FVector playerLoc, FRotator playerRot)
{
	if (isObjHighlighted) {
		isAbilityActive = true;
		Mesh->SetSimulatePhysics(false);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("gets here"));
		float interpSpeed = 10.f;
		float zOffset = 10.f;
		FVector realLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + zOffset);
		FVector frontLoc = FVector(GetActorLocation().X + 100.f, 0, 0);
		//FVector SmoothLocation = FMath::VInterpTo(realLocation, playerLoc, GetWorld()->GetDeltaSeconds(), interpSpeed);
		FVector SmoothLocation = FMath::VInterpTo(realLocation, playerLoc, GetWorld()->GetDeltaSeconds(), interpSpeed);
		FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), playerRot, GetWorld()->GetDeltaSeconds(), interpSpeed);

		isPoltergeistActive = true;

		// Rotations don't really work
		//SetActorLocation(SmoothLocation, false, nullptr, ETeleportType::TeleportPhysics);
		SetActorLocation(SmoothLocation, false, nullptr, ETeleportType::TeleportPhysics);
		//SetActorRotation(SmoothRotation);
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), HoverSound, GetActorLocation());

		//poltergeistCount++;
	}

	/*if (poltergeistCount <= 1)
	{

	}*/
}

void AInteractableObject::RotateObject()
{
	// declare size of radius to move around
	FVector Radius = FVector(5, 0, 0);

	// angle increases by 1 every frame
	AngleAxis++;

	// prevent number from growind indefinitely
	if (AngleAxis > 360.0f) {

		AngleAxis = 1;
	}

	FVector RotateValue = Radius.RotateAngleAxis(AngleAxis, FVector(0, 0, 1));

	PlayerLocation.X += RotateValue.X;
	PlayerLocation.Y += RotateValue.Y;
	PlayerLocation.Z += RotateValue.Z;

	SetActorLocation(PlayerLocation);
}





