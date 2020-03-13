#include "PiratePursuitCharacter.h"

#include "Combat/PunchHitBox.h"
#include "Combat/PlayerStunComponent.h"
#include "Treasure/TreasureHolderComponent.h"
#include "Water.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// APiratePursuitCharacter

APiratePursuitCharacter::APiratePursuitCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	_StunComponent = CreateDefaultSubobject<UPlayerStunComponent>(TEXT("Stun"));
	
	_TreasureHolderComponent = CreateDefaultSubobject<UTreasureHolderComponent>(TEXT("TreasureHolder"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void APiratePursuitCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APiratePursuitCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APiratePursuitCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APiratePursuitCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APiratePursuitCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &APiratePursuitCharacter::Punch);
}

void APiratePursuitCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APiratePursuitCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APiratePursuitCharacter::Punch()
{
	if (!_StunComponent->m_IsStunned && !m_IsPunching)
	{
		// spawn hit box
		FActorSpawnParameters spawnHitBoxParams;
		spawnHitBoxParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnHitBoxParams.Instigator = this;
		FTransform spawnTransform = FTransform();
		spawnTransform.SetLocation(FVector(100.0f, 0.0, 0.0f));
		APunchHitBox * hitbox = GetWorld()->SpawnActor<APunchHitBox>(GetClass(), spawnTransform, spawnHitBoxParams);

		m_IsPunching = true;
		hitbox->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		GetWorld()->GetTimerManager().SetTimer(_DestroyHitBoxTimerHandle, FTimerDelegate::CreateUObject(this, &APiratePursuitCharacter::DestoryHitBox, hitbox), 0.2f, false);
	}
}

void APiratePursuitCharacter::MoveForward(float Value)
{
	// if we are climbing move in the upwards direction
	if (m_IsClimbing)
	{
		AddMovementInput(GetActorUpVector(), Value);
	}
	else
	{
		FRotator contolRot = GetControlRotation();
		
		// Zero our pitch and roll, only move on plane
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, contolRot.Yaw, 0.0f)), Value);
	}
}

void APiratePursuitCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APiratePursuitCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Disable player input for 3 seconds
	DisableInput(nullptr);
	GetWorld()->GetTimerManager().SetTimer(_ReEnablePlayerInputTimerHandle, this, &APiratePursuitCharacter::ReEnablePlayerInput, 3.0f, false);

	// add hit and overlap bindings
	GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &APiratePursuitCharacter::BeginOverlap);
	OnActorHit.AddUniqueDynamic(this, &APiratePursuitCharacter::OnHit);
	FindNextRespawnPlatform();

	// initalize water instance reference
	if (_WaterInstance == nullptr)
	{
		TArray<AActor*> water;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWater::StaticClass(), water);
		_WaterInstance = Cast<AWater>(water[0]);
	}

	// get all actors with respawn tag and sort them based on their height (lowest first)
	TArray<AActor*> respawnPlatformsArray;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), m_RespawnTag, respawnPlatformsArray);
	respawnPlatformsArray.Sort(APiratePursuitCharacter::LowestPosition);

	// add all respawn platforms to stack, lower ones will be higher on the stack
	for (AActor * platform : respawnPlatformsArray)
	{
		_RespawnPlatforms.push(platform);
	}
}

void APiratePursuitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindNextRespawnPlatform();
	m_HasTreasure = _TreasureHolderComponent->GetHeldTreasure() != nullptr;
	if (IsValid(_StunComponent))
	{
		m_IsStunned = _StunComponent->m_IsStunned;
	}
}

void APiratePursuitCharacter::Walk()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void APiratePursuitCharacter::Climb()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void APiratePursuitCharacter::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	// when we hit an actor update current respawn platform
	if (OtherActor != nullptr && OtherActor != this)
	{
		FindNextRespawnPlatform();
	}
}

void APiratePursuitCharacter::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// if we are overlap with the water
	if (OtherActor == _WaterInstance)
	{
		FVector newPostion = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_SplooshSound, newPostion);
		
		// if the current respawn platform is above the water level
		if (_RespawnPlatform != nullptr &&
			_RespawnPlatform->GetActorLocation().Z > _WaterInstance->GetActorLocation().Z)
		{
			newPostion = _RespawnPlatform->GetActorLocation() + (GetActorUpVector() * m_PlayerHeight);
		}
		// find a new respawn platform
		else
		{
			AActor * newRespawnPlat = nullptr;
			// iterate until we find next available respawn platform
			while (newRespawnPlat == nullptr && !_RespawnPlatforms.empty())
			{
				AActor * possiblePlat = _RespawnPlatforms.top();
				// if its below the water and not high enough above remove it from stack
				if (possiblePlat->GetActorLocation().Z < _WaterInstance->GetActorLocation().Z
					|| abs(possiblePlat->GetActorLocation().Z - _WaterInstance->GetActorLocation().Z) < m_PlayerHeight * 0.5f)
				{
					_RespawnPlatforms.pop();
				}
				else
				{
					newRespawnPlat = possiblePlat;
					newPostion = newRespawnPlat->GetActorLocation() + (GetActorUpVector() * m_PlayerHeight);
				}
			}
		}
		SetActorLocation(newPostion);
	}
}

void APiratePursuitCharacter::DestoryHitBox(APunchHitBox * i_HitBox)
{
	i_HitBox->Destroy();
	m_IsPunching = false;
}

void APiratePursuitCharacter::ReEnablePlayerInput()
{
	EnableInput(nullptr);
}

void APiratePursuitCharacter::FindNextRespawnPlatform()
{
	FHitResult hitResult;
	// set up vectors for line trace
	FVector startVector = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - m_PlayerHeight / 2);
	FVector endVector = startVector + (-1 * GetActorUpVector()) * m_PlayerHeight / 2;

	FCollisionQueryParams TraceParams(FName(TEXT("Trace")));

	GetWorld()->LineTraceSingleByObjectType(hitResult, startVector, endVector, ECC_WorldStatic, TraceParams);
	// if we hit another actor that has the respawn tag
	if (hitResult.GetActor() != nullptr && hitResult.GetActor()->ActorHasTag(m_RespawnTag))
	{
		_RespawnPlatform = hitResult.GetActor();
	}
}
