#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TreasureHolderComponent.generated.h"

class ATreasure;

class UCharacterMovementComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PIRATEPURSUIT_API UTreasureHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UTreasureHolderComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "TreasureHolderComponent")
		void PickupTreasure(ATreasure * i_NewTreasure);

	UFUNCTION(BlueprintCallable, Category = "TreasureHolderComponent")
		void LoseTreasure();

	UFUNCTION(BlueprintPure, Category = "TreasureHolderComponent")
		ATreasure * GetHeldTreasure() const { return _HeldTreasure; }

	UFUNCTION(BlueprintPure, Category = "TreasureHolderComponent")
		bool InCooldownState() const { return _CurrentCooldown > 0; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreasureHolderComponent")
		// Amount of time for cooldown between picking up treasure
		float m_CooldownTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreasureHolderComponent")
		// Multiplier to affect characters speed when picking up treasure
		float m_SpeedMultiplier = 0.85f;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "TreasureHolderComponent")
		// Reference to treasure being held
		ATreasure * _HeldTreasure;

	UPROPERTY(VisibleAnywhere, Category = "TreasureHolderComponent")
		// Cooldown to pick up treasure
		float _CurrentCooldown = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "TreasureHolderComponent")
		// Cooldown to pick up treasure
		float _OriginalMaxSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, Category = "TreasureHolderComponent")
		// Owning actors movement component
		UCharacterMovementComponent * _OwnerMovement;

};
