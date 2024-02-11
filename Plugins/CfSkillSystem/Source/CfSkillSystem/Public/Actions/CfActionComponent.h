// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CfSkill.h"
#include "GameFramework/Character.h"
#include "CfActionComponent.generated.h"


/**
 * 액션을 실행하는 컴퍼넌트, 스킬이나 히트리액션을 관리한다.
 */
UCLASS( ClassGroup=(CombatFoundation), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// uniform initialization 전용 임시 struct, PlayAction argument로 사용된다.
	struct FActionInfo
	{
		FActionInfo() : SkillData(nullptr) , IsValid(false) {}
		FActionInfo(bool InIsValid) : SkillData(nullptr), IsValid(InIsValid) {}
		FActionInfo(const FCfSkillData* InSkillData) : SkillData(InSkillData), IsValid(true) {}
		FActionInfo(const FCfDamageEvent& InDamageEvent) : SkillData(nullptr), DamageEvent(InDamageEvent), IsValid(true) {}

		FActionInfo(const FActionInfo& Info) { operator=(Info); }
		const FActionInfo& operator=(const FActionInfo& Info)
		{
			SkillData = Info.SkillData;
			FCfDamageEvent* Event = const_cast<FCfDamageEvent*>(&DamageEvent);
			*Event = Info.DamageEvent;
			IsValid = Info.IsValid;
			return *this;
		}

		const FCfSkillData* SkillData;
		const FCfDamageEvent DamageEvent;
		bool IsValid = false;
	};

public:
	// Sets default values for this component's properties
	UCfActionComponent();
	inline static FName ComponentName = TEXT("SkillComponent");
	static UCfActionComponent* GetActionComponent(const AActor* Actor);
	static UCfActionComponent* GetActionComponent(const UActorComponent* MeshComp);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetSkillState(ECfSkillState InSkillState);
	ECfSkillState GetSkillState() const { return SkillState; }
	ACharacter* GetOwnerChar() const { return OwnerChar; }
	AController* GetController() const { return OwnerChar ? OwnerChar->Controller : nullptr; }
	const UDataTable* GetSkillTable() const { return SkillTable; }
	class UCfActionBase* GetCurrentAction() const { return CurrentAction; }

	UAnimMontage* GetKnockBackMontage() const { return KnockBackMontage; }
	UAnimMontage* GetDownMontage() const { return DownMontage; }
	UAnimMontage* GetAirborneMontage() const { return AirborneMontage; }	
	UAnimMontage* GetWakeupMontage() const { return WakeupMontage; }	

	const FCfSkillData* GetDesiredSkill(const TArray<FName>& RowNames) const;
	void InputSkill(const FCfSkillData* InSkillData);
	void PlayAction(const FActionInfo& ActionInfo);
	void ReserveAction(const FActionInfo& ActionInfo);
	void SetStun(const float InRecoveryTime, const float InStunPlayRate);
	bool IsSuperArmorActive() const;

private:
	bool CanCancelSkill(const FCfSkillData* InSkillData) const;
	bool CanPlaySkill(const FCfSkillData* InSkillData) const;
	void PlaySkill(const FCfSkillData* InSkillData);
	void TickAction(float DeltaTime);
	void ClearAction();
	void ReserveSkill(const FCfSkillData* InSkillData);

	/** Skill Table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta = (AllowPrivateAccess = "true", DisplayName="KnockBack"))
	UAnimMontage* KnockBackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta = (AllowPrivateAccess = "true", DisplayName="Down"))
	UAnimMontage* DownMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta = (AllowPrivateAccess = "true", DisplayName="Airborne"))
	UAnimMontage* AirborneMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data", meta = (AllowPrivateAccess = "true", DisplayName="Getup"))
	UAnimMontage* WakeupMontage = nullptr;

	UPROPERTY(Transient)
	TMap<const UInputAction*, ECfSkillKey> SkillSlotCache;
	
	UPROPERTY(Transient)
	ECfSkillState SkillState = ECfSkillState::None;

	UPROPERTY(Transient)
	ACharacter* OwnerChar = nullptr;

	UPROPERTY(Transient)
	UAnimInstance* AnimInstance = nullptr; 

	UPROPERTY(Transient)
	class UCfActionBase* CurrentAction = nullptr;

	UPROPERTY(Transient)
	FName ReservedRowName;
	FActionInfo ReverseActionInfo;

#pragma region HitList
public:
	bool HasActorInHitList(const ACharacter* InVictim) const;
	void PushHitActorList(ACharacter* InVictim);
	void ClearHitActorList();

private:
	UPROPERTY()
	TSet<ACharacter*> HitActorList;

#pragma endregion
};