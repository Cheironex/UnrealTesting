#include "Misc/AutomationTest.h"
#include "AutomatedTests/Enemy/HealthAndResistance.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthAndResistanceTestConstructors, "Project.UnitTests.HealthAndResistanceUnitTests.Constructors", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthAndResistanceTestHeal, "Project.UnitTests.HealthAndResistanceUnitTests.Heal", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthAndResistanceTestDamageNoRes, "Project.UnitTests.HealthAndResistanceUnitTests.DamageNoRes", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthAndResistanceTestDamageRes, "Project.UnitTests.HealthAndResistanceUnitTests.DamageRes", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
const auto DefaultHealth = 100.f;
const TMap<FName, float> DefaultResistances = {};
const bool DefaultbIsDead = false;

namespace
{
	bool IsDefault(const FHealthAndResistance &DefaultHealthAndResistances)
	{
		auto bIsDefault = false;
		bIsDefault = DefaultHealthAndResistances.Health == DefaultHealth && DefaultHealthAndResistances.Resistances.Num() == DefaultResistances.Num() && DefaultHealthAndResistances.bIsDead == DefaultbIsDead;

		return bIsDefault;
	}

	template <typename KeyType, typename ValueType>
	bool AreMapEqual(const TMap<KeyType, ValueType> &Map1, const TMap<KeyType, ValueType> &Map2)
	{
		if (Map1.Num() != Map2.Num())
		{
			return false;
		}

		for (const TPair<KeyType, ValueType>& Pair1 : Map1)
		{
			if (Map2.Contains(Pair1.Key) == false)
			{
				return false;
			}

			if (Map2[Pair1.Key] != Pair1.Value)
			{
				return false;
			}
		}
		return true;
	}
}

