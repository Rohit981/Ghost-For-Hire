// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyMoveToAbilities.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API UEnemyMoveToAbilities : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyMoveToAbilities();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
