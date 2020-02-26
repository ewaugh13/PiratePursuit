// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TreasureHolderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIRATEPURSUIT_API UTreasureHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTreasureHolderComponent();

	UFUNCTION(BlueprintCallable, Category = "TreasureHolderComponent")
	void GetTreasure(AActor* treasure);

	UFUNCTION(BlueprintCallable, Category = "TreasureHolderComponent")
	void LoseTreasure();

	UFUNCTION(BlueprintPure, Category = "TreasureHolderComponent")
	bool hasTreasure() const { return (bool)treasure; }

	UFUNCTION(BlueprintPure, Category = "TreasureHolderComponent")
	bool canGrabTreasure() const { return currentCooldown >= cooldownTime; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float currentCooldown = 0.f;

	float originalMaxSpeed = 300.f;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* treasure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreasureHolderComponent")
	float cooldownTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreasureHolderComponent")
	float speedMultiplier = .85f;
};
