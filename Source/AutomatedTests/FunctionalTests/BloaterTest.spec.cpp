#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "AutomatedTests/Enemy/HealthAndResistance.h"
#include "AutomatedTests/Enemy/Bloater.h"
#include "AutomatedTests/AutomatedTestsCharacter.h"

BEGIN_DEFINE_SPEC(BloaterSpec, "Project.FunctionalTests.Bloater.BloaterTestSpecs", EAutomationTestFlags::EngineFilter | EAutomationTestFlags::ApplicationContextMask)
ABloater *Bloater;
AAutomatedTestsCharacter *Character;
UWorld *World;
END_DEFINE_SPEC(BloaterSpec)
void BloaterSpec::Define()
{
	BeforeEach([this]()
	{
		World = UWorld::CreateWorld(EWorldType::Game, false);
		FWorldContext &WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
		WorldContext.SetCurrentWorld(World);
		Bloater = NewObject<ABloater>(World->PersistentLevel);
		Character = NewObject<AAutomatedTestsCharacter>(World->PersistentLevel);
		Bloater->SetActorLocation(FVector::ZeroVector);
		Character->SetActorLocation(FVector(600.f, 0.f, 0.f));
	});

	Describe("Basic exlosion", [this]()
	{
		It("actors should be valid", [this]()
		{
			TestTrue("Bloater is valid", IsValid(Bloater));
			TestTrue("Character is valid", IsValid(Character)); 
		});
		It("Character should have default health", [this]()
		   {
			FHealthAndResistance DefaultHealthAndResistance{};
			TestEqual("Hero should have default hp", Character->GetHealthAndResistance().Health, DefaultHealthAndResistance.Health); });
		It("Bloater should not explode if distance is greater then ", [this]()
		   {
			const auto BloaterTiggerDistance = Bloater->BloatTriggerDistance;
			auto Distance = FVector::Dist(Character->GetActorLocation(), Bloater->GetActorLocation());
			TestTrue(TEXT("Distance is greater then bloat distance"), BloaterTiggerDistance < Distance);

			FHealthAndResistance DefaultHealthAndResistance{};
			Bloater->Tick(1.f);
			TestTrue(TEXT("Bloater is still valid"), IsValid(Bloater));
			TestTrue(TEXT("Character Health is intact"), Character->GetHealthAndResistance().Health == DefaultHealthAndResistance.Health); });

		
	
		It("Bloater should explode if distance is lower then BloatTriggerDistance", [this]()
		   {
			Bloater->SetActorLocation(FVector(300.f, 0.f, 0.f));

			const auto BloaterTiggerDistance = Bloater->BloatTriggerDistance;
			auto Distance = FVector::Dist(Character->GetActorLocation(), Bloater->GetActorLocation());
			TestTrue(TEXT("Distance is lower then bloat distance"), BloaterTiggerDistance > Distance);

			Bloater->Tick(1.f);
			FHealthAndResistance DefaultHealthAndResistance{};
			TestTrue(TEXT("Character Health lower"), Character->GetHealthAndResistance().Health == (DefaultHealthAndResistance.Health - 50.f));
			TestFalse(TEXT("Bloater is destroyed"), IsValid(Bloater)); }); 
			   
	});
}