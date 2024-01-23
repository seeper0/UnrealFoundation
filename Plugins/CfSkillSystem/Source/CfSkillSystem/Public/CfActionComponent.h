// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CfSkill.h"
#include "GameFramework/Character.h"
#include "CfActionComponent.generated.h"


UCLASS( ClassGroup=(Cf), meta=(BlueprintSpawnableComponent) )
class CFSKILLSYSTEM_API UCfActionComponent : public UActorComponent
{
	GENERATED_BODY()

	struct FSkillInfo
	{
		bool PreInput = false;
		const struct FCfSkillData* SkillData = nullptr;
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

	const FCfSkillData* GetDesiredSkill(const TArray<FName>& RowNames) const;
	void InputSkill(const FCfSkillData* InSkillData);
	void HitReaction(const FCfDamageEvent& DamageEvent);

private:
	bool CanCancelSkill(const FCfSkillData* InSkillData) const;
	bool CanPlaySkill(const FCfSkillData* InSkillData) const;
	void PlaySkill(const FCfSkillData* InSkillData);
	void PlayAction(UAnimMontage* InMontage, const FCfSkillData* InSkillData);
	void TickAction();
	void ClearAction();

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	void ReserveSkill(const FCfSkillData* InSkillData);
	void TickInput();
	
	/** Skill Table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillTable = nullptr;

	UPROPERTY()
	TMap<const UInputAction*, ECfSkillKey> SkillSlotCache;
	
	UPROPERTY()
	ECfSkillState SkillState = ECfSkillState::None;

	UPROPERTY()
	ACharacter* OwnerChar = nullptr;

	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr; 

	UPROPERTY()
	class UCfActionBase* CurrentAction = nullptr;

	UPROPERTY()
	FName ReservedRowName;

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