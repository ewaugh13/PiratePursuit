#include "PiratePursuitCharacter.h"

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

	//isOnLadder = false;

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

void APiratePursuitCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (isOnLadder == false))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	else if ((Controller != NULL) && (Value != 0.0f) && (isOnLadder == true))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		//const FVector Direction = GetActorUpVector();
		AddMovementInput(Direction, Value);
	}
}

void APiratePursuitCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
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

	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &APiratePursuitCharacter::BeginOverlap);
	this->OnActorHit.AddUniqueDynamic(this, &APiratePursuitCharacter::OnHit);
	this->SetRespawnPlatform();

	// initalize water instance reference
	if (WaterInstance == nullptr)
	{
		TArray<AActor*> water;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWater::StaticClass(), water);
		WaterInstance = Cast<AWater>(water[0]);
	}

	TArray<AActor*> respawnPlatformsArray;
	UGameplayStatics::GetAllActorsWithTag(this->GetWorld(), this->RespawnTag, respawnPlatformsArray);
	respawnPlatformsArray.Sort(APiratePursuitCharacter::LowestPosition);

	for (AActor * platform : respawnPlatformsArray)
	{
		this->respawnPlatforms.push(platform);
	}
}

void APiratePursuitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->SetRespawnPlatform();
}

void APiratePursuitCharacter::Walk()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	//StopJumping();
}

void APiratePursuitCharacter::Climb()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void APiratePursuitCharacter::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		this->SetRespawnPlatform();
	}
}

void APiratePursuitCharacter::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this->WaterInstance)
	{
		FVector newPostion = this->GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), this->SplooshSound, newPostion);
		if (this->RespawnPlatform != nullptr &&
			this->RespawnPlatform->GetActorLocation().Z > this->WaterInstance->GetActorLocation().Z)
		{
			newPostion = this->RespawnPlatform->GetActorLocation() + (GetActorUpVector() * this->PlayerHeight);
		}
		else
		{
			AActor * newRespawnPlat = nullptr;
			while (newRespawnPlat == nullptr && !this->respawnPlatforms.empty())
			{
				AActor * possiblePlat = this->respawnPlatforms.top();
				// if its below the water and not high enough above remove it from stack
				if (possiblePlat->GetActorLocation().Z < this->WaterInstance->GetActorLocation().Z
					|| abs(possiblePlat->GetActorLocation().Z - this->WaterInstance->GetActorLocation().Z) < this->PlayerHeight * 0.5f)
				{
					this->respawnPlatforms.pop();
				}
				else
				{
					newRespawnPlat = possiblePlat;
					newPostion = newRespawnPlat->GetActorLocation() + (GetActorUpVector() * this->PlayerHeight);
				}
			}
		}
		this->SetActorLocation(newPostion);
	}
}

void APiratePursuitCharacter::SetRespawnPlatform()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "calling set respawn platform");
	FHitResult hitResult;
	UWorld* theWorld = this->GetWorld();
	FVector startVector = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z - this->PlayerHeight / 2);
	FVector endVector = startVector + (-1 * GetActorUpVector()) * this->PlayerHeight / 2;

	FCollisionQueryParams TraceParams(FName(TEXT("Trace")));

	theWorld->LineTraceSingleByObjectType(hitResult, startVector, endVector, ECC_WorldStatic, TraceParams);
	if (hitResult.GetActor() != nullptr && hitResult.GetActor()->ActorHasTag(this->RespawnTag)) //&& (this->RespawnPlatform == nullptr || hitResult.GetActor()->GetFName() == Actor->GetFName()))
	{
		this->RespawnPlatform = hitResult.GetActor();
	}
}
