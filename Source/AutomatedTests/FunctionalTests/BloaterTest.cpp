#include "Misc/AutomationTest.h"
#include "AutomatedTests/Enemy/HealthAndResistance.h"
#include "AutomatedTests/Enemy/Bloater.h"
#include "AutomatedTests/AutomatedTestsCharacter.h"
#include "Tests/AutomationCommon.h"

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FBloaterTestCharacter, FAutomationTestBase *, Test, AAutomatedTestsCharacter *, Character);
bool FBloaterTestCharacter::Update()
{
	FHealthAndResistance DefaultHealthAndResistance{};
	Test->TestEqual("Hero should have default hp", Character->GetHealthAndResistance().Health, DefaultHealthAndResistance.Health);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FBloaterTestDistance, FAutomationTestBase *, Test, AAutomatedTestsCharacter *, Character, ABloater *, Bloater);
bool FBloaterTestDistance::Update()
{
	const auto BloaterTiggerDistance = Bloater->BloatTriggerDistance;
	auto Distance = FVector::Dist(Character->GetActorLocation(), Bloater->GetActorLocation());
	UE_LOG(LogEngineAutomationTests, Log, TEXT("Bloat distance is %f and distance is %f"), BloaterTiggerDistance, Distance);

	Test->TestTrue(TEXT("Distance is greater then bloat distance"), BloaterTiggerDistance < Distance);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FBloaterTestNoExplosion, FAutomationTestBase *, Test, AAutomatedTestsCharacter *, Character, ABloater *, Bloater);
bool FBloaterTestNoExplosion::Update()
{
	FHealthAndResistance DefaultHealthAndResistance{};
	Bloater->Tick(1.f);
	Test->TestTrue(TEXT("Bloater is still valid"), IsValid(Bloater));
	Test->TestTrue(TEXT("Character Health is intact"), Character->GetHealthAndResistance().Health == DefaultHealthAndResistance.Health);

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FBloaterTestMove, FAutomationTestBase *, Test, AAutomatedTestsCharacter *, Character, ABloater *, Bloater);
bool FBloaterTestMove::Update()
{
	UE_LOG(LogEngineAutomationTests, Log, TEXT("Bloater Location Before %s"), *Bloater->GetActorLocation().ToCompactString());
	UE_LOG(LogEngineAutomationTests, Log, TEXT("Character Location Before %s"), *Character->GetActorLocation().ToCompactString());

	Bloater->SetActorLocation(FVector(300.f, 0.f, 0.f));
	const auto BloaterTiggerDistance = Bloater->BloatTriggerDistance;

	auto Distance = FVector::Dist(Character->GetActorLocation(), Bloater->GetActorLocation());
	UE_LOG(LogEngineAutomationTests, Log, TEXT("Bloater Location After %s"), *Bloater->GetActorLocation().ToCompactString());
	UE_LOG(LogEngineAutomationTests, Log, TEXT("Bloat distance is %f and distance is %f"), BloaterTiggerDistance, Distance);

	Test->TestTrue(TEXT("Distance is lower then bloat distance"), BloaterTiggerDistance > Distance);
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_THREE_PARAMETER(FBloaterTestExplosion, FAutomationTestBase *, Test, AAutomatedTestsCharacter *, Character, ABloater *, Bloater);
bool FBloaterTestExplosion::Update()
{
	Bloater->Tick(0.1f);
	const auto BloaterTiggerDistance = Bloater->BloatTriggerDistance;
	FHealthAndResistance DefaultHealthAndResistance{};
	UE_LOG(LogEngineAutomationTests, Log, TEXT("Character Health is %f"), Character->GetHealthAndResistance().Health);
	Test->TestTrue(TEXT("Character Health lower"), Character->GetHealthAndResistance().Health == (DefaultHealthAndResistance.Health - 50.f));
	Test->TestFalse(TEXT("Bloater is destroyed"), IsValid(Bloater));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBloaterTestLatent, "Project.FunctionalTests.Bloater.BloaterTestLatent", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
bool FBloaterTestLatent::RunTest(const FString &Parameters)
{
	UWorld *World = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext &WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	auto *Character = NewObject<AAutomatedTestsCharacter>(World->PersistentLevel);
	auto *Bloater = NewObject<ABloater>(World->PersistentLevel);
	Bloater->SetActorLocation(FVector(0.f, 0.f, 0.f));
	Character->SetActorLocation(FVector(600.f, 0.f, 0.f));

	ADD_LATENT_AUTOMATION_COMMAND(FBloaterTestCharacter(this, Character));
	ADD_LATENT_AUTOMATION_COMMAND(FBloaterTestDistance(this, Character, Bloater));
	ADD_LATENT_AUTOMATION_COMMAND(FBloaterTestNoExplosion(this, Character, Bloater));
	ADD_LATENT_AUTOMATION_COMMAND(FBloaterTestMove(this, Character, Bloater));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
	ADD_LATENT_AUTOMATION_COMMAND(FBloaterTestExplosion(this, Character, Bloater));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBloaterTest, "Project.FunctionalTests.Bloater.BloaterTest", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter);
bool FBloaterTest::RunTest(const FString &Parameters)
{
	UWorld *World = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext &WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	auto *Character = NewObject<AAutomatedTestsCharacter>(World->PersistentLevel);
	Character->SetActorLocation(FVector(0.f, 0.f, 0.f));
	auto *Bloater = NewObject<ABloater>(World->PersistentLevel);
	Bloater->SetActorLocation(FVector(600.f, 0.f, 0.f));

	const auto HealthAndResistance = Character->GetHealthAndResistance();
	const auto BloaterTiggerDistance = Bloater->BloatTriggerDistance;

	auto Distance = FVector::Dist(Character->GetActorLocation(), Bloater->GetActorLocation());
	TestTrue(TEXT("Distance is greater then bloat distance"), BloaterTiggerDistance < Distance);

	Bloater->Tick(1.f);

	TestTrue(TEXT("Bloater is still valid"), IsValid(Bloater));
	TestTrue(TEXT("Character Health is intact"), Character->GetHealthAndResistance().Health == HealthAndResistance.Health);
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));

	Bloater->SetActorLocation(FVector(300, 0, 0));

	Distance = FVector::Dist(Character->GetActorLocation(), Bloater->GetActorLocation());
	TestTrue(TEXT("Distance is lower then bloat distance"), BloaterTiggerDistance > Distance);
	Bloater->Tick(1.f);


	return true;
}