bool FHealthAndResistanceTestConstructors::RunTest(const FString &Parameters)
{
	const float BaseHealth = 50.f;
	TMap<FName, float> Resistances = {};
	Resistances.Emplace("Fire", 20);

	const bool bIsDead = true;

	// Default Object test
	{
		FHealthAndResistance DefaultHealthAndResistances = {};

		TestEqual(TEXT("CON_0: Default FHealthAndResistance healths should be equal"), DefaultHealthAndResistances.Health, DefaultHealth);
		TestEqual(TEXT("CON_0: Default FHealthAndResistance resistances should be equal"), DefaultHealthAndResistances.Resistances.Num(), DefaultResistances.Num());
		TestEqual(TEXT("CON_0: Default FHealthAndResistance bIsDead should be equal"), DefaultHealthAndResistances.bIsDead, DefaultbIsDead);
	}
	// Constructor FHealthAndResistance(float Health) test
	{
		FHealthAndResistance DefaultHealthAndResistances = {BaseHealth};

		TestEqual(TEXT("CON_1: FHealthAndResistance constructed with 50.f health should have 50.f"), DefaultHealthAndResistances.Health, 50.f);
		TestEqual(TEXT("CON_1: FHealthAndResistance resistances should be empty"), DefaultHealthAndResistances.Resistances.Num(), 0);
		TestEqual(TEXT("CON_1: Default FHealthAndResistance bIsDead should be equal"), DefaultHealthAndResistances.bIsDead, DefaultbIsDead);
	}
	// Constructor FHealthAndResistance(float Health, TMap<FName, float>Resistances) test
	{

		FHealthAndResistance DefaultHealthAndResistances = {BaseHealth, Resistances};

		TestEqual(TEXT("CON_2: FHealthAndResistance constructed with 50.f health should have 50.f"), DefaultHealthAndResistances.Health, BaseHealth);
		TestTrue(TEXT("CON_2: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, DefaultHealthAndResistances.Resistances));
		TestEqual(TEXT("CON_2: FHealthAndResistance constructed with resistances should have same number of resistances"), DefaultHealthAndResistances.Resistances.Num(), Resistances.Num());
		TestEqual(TEXT("CON_2: Default FHealthAndResistance bIsDead should be equal"), DefaultHealthAndResistances.bIsDead, DefaultbIsDead);
	}

	// Constructor FHealthAndResistance(float Health, TMap<FName, float>Resistances, bool bIsDead) test
	{
		FHealthAndResistance DefaultHealthAndResistances = {BaseHealth, Resistances, bIsDead};

		TestEqual(TEXT("CON_3: FHealthAndResistance constructed with 50.f health should have 50.f"), DefaultHealthAndResistances.Health, BaseHealth);
		TestTrue(TEXT("CON_2: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, DefaultHealthAndResistances.Resistances));

		TestEqual(TEXT("CON_3: FHealthAndResistance constructed with resistances should have same number of resistances"), DefaultHealthAndResistances.Resistances.Num(), Resistances.Num());
		TestEqual(TEXT("CON_3: FHealthAndResistance bIsDead should be equal to provided one"), DefaultHealthAndResistances.bIsDead, bIsDead);
	}

	return true;
}

bool FHealthAndResistanceTestHeal::RunTest(const FString &Parameters)
{
	// Over Heal Test
	{
		FHealthAndResistance DefaultHealthAndResistances = {};
		TestTrue(TEXT("Over Heal Test: DefaultHealthAndResistances should be default "), IsDefault(DefaultHealthAndResistances));
		DefaultHealthAndResistances.Heal(20);
		TestEqual(TEXT("Over Heal Test: Heal shouldn't overheal"), DefaultHealthAndResistances.Health, DefaultHealth);
	}
	// Negative heal
	{
		const float MinusHealth = -20.f;
		FHealthAndResistance DefaultHealthAndResistances = {};
		TestTrue(TEXT("Negative heal: At Start DefaultHealthAndResistances should be default "), IsDefault(DefaultHealthAndResistances));

		DefaultHealthAndResistances.Heal(MinusHealth);
		TestEqual(TEXT("Negative heal: Heal should not remove hp "), DefaultHealthAndResistances.Health, DefaultHealth);
		TestTrue(TEXT("Negative heal: Object after test should be as default"), IsDefault(DefaultHealthAndResistances));
	}
	// Healing form not full health
	{
		const float HealAmount = 20.f;
		const float StartHealth = 50.f;
		FHealthAndResistance HealthAndResistances = {StartHealth};

		HealthAndResistances.Heal(HealAmount);
		TestEqual(TEXT("Healing form not full health: Heal should Add HealAmount to Health"), HealthAndResistances.Health, StartHealth + HealAmount);
	}
	// Healing from 0 health
	{
		const float HealAmount = 100.f;
		const float StartHealth = 0.f;
		FHealthAndResistance HealthAndResistances = {StartHealth};

		HealthAndResistances.Heal(HealAmount);
		TestEqual(TEXT("Healing from 0 health: Heal should Add HealAmount to Health"), HealthAndResistances.Health, StartHealth + HealAmount);
		TestTrue(TEXT("Healing from 0 health: Object after test should be as default"), IsDefault(HealthAndResistances));
	}
	// Healing from dead
	{
		const float HealAmount = 100.f;
		const float StartHealth = 0.f;
		FHealthAndResistance HealthAndResistances = {StartHealth, DefaultResistances, true};

		HealthAndResistances.Heal(HealAmount);
		TestEqual(TEXT("Healing from dead: Healing should not work on dead characters "), HealthAndResistances.Health, StartHealth);
		TestTrue(TEXT("Healing from dead: Heal should not ressurect character"), HealthAndResistances.bIsDead);
	}

	return true;
}

bool FHealthAndResistanceTestDamageNoRes::RunTest(const FString &Parameters)
{
	// Damage lower then health
	{
		const auto DamageAmount = 50.f;
		const auto ExpectedValue = 50.f;
		FHealthAndResistance DefaultHealthAndResistances = {};
		TestTrue(TEXT("Damage lower then health: DefaultHealthAndResistances should be default at test start"), IsDefault(DefaultHealthAndResistances));

		DefaultHealthAndResistances.Damage("None", DamageAmount);
		TestEqual(TEXT("Damage lower then health: After taking damage healh should have Expected Value "), DefaultHealthAndResistances.Health, ExpectedValue);
		TestFalse(TEXT("Damage lower then health: If damage is not taking whole hp or more, character shouln't be dead "), DefaultHealthAndResistances.bIsDead);
	}
	// Damage Take whole health
	{
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 0.f;
		FHealthAndResistance DefaultHealthAndResistances = {};
		TestTrue(TEXT("Damage Take whole health: DefaultHealthAndResistances should be default at test start"), IsDefault(DefaultHealthAndResistances));

		DefaultHealthAndResistances.Damage("None", DamageAmount);
		TestEqual(TEXT("Damage Take whole health: After taking damage healh should have Expected Value "), DefaultHealthAndResistances.Health, ExpectedValue);
		TestTrue(TEXT("Damage Take whole health: Taking whole health should make character dead"), DefaultHealthAndResistances.bIsDead);
	}
	// Damage Take more then health
	{
		const auto DamageAmount = 120.f;
		const auto ExpectedValue = 0.f;
		FHealthAndResistance DefaultHealthAndResistances = {};
		TestTrue(TEXT("Damage Take whole health: DefaultHealthAndResistances should be default at test start"), IsDefault(DefaultHealthAndResistances));

		DefaultHealthAndResistances.Damage("None", DamageAmount);
		TestEqual(TEXT("Damage Take whole health: After taking damage healh should have Expected Value "), DefaultHealthAndResistances.Health, ExpectedValue);
		TestTrue(TEXT("Damage Take whole health: Taking whole health should make character dead"), DefaultHealthAndResistances.bIsDead);
	}

	return true;
}
bool FHealthAndResistanceTestDamageRes::RunTest(const FString &Parameters)
{

	// Damage With 20 resistance
	{
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 20.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 20);
		FHealthAndResistance HealthAndResistances = {DefaultHealth, Resistances};
		TestTrue(TEXT("Damage With 20 resistance: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Damage With 20 resistance: After taking damage health should have Expected Value "), HealthAndResistances.Health, ExpectedValue);
		TestFalse(TEXT("Damage With 20 resistance: If damage is not taking whole hp or more, character shouln't be dead "), HealthAndResistances.bIsDead);
	}
	// Damage With 100 resistance
	{
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 100.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 100);
		FHealthAndResistance HealthAndResistances = {DefaultHealth, Resistances};
		TestTrue(TEXT("Damage With 100 resistance: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Damage With 100 resistance: After taking damage health should have Expected Value "), HealthAndResistances.Health, ExpectedValue);
		TestFalse(TEXT("Damage With 100 resistance: If damage is not taking whole hp or more, character shouln't be dead "), HealthAndResistances.bIsDead);
	}
	// Damage With 120 resistance
	{
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 100.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 120);
		FHealthAndResistance HealthAndResistances = {DefaultHealth, Resistances};
		TestTrue(TEXT("Damage With 120 resistance: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Damage With 120 resistance: Damage with over 100 reistance should not overheal "), HealthAndResistances.Health, ExpectedValue);
		TestFalse(TEXT("Damage With 120 resistance: If damage is not taking whole hp or more, character shouln't be dead "), HealthAndResistances.bIsDead);
	}

	// Damage With 200 resistance and 50 health
	{
		const auto StartingHealth = 50.f;
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 100.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 200);
		FHealthAndResistance HealthAndResistances = {StartingHealth, Resistances};
		TestTrue(TEXT("Damage With 200 resistance and 50 health: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Damage With 200 resistance and 50 health: Damage with over 100 reistance should not overheal "), HealthAndResistances.Health, ExpectedValue);
		TestFalse(TEXT("Damage With 200 resistance and 50 health: If damage is not taking whole hp or more, character shouln't be dead "), HealthAndResistances.bIsDead);
	}

	// Damage With 200 resistance and dead
	{
		const auto StartingHealth = 0.f;
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 100.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 200);
		FHealthAndResistance HealthAndResistances = {StartingHealth, Resistances, true};
		TestTrue(TEXT("Damage With 200 resistance and dead: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Damage With 200 resistance and dead: Damage with over 100 resistance should not heal dead"), HealthAndResistances.Health, ExpectedValue);
		TestTrue(TEXT("Damage With 200 resistance and dead:  Damage with over 100 resistance should not revive"), HealthAndResistances.bIsDead);
	}
	// Damage With 10 resistance and 70 hp
	{
		const auto StartingHealth = 70.f;
		const auto DamageAmount = 100.f;
		const auto ExpectedValue = 0.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 10);
		FHealthAndResistance HealthAndResistances = {StartingHealth, Resistances};
		TestTrue(TEXT("Damage With 10 resistance and 70 hp: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Damage With 10 resistance and 70 hp:  Damage that takes more or equal should not take more then health"), HealthAndResistances.Health, ExpectedValue);
		TestTrue(TEXT("Damage With 10 resistance and 70 hp:  Damage that takes more or equal as hp should kill"), HealthAndResistances.bIsDead);
	}
	// Negative Damage
	{
		const auto DamageAmount = -100.f;
		TMap<FName, float> Resistances = {};
		Resistances.Emplace("Fire", 10);
		FHealthAndResistance HealthAndResistances = {DefaultHealth, Resistances};
		TestTrue(TEXT("Negative Damage: FHealthAndResistance Resistance should be same as one given in constructor "), AreMapEqual(Resistances, HealthAndResistances.Resistances));

		HealthAndResistances.Damage("Fire", DamageAmount);
		TestEqual(TEXT("Negative Damage:  Negative Damage should not heal or do damage"), HealthAndResistances.Health, DefaultHealth);
		TestFalse(TEXT("Negative Damage:  Negatie Damage should not kill"), HealthAndResistances.bIsDead);
	}

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS