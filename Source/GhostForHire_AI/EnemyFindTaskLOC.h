// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "Enemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyFindTaskLOC.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTFORHIRE_AI_API UEnemyFindTaskLOC : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyFindTaskLOC();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual FString GetStaticDescription() const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		float Search_Radius = 150.f;

	AEnemy* Enemy;
	
};
