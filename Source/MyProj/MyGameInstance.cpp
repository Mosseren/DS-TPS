#include "MyGameInstance.h"
#include "Engine/Engine.h"

UMyGameInstance::UMyGameInstance()
{
}


//数据加载逻辑
void UMyGameInstance::LoadData()
{
	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is not assigned in MyGameInstance!"));
		return;
	}
	ItemInfos.Empty();
	ItemInfos.Add(EItemType::Accessory, TArray<UGameItemInfo*>());
	ItemInfos.Add(EItemType::Armor, TArray<UGameItemInfo*>());
	ItemInfos.Add(EItemType::Consumable, TArray<UGameItemInfo*>());
	ItemInfos.Add(EItemType::Shield, TArray<UGameItemInfo*>());
	ItemInfos.Add(EItemType::Weapon, TArray<UGameItemInfo*>());

	 //从数据表加载所有行
	TArray<FName> RowNames = DataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FMyItemInfo* Row = DataTable->FindRow<FMyItemInfo>(RowName, TEXT("LoadData"));
		if (Row)
		{
			UGameItemInfo* NewGameInfo = NewObject<UGameItemInfo>();
			*NewGameInfo = *Row;
			ItemInfos[Row->ItemType].Add(NewGameInfo);
		}
	}
}

 //数据保存逻辑
void UMyGameInstance::SaveData()
{
	//if (!DataTable)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("DataTable is not assigned in MyGameInstance!"));
	//	return;
	//}

	// //遍历当前数据并保存到数据表
	//for (const FMyItemInfo& Data : ItemInfos)
	//{
	//	FName RowName = Data.Name;
	//	FMyItemInfo* Row = DataTable->FindRow<FMyItemInfo>(RowName, TEXT("SaveData"));
	//	if (Row)
	//	{
	//		*Row = Data;  //更新数据
	//		UE_LOG(LogTemp, Log, TEXT("data table updated"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Row not found"));
	//	}
	//}
}
