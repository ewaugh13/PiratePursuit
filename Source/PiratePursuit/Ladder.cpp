
#include "Ladder.h"

#include "PiratePursuitCharacter.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ALadder::ALadder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	CollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnOverlapEnd);
}

void ALadder::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	APiratePursuitCharacter * pirateCharacter = Cast<APiratePursuitCharacter>(OtherActor);

	if (pirateCharacter != nullptr)
	{
		// only can climb if we aren't holding treasure
		if (!pirateCharacter->m_HasTreasure)
		{
			pirateCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			pirateCharacter->m_IsClimbing = true;
		}
	}
}

void ALadder::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	APiratePursuitCharacter * pirateCharacter = Cast<APiratePursuitCharacter>(OtherActor);

	if (pirateCharacter != nullptr)
	{
		pirateCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		pirateCharacter->m_IsClimbing = false;
	}
}

