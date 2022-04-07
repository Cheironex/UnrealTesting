#include "AutomatedTests/Enemy/HealthAndResistance.h"

void FHealthAndResistance::Heal(float HealAmount)
{
    if (bIsDead || HealAmount < 0.f)
    {
        return;
    }

    this->Health += HealAmount;
}

void FHealthAndResistance::Damage(FName Type, float DamageAmount)
{
    if (bIsDead || DamageAmount < 0.f)
    {
        return;
    }

    const auto ResistanceMutli = this->Resistances.Contains(Type) ? this->Resistances[Type] : 0.f;

    DamageAmount -= ResistanceMutli / 100 * DamageAmount;

    this->Health -= DamageAmount;

    if (this->Health <= 0.f)
    {
        this->bIsDead = true;
    }

   this->Health = FMath::Clamp(this->Health, 0.f, 100.f);
}

void FHealthAndResistance::Damage(const TPair<FName, float> &Damage)
{
    if (bIsDead)
    {
        return;
    }
    this->Damage(Damage.Key, Damage.Value);
}