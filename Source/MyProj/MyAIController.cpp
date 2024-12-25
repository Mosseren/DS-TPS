// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

AMyAIController::AMyAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AMyAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	if (!IsValid(InPawn)) {
		return;
	}

	/*EnemyCharacter = Cast<ACharacter>(InPawn);*/
	/*if (!IsValid(EnemyCharacter)) {
		return;
	}*/

	if (IsValid(BlackboardComponent->GetBlackboardAsset())) {
		BlackboardComponent->InitializeBlackboard(*(BlackboardComponent->GetBlackboardAsset()));
		BehaviorTreeComponent->StartLogic();
	}
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

}