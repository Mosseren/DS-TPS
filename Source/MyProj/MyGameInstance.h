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

	// 加载数据表
	UFUNCTION(BlueprintCallable, Category = "Data Management")
	void LoadData();

	// 保存数据
	UFUNCTION(BlueprintCallable, Category = "Data Management")
	void SaveData();

	// 临时存储的数据
	TMap<EItemType, TArray<UGameItemInfo*>> ItemInfos;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DataTable;

};
