// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"
#include "PiratePursuitCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

void ABarrel::BeginPlay()
{
	Super::BeginPlay();

	this->FloatComponent = this->FindComponentByClass<UFloatWater>();

	UStaticMeshComponent * staticMeshComp = this->GetStaticMeshComponent();
	TArray<USceneComponent *> childrenComponents;
	staticMeshComp->GetChildrenComponents(true, childrenComponents);
	uint32 numChildren = staticMeshComp->GetNumChildrenComponents();

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

	this->OnActorHit.AddUniqueDynamic(this, &ABarrel::OnHit);
}

void ABarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABarrel::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<APiratePursuitCharacter>(OtherActor))
	{
		this->FloatComponent->SetActorOnTop(true);
	}
}

void ABarrel::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APiratePursuitCharacter>(OtherActor))
	{
		this->FloatComponent->SetActorOnTop(false);
		this->FloatComponent->SetQuickRise(true);
	}
}

void ABarrel::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		
	}
}
