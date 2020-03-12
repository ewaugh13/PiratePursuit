#include "Barrel.h"

#include "FloatWater.h"
#include "PiratePursuitCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

void ABarrel::BeginPlay()
{
	Super::BeginPlay();

	_FloatComponent = FindComponentByClass<UFloatWater>();

	TArray<USceneComponent *> childrenComponents;
	GetStaticMeshComponent()->GetChildrenComponents(true, childrenComponents);

	UCapsuleComponent * capsule = nullptr;
	for (USceneComponent * component : childrenComponents)
	{
		capsule = Cast<UCapsuleComponent>(component);
		if (capsule != nullptr)
		{
			capsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABarrel::BeginOverlap);
			capsule->OnComponentEndOverlap.AddUniqueDynamic(this, &ABarrel::OnOverlapEnd);
			break;
		}
	}
}

void ABarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABarrel::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<APiratePursuitCharacter>(OtherActor))
	{
		_FloatComponent->SetActorOnTop(true);
	}
}

void ABarrel::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APiratePursuitCharacter>(OtherActor))
	{
		_FloatComponent->SetActorOnTop(false);
		_FloatComponent->SetQuickRise(true);
	}
}
