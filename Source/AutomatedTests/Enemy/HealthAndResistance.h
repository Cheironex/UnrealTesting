#pragma once
#include "CoreMinimal.h"
#include "HealthAndResistance.generated.h"

USTRUCT(BlueprintType)
struct FHealthAndResistance
{

    GENERATED_BODY()

    FHealthAndResistance() : Health(100.f), Resistances({}), bIsDead(false) {};
    FHealthAndResistance(float Health) : Health(Health), Resistances({}), bIsDead(false) {};
    FHealthAndResistance(float Health, TMap<FName, float>Resistances) : Health(Health), Resistances(Resistances), bIsDead(false) {};
    FHealthAndResistance(float Health, TMap<FName, float>Resistances, bool bIsDead) : Health(Health), Resistances(Resistances), bIsDead(bIsDead) {};


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health = 100.f;

    TMap<FName, float> Resistances;

    bool bIsDead = false;

    void Heal(float HealAmount);
    void Damage(FName Type, float DamageAmount);
    void Damage(const TPair<FName, float>& Damage);

};
