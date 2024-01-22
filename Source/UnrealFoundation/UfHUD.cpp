// Fill out your copyright notice in the Description page of Project Settings.


#include "UfHUD.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "CfUtil.h"
#include "CfActionComponent.h"
#include "CfHUDWidget.h"
#include "UfCharacter.h"
#include "UfGameMode.h"

void AUfHUD::BeginPlay()
{
	Super::BeginPlay();

	if(AUfGameMode* Mode = Cast<AUfGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if ( const TObjectPtr< UCfHUDWidget > Widget = CreateWidget< UCfHUDWidget >( GetWorld(), Mode->HudWidgetClass ) )
		{
			Widget->AddToViewport();
		}
	}
}
