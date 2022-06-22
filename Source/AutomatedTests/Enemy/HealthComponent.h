// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutomatedTests/Enemy/HealthComponent.h"
#include "AutomatedTests/Enemy/HealthAndResistance.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTOMATEDTESTS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void ApplyDamage(const TPair<FName, float>& Damage)
	{
		this->HealthAndResistance.Damage(Damage);
	}
	void ApplyDamage(FName Type, float DamageAmount)
	{
		this->HealthAndResistance.Damage(Type, DamageAmount);
	}
	void Heal(float HealAmount)
	{
		this->HealthAndResistance.Heal(HealAmount);
	}
	UFUNCTION(BlueprintCallable)
	const FHealthAndResistance& GetHealthAndResistance() const
	{
		return HealthAndResistance;
	}
private:
	FHealthAndResistance HealthAndResistance;
		
};
