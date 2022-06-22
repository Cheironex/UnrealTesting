// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomatedTests/Enemy/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

