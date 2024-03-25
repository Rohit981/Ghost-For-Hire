// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ScareStateTask.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_API UScareStateTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UScareStateTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
