// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFindTaskLOC.h"
#include "AIController.h"
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyFindTaskLOC::UEnemyFindTaskLOC()
{
	NodeName = TEXT("Find Task Location");
}

EBTNodeResult::Type UEnemyFindTaskLOC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	APawn* AIPawn = AIController->GetPawn();

	Enemy = Cast<AEnemy>(AIPawn);

	FVector Task_Location = Enemy->TaskActor->GetActorLocation();

	FVector Origin = AIPawn->GetActorLocation();

	FNavLocation Location;

	/*Location.Location = Task_Location;*/

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSystem != nullptr && NavSystem->GetRandomPointInNavigableRadius(Task_Location, Search_Radius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);

	}
	

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

FString UEnemyFindTaskLOC::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
