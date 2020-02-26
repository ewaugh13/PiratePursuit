// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "WaterInstanceData.h"

#include "GameFramework/Character.h"
//#include "GameFramework/GameModeBase.h"

#include <stack> 

#include "PiratePursuitCharacter.generated.h"

UCLASS(config = Game)
class APiratePursuitCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	APiratePursuitCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	bool isOnLadder;

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void Walk();

	void Climb();

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

private:
	UPROPERTY(EditAnyWhere)
		uint32 PlayerHeight = 150;
	UPROPERTY(EditAnyWhere)
		FName RespawnTag = FName(TEXT("Respawn Platform"));
	UPROPERTY(EditAnyWhere)
		USoundBase * SplooshSound = nullptr;
	void SetRespawnPlatform();
	AActor * RespawnPlatform;
	AActor * WaterInstance;
	std::stack<AActor *> respawnPlatforms;

	inline static bool LowestPosition(const AActor& i_Actor1, const AActor& i_Actor2)
	{
		return (i_Actor1.GetActorLocation().Z > i_Actor2.GetActorLocation().Z);
	}
};

