// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UfAnimNotify_SkillEnd.generated.h"

/**
 * 
 */
UCLASS()
class UFSKILLSYSTEM_API UUfAnimNotify_SkillEnd : public UAnimNotify
{
	GENERATED_BODY()

	UUfAnimNotify_SkillEnd(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override { return TEXT("3_End"); }
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
