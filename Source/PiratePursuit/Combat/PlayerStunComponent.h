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

	UPROPERTY(VisibleAnywhere, Category = "Stun")
		// Time player is stunned
		bool m_IsStunned = false;

	UPROPERTY(VisibleAnywhere, Category = "Stun")
		// If player is in grace period
		bool m_IsInGracePeriod = false;

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

	FTimerDynamicDelegate _StunDelegate;
	FTimerDynamicDelegate _UnstunDelegate;
	FTimerDynamicDelegate _StartGracePeriodDelegate;
	FTimerDynamicDelegate _EndGracePeriodDelegate;

	UPROPERTY(VisibleAnywhere, Category = "Stun")
		// The players original max speed
		float _OriginalMaxSpeed;
};
