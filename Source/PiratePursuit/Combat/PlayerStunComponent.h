#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStunComponent.generated.h"

class USoundBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PIRATEPURSUIT_API UPlayerStunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStunComponent();

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stun")
		// Sound to play when stunning a player
		USoundBase * m_PunchSound;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stun")
		// Time player is stunned
		float m_StunTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stun")
		// Grace period with getting stunned
		float m_GracePeriod = 2.0f;

	UFUNCTION()
		void Stun();
	UFUNCTION()
		void Unstun();
	UFUNCTION()
		void StartGracePeriod();
	UFUNCTION()
		void EndGracePeriod();

private:

	void BindDelegates();

	FTimerDynamicDelegate StunDelegate;
	FTimerDynamicDelegate UnstunDelegate;
	FTimerDynamicDelegate StartGracePeriodDelegate;
	FTimerDynamicDelegate EndGracePeriodDelegate;

	UPROPERTY(VisibleAnywhere, Category = "Stun")
		// The players original max speed
		float _OriginalMaxSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Stun")
		// Time player is stunned
		bool _IsStunned = false;

	UPROPERTY(VisibleAnywhere, Category = "Stun")
		// If player is in grace period
		bool _IsInGracePeriod = false;

};
