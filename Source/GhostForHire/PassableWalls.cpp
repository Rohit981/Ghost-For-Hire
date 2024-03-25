// Fill out your copyright notice in the Description page of Project Settings.


#include "PassableWalls.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APassableWalls::APassableWalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(Root);

	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


}

// Called when the game starts or when spawned
void APassableWalls::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APassableWalls::OnBeginOverlap);

	Mesh->OnComponentEndOverlap.AddDynamic(this, &APassableWalls::OnEndOverlap);

	Player = Cast<AGhostForHireCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//Player->setspringarm

	//if (!IsPassible)
	//{
	//	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	//}
}

// Called every frame
void APassableWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player->resetArmLength)
	{
		Mesh->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Block);
		//Player->resetArmLength = false;
	}

	if (IsBlockingPlayer)
	{
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Location: %f"), Player->GetVelocity().Z));
	if (bFloorUp)
	{
		FVector PlayerLoc = Player->GetActorLocation();
		FVector GoUp = FVector(PlayerLoc.X, PlayerLoc.Y, PlayerLoc.Z + 200.f);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Floor"));
		PlayerLoc = GetActorLocation();
		FVector NewPlayerLoc = FMath::VInterpTo(Player->GetActorLocation(), GoUp, GetWorld()->DeltaTimeSeconds, 1.0f);
		Player->SetActorLocation(NewPlayerLoc);
		//if (FMath::IsNearlyEqual(GoUp.Z, Player->GetActorLocation().Z, 50.0f))
		//{
		//	bFloorUp = false;
		//}
	}

	if (bFloorDown)
	{
		FVector PlayerLoc = Player->GetActorLocation();
		FVector GoUp = FVector(PlayerLoc.X, PlayerLoc.Y, PlayerLoc.Z - 200.f);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Floor"));
		PlayerLoc = GetActorLocation();
		FVector NewPlayerLoc = FMath::VInterpTo(Player->GetActorLocation(), GoUp, GetWorld()->DeltaTimeSeconds, 1.0f);
		Player->SetActorLocation(NewPlayerLoc);
		//if (FMath::IsNearlyEqual(GoUp.Z, Player->GetActorLocation().Z, 50.0f))
		//{
		//	bFloorUp = false;
		//}
	}
}

void APassableWalls::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != this && OtherActor->IsA(AGhostForHireCharacter::StaticClass())) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Debugger"));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Location: %f"), Player->GetActorLocation().Z));

		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			// ECTO - PHASE =============================================================================================================
			//FVector Location = Player->GetActorLocation(); //+ FVector(100.f, 100.f, 0);
			//Location = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, Player->GetActorLocation().Z);
			//FVector ForwardVector = Player->GetActorForwardVector() * -10.f; //+ FVector(100.f, 100.f, 0);
			////FVector ForwardVector = FollowCamera->GetForwardVector() * 100.f;
			//FVector ForwardLocation = Location + ForwardVector;
			//FRotator Rotation = Player->GetActorRotation();

			//AEctoSplatObject* Hurl = World->SpawnActor<AEctoSplatObject>(EctoSplatClass, ForwardLocation, Rotation);
			// ECTO - PHASE =============================================================================================================

			//Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
			Player->collidingWithWall = true;
			Mesh->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("E-Hurl"));
		}
		if (OverlappedComp->ComponentHasTag("Floor") && Player->GetVelocity().Z > 0)
		{
			bFloorUp = true;
		}
		if (OverlappedComp->ComponentHasTag("Floor") && Player->GetVelocity().Z < 0)
		{
			bFloorDown = true;
		}
	}
	//if (OtherActor && OtherActor != this && OtherActor->IsA(ASpringArmComponent::StaticClass()))
}

void APassableWalls::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AGhostForHireCharacter::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Left"));
		Player->collidingWithWall = false;
		Player->leavingWallPosition = Player->GetActorLocation();
		//Player->checkWallDistance();
		//Mesh->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Block);
		bFloorUp = false;
		bFloorDown = false;
	}
}


