// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMoveTOTask.h"
#include "AIController.h"
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UEnemyMoveTOTask::UEnemyMoveTOTask()
{
	NodeName = TEXT("Move To Task");
}

EBTNodeResult::Type UEnemyMoveTOTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	//FVector TaskLoc = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());

	AActor* TaskActor = Enemy->TaskActor;


	//MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 800;



	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TaskLoc);

	UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, TaskActor);



	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
