// Fill out your copyright notice in the Description page of Project Settings.


#include "CfHitBoxComponent.h"

#include "CfCheatManager.h"

UCfHitBoxComponent::UCfHitBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCfHitBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		CheatManager = Cast<UCfCheatManager>(PC->CheatManager);
	}
}

void UCfHitBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CheatManager)
	{
		if(bHiddenInGame && CheatManager->IsShowHitBox())
		{
			SetHiddenInGame(false);
		}
		else if(!bHiddenInGame && !CheatManager->IsShowHitBox())
		{
			SetHiddenInGame(true);
		}
	}
}
