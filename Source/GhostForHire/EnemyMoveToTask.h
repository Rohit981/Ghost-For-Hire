// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyMoveToTask.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API UEnemyMoveToTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyMoveToTask();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
