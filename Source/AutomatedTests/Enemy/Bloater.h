// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Bloater.generated.h"

UCLASS()
class AUTOMATEDTESTS_API ABloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloater();

	float BloatDistance = 800.f;
	UPROPERTY(BlueprintReadWrite)
	float BloatTriggerDistance = 500.f;
	UPROPERTY(BlueprintReadWrite)
	float BloatDamage = 50.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	USceneComponent* ScaneComponent;

	void Bloat(TArray<AActor*> ActorsInBloatRadius);
	
};
