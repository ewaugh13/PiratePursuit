#include "PunchHitBox.h"

#include "PiratePursuitCharacter.h"
#include "PlayerStunComponent.h"

#include "Components/BoxComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APunchHitBox::APunchHitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = m_HitBox;
	m_HitBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	m_HitBox->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	m_HitBox->SetCollisionProfileName(FName(TEXT("Trigger")));

	m_HitBox->OnComponentBeginOverlap.AddDynamic(this, &APunchHitBox::BeginOverlap);
}

void APunchHitBox::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != GetAttachParentActor())
	{
		APiratePursuitCharacter * pirateCharacter = Cast<APiratePursuitCharacter>(OtherActor);
		
		UPlayerStunComponent * stunComponent = pirateCharacter->GetStunComponent();
		if (!stunComponent->m_IsStunned && !stunComponent->m_IsInGracePeriod)
		{
			stunComponent->Stun();
			pirateCharacter->GetCharacterMovement()->AddForce(GetActorForwardVector() * m_KnockbackPower);
		}
	}
}
