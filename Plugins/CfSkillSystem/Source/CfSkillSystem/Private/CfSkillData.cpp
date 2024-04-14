// Fill out your copyright notice in the Description page of Project Settings.


#include "CfSkillData.h"

bool FCfSkillData::NotChain() const
{
	return (RequireSkill1 == NAME_None);
}

bool FCfSkillData::CanChain(const FName CurrentRowName) const
{
	if(NotChain())
		return false;

	for(int32 i=0; i<GetMaxRequireSkill(); ++i)
	{
		if(GetRequireSkill(i) == CurrentRowName)
			return true;
	}

	return false;
}

FName FCfSkillData::GetRequireSkill(int32 Index) const
{
	switch (Index)
	{
	case 0:
		return RequireSkill1;
	case 1:
		return RequireSkill2;
	case 2:
		return RequireSkill3;
	default:
		return NAME_None;
	}
}

float FCfSkillData::GetChargeInputTime(int32 Index) const
{
	switch (Index)
	{
	case 1:
		return ChargeInputTime2;
	case 2:
		return ChargeInputTime3;
	}
	return ChargeInputTime1;
}

float FCfSkillData::GetChargeDamageRate(int32 Index) const
{
	switch (Index)
	{
	case 0:
		return ChargeDamageRate1;
	case 1:
		return ChargeDamageRate2;
	case 2:
		return ChargeDamageRate3;
	}
	return 1.0f;
}

int32 FCfSkillData::GetChargeLevel(float ChargeTime) const
{
	for(int32 i=0; i<GetMaxCharge(); ++i)
	{
		if(ChargeTime < GetChargeInputTime(i))
		{
			return i;
		}
	}
	return GetMaxCharge() - 1;
}
