// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/CfActionSkill.h"
#include "GameFramework/Character.h"
#include "HUD/CfOverlayLockOnComponent.h"
#include "CfCameraBoomComponent.h"
#include "CfSkillData.h"

void UCfActionSkill::InitSkill(ACharacter* InOwner, UCfActionComponent* InComponent, const FCfSkillData* InSkillTable)
{
	Super::InitAction(InOwner, InComponent, InSkillTable->Montage);
	SkillTable = InSkillTable;
}

FName UCfActionSkill::GetActionName() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName;
	}
	return Super::GetActionName();
}

FString UCfActionSkill::ToString() const
{
	if(GetSkillTable())
	{
		return GetSkillTable()->RowName.ToString();
	}
	return Super::ToString();
}

bool UCfActionSkill::CanMoveDuring() const
{
	return (Owner && SkillTable && SkillTable->MobileType == ECfMobileType::Input);
}

bool UCfActionSkill::CanInputAutoRapid() const
{
	return SkillTable && SkillTable->InputType == ECfInputType::AutoRapid;
}

bool UCfActionSkill::HasSkillKey(ECfSkillKey InSkillKey) const
{
	return SkillTable && SkillTable->InputKey == InSkillKey;
}

void UCfActionSkill::OnBegin()
{
	Super::OnBegin();

	if(SkillTable == nullptr || Owner == nullptr)
		return;

	if(UCfOverlayLockOnComponent* LockonComponent = UCfCameraBoomComponent::GetLockingComponent(Owner->GetWorld()))
	{
		Target = LockonComponent;
	}
	else
	{
		Target = UCfCameraBoomComponent::FindLockingTarget(Owner,  SkillTable->LockingRange, 180.0f);
	}

	if(SkillTable->Orientation == ECfSkillOrientation::AimOriented)
	{
		bReachedDesiredRotation = false;
	}
	else if(SkillTable->Orientation == ECfSkillOrientation::AutoOriented && Target != nullptr)
	{
		bReachedDesiredRotation = false;
	}
	else
	{
		bReachedDesiredRotation = true;
	}

	CameraBoomComponent = Owner->GetComponentByClass<UCfCameraBoomComponent>();
}

void UCfActionSkill::OnTick(float DeltaTime)
{
	check(Owner);
	check(Owner->GetWorld());
	check(SkillTable);

	Super::OnTick(DeltaTime);

	if(Montage == nullptr)
	{
		return;
	}

	if(!bReachedDesiredRotation)
	{
		const float CurrentYaw = Owner->GetActorRotation().Yaw;
		float TargetYaw = Owner->GetBaseAimRotation().Yaw;
		if(SkillTable->Orientation == ECfSkillOrientation::AutoOriented && Target != nullptr)
		{
			TargetYaw = (Target->GetComponentLocation() - Owner->GetActorLocation()).Rotation().Yaw;
		}

		const float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 15.0f);
		Owner->SetActorRotation(FRotator(0, NewYaw, 0));
		if(FMath::IsNearlyEqual(TargetYaw, NewYaw, 0.1f))
		{
			bReachedDesiredRotation = true;
		}
	}

	if(Owner->GetMesh() && Owner->GetMesh()->GetAnimInstance() &&
		Montage->IsValidLowLevel() &&
		SkillTable->MobileType == ECfMobileType::Curve)
	{
		const float ForwardValue = Owner->GetMesh()->GetAnimInstance()->GetCurveValue("ForwardMovement");
		const float UpValue = Owner->GetMesh()->GetAnimInstance()->GetCurveValue("UpMovement");
		const float YawValue = Owner->GetMesh()->GetAnimInstance()->GetCurveValue("YawRotation");

		const float ForwardSpeed = (ForwardValue - PrevForwardValue) / DeltaTime;
		const float UpSpeed = (UpValue - PrevUpValue) / DeltaTime;
		const float YawSpeed = (YawValue - PrevYawValue) / DeltaTime;

		PrevForwardValue = ForwardValue;
		PrevUpValue = UpValue;
		PrevYawValue = YawValue;

		const FRotator Rotator(0.f, Owner->GetActorRotation().Yaw + YawSpeed * DeltaTime, 0.f);
		Owner->SetActorRotation(Rotator);

		const FVector ForwardDirection = Owner->GetActorForwardVector();
		const FVector MovementDirection = ForwardSpeed * ForwardDirection + FVector(0.f, 0.f, UpSpeed);
		Owner->GetCharacterMovement()->MoveSmooth(MovementDirection, DeltaTime);
	}
}

void UCfActionSkill::OnButtonReleased(const ECfSkillKey InSkillKey)
{
	if(AnimInstance)
	{
		const FName SectionName = TEXT("Action");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}
