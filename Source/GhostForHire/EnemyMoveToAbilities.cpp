// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMoveToAbilities.h"
#include "AIController.h"
#include "AverageJoeController.h"
#include "NavigationSystem.h"
#include "AverageJoeAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UEnemyMoveToAbilities::UEnemyMoveToAbilities()
{
	NodeName = TEXT("Move To Ability");
}

EBTNodeResult::Type UEnemyMoveToAbilities::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	//FVector TaskLoc = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	AAverageJoeAI* Enemy = Cast<AAverageJoeAI>(AIController->GetPawn());


	AActor* TaskActor = Enemy->AbilityTask;

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("TaskActor: %s"), *TaskActor->GetActorNameOrLabel()));


	//MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 800;



	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TaskLoc);

	UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, TaskActor);



	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
