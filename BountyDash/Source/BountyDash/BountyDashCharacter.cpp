// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashCharacter.h"
#include "BountyDash.h"
#include "EngineUtils.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TargetPoint.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "BountyDashGameModeBase.h"

// Sets default values
ABountyDashCharacter::ABountyDashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> myAnimBP(TEXT("/Game/Barrel_Hopper/Character/Animation/BH_Character_AnimBP.BH_Character_AnimBP"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> myMesh(TEXT("/Game/Barrel_Hopper/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (myMesh.Succeeded() && myAnimBP.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnimBP.Object->GeneratedClass);
	}

	// Rotate and position the mesh so it sits in the capsule component properly
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Configure character movement
	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check(CameraBoom);

	CameraBoom->AttachTo(RootComponent);

	// The camera follows at this distance behind the character
	CameraBoom->TargetArmLength = 500.0f;

	// Offset to player
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	check(FollowCamera);

	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// Rotational change to make the camera look down slightly
	FollowCamera->AddRelativeRotation(FQuat(FRotator(-10.0f, 0.0f, 0.0f)));

	// Game Properties
	CharSpeed = 10.0f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABountyDashCharacter::MyOnComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABountyDashCharacter::MyOnComponentEndOverlap);

	// Poses the input at ID 0 (the default controller)
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABountyDashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		TargetArray.Add(*TargetIter);
	}

	auto SortPred = [](const AActor& A, const AActor& B)->bool
	{
		return (A.GetActorLocation().Y < B.GetActorLocation().Y);
	};

	TargetArray.Sort(SortPred);

	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);
}

// Called every frame
void ABountyDashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetArray.Num() > 0)
	{
		FVector targetLoc = TargetArray[CurrentLocation]->GetActorLocation();
		targetLoc.Z = GetActorLocation().Z;
		targetLoc.X = GetActorLocation().X;

		if (targetLoc != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), targetLoc, CharSpeed * DeltaTime));
		}
	}
}

// Called to bind functionality to input
void ABountyDashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("MoveRight", IE_Pressed, this, &ABountyDashCharacter::MoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABountyDashCharacter::MoveLeft);
}

void ABountyDashCharacter::MoveRight()
{
	if (Controller != nullptr)
	{
		if (CurrentLocation < TargetArray.Num() - 1)
		{
			++CurrentLocation;
		}
	}
}

void ABountyDashCharacter::MoveLeft()
{
	if (Controller != nullptr)
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
	}
}

void ABountyDashCharacter::MyOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABountyDashCharacter::MyOnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABountyDashCharacter::ScoreUp()
{

}