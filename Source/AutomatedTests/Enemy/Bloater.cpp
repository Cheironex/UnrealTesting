// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomatedTests/Enemy/Bloater.h"
#include "Kismet/GameplayStatics.h"
#include "AutomatedTests/AutomatedTestsCharacter.h"
#include "AutomatedTests/Enemy/HealthComponent.h"

// Sets default values
ABloater::ABloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->ScaneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	this->RootComponent = this->ScaneComponent;
}

// Called when the game starts or when spawned
void ABloater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAutomatedTestsCharacter::StaticClass(), Actors);
	
	TArray<AActor*> CloseActors;
	const auto ActorLocation = this->GetActorLocation();
	bool bShouldBloat = false;
	for (auto* Actor: Actors)
	{
		if(IsValid(Actor) == false)
		{
			continue;
		}
		const auto DistanceSq = FVector::DistSquared(Actor->GetActorLocation(), ActorLocation);
		if(DistanceSq < (this->BloatDistance * this->BloatDistance))
		{
			CloseActors.Add(Actor);
		}
		if(DistanceSq < (this->BloatTriggerDistance * this->BloatTriggerDistance))
		{
			bShouldBloat = true;
		}
	}

	if(bShouldBloat)
	{
		Bloat(CloseActors);
	}

}

void ABloater::Bloat(TArray<AActor*> ActorsInBloatRadius)
{
	for(auto* Actor : ActorsInBloatRadius)
	{
		if(IsValid(Actor) == false)
		{
			continue;
		}
		auto* Character = Cast<AAutomatedTestsCharacter>(Actor);
		if(IsValid(Character) == false)
		{
			continue;
		}

		auto* Component = Character->GetComponentByClass(UHealthComponent::StaticClass());
		if(IsValid(Component))
		{
			auto * HealthComponent =  Cast<UHealthComponent>(Component);
			if(IsValid(HealthComponent))
			{
				HealthComponent->ApplyDamage("Chemical", 50.f);
			}
		}
	}

	this->Destroy();
}
