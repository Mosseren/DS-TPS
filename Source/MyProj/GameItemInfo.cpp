// Fill out your copyright notice in the Description page of Project Settings.


#include "GameItemInfo.h"

UGameItemInfo& UGameItemInfo::operator=(const FMyItemInfo& ItemInfoStruct)
{
	this->ID = ItemInfoStruct.ID;
	this->Name = ItemInfoStruct.Name;
	this->Count = ItemInfoStruct.Count;
	this->Description = ItemInfoStruct.Description;
	this->IsStackable = ItemInfoStruct.IsStackable;
	this->Texture = ItemInfoStruct.Texture;
	this->ItemType = ItemInfoStruct.ItemType;
	return *this;
}
