// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CfHUD.generated.h"

/**
 * HUD, 각종 디버그 렌더링
 */
UCLASS()
class CFSKILLSYSTEM_API ACfHUD : public AHUD
{
	GENERATED_BODY()

#pragma region Instance
	static ACfHUD* GetInstance(UWorld* World);
#pragma endregion


#pragma region HUDWidget & etc
public:
	static class UCfHUDWidget* GetHUDWidget(UWorld* World);
	static bool GetAimWorldTransform(UWorld* World, FVector& WorldAimLocation, FVector& WorldAimDirection, FVector& WorldCenterLocation);
	static void RegisterTargetWidget(UWorld* World, class UCfOverlayLockOnComponent* InTarget);
	static void UnregisterTargetWidget(UWorld* World);
	static void ToggleTargetWidget(UWorld* World, UCfOverlayLockOnComponent* InTarget);
	static UCfOverlayLockOnComponent* GetLockingTarget(UWorld* World);
	static FVector2D ToAbsolute(UWorld* World, const FVector2D& Local);

protected:
	UPROPERTY()
	class UCfHUDWidget* HUDWidget;
#pragma endregion


#pragma region Draw Debug Info
public:
	virtual void DrawHUD() override;

private:
	static FString PrintfImpl(const TCHAR* Fmt, ...);
	void DrawInfo(UWorld* World, const FVector& Location, const FColor Color, const float Scale, const FString& Text);

public:
	template <typename FmtType, typename... Types>
	void DrawActorInfo(const AActor* Actor, const FColor Color, const float Scale, const FmtType& Fmt, Types... Args)
	{
		const FString Text = PrintfImpl((const TCHAR*)Fmt, Args...);
		DrawInfo(Actor->GetWorld(), Actor->GetActorLocation(), Color, Scale, Text);
	}

private:
	void DrawPlayerInfo();
#pragma endregion
};
