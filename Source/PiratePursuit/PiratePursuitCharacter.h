#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <stack> 
#include "PiratePursuitCharacter.generated.h"

class APunchHitBox;
class AWater;
class UPlayerStunComponent;
class UTreasureHolderComponent;

UCLASS(config = Game)
class APiratePursuitCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent * CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent * FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UPlayerStunComponent * _StunComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UTreasureHolderComponent * _TreasureHolderComponent;

public:
	APiratePursuitCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnyWhere, Category = "PirateCharacter")
		uint32 m_PlayerHeight = 150;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PirateCharacter")
		FName m_RespawnTag = FName(TEXT("Respawn Platform"));

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "PirateCharacter")
		USoundBase * m_SplooshSound;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Punch();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent * GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent * GetFollowCamera() const { return FollowCamera; }
	// Returns Stun subobject
	FORCEINLINE class UPlayerStunComponent * GetStunComponent() const { return _StunComponent; }
	// Returns Treasure holder subobject
	FORCEINLINE class UTreasureHolderComponent * GetTreasureComponent() const { return _TreasureHolderComponent; }

	void Walk();

	void Climb();

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void DestoryHitBox(APunchHitBox * i_HitBox);
	UFUNCTION()
		void ReEnablePlayerInput();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PirateCharacter")
		// If the player is climbing on a vine wall
		bool m_IsClimbing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PirateCharacter")
		// If the player currently has the treasure
		bool m_HasTreasure = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PirateCharacter")
		// If the player is punching
		bool m_IsPunching = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PirateCharacter")
		// If the player is in a stunned state
		bool m_IsStunned = false;

private:

	FTimerHandle _DestroyHitBoxTimerHandle;
	FTimerHandle _ReEnablePlayerInputTimerHandle;

	void FindNextRespawnPlatform();

	UPROPERTY(VisibleAnywhere, Category = "PirateCharacter")
		AActor * _RespawnPlatform;
	UPROPERTY(VisibleAnywhere, Category = "PirateCharacter")
		AWater * _WaterInstance;

	std::stack<AActor *> _RespawnPlatforms;

	// Predicate used to sort array of respawn platforms with lower ones (Z axis) first
	inline static bool LowestPosition(const AActor& i_Actor1, const AActor& i_Actor2)
	{
		return (i_Actor1.GetActorLocation().Z > i_Actor2.GetActorLocation().Z);
	}
};

