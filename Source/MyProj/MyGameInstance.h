// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GameItemInfo.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJ_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	// �������ݱ�
	UFUNCTION(BlueprintCallable, Category = "Data Management")
	void LoadData();

	// ��������
	UFUNCTION(BlueprintCallable, Category = "Data Management")
	void SaveData();

	// ��ʱ�洢������
	TMap<EItemType, TArray<UGameItemInfo*>> ItemInfos;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DataTable;

};
