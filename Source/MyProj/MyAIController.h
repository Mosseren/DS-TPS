// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJ_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;

protected:
	void BeginPlay() override;

private:
	// �ڰ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	// ��Ϊ�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	// AI��֪���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	/*class ACharacter* EnemyCharacter;*/

};
