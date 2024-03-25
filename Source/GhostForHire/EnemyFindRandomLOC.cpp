// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFindRandomLOC.h"
#include "AIController.h"
#include "AverageJoeController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyFindRandomLOC::UEnemyFindRandomLOC()
{
    NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UEnemyFindRandomLOC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FNavLocation Location;

    AAverageJoeController* AIController = Cast<AAverageJoeController>(OwnerComp.GetAIOwner());

    APawn* AIPawn = AIController->GetPawn();


    FVector Origin = AIPawn->GetActorLocation();



    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    if (NavSystem != nullptr && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
    {
        AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);


    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Nav Mesh Not Found")));
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

    return EBTNodeResult::Succeeded;
}

FString UEnemyFindRandomLOC::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}