// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyMoveTOTask.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_AI_API UEnemyMoveTOTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyMoveTOTask();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
