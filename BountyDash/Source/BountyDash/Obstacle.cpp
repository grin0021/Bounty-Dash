// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "BountyDash.h"
#include "BountyDashGameModeBase.h"
#include "Components/SphereComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	check(Collider);

	RootComponent = Collider;
	Collider->SetCollisionProfileName("OverlapAllDynamic");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);

	Mesh->AttachTo(Collider);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	KillPoint = -20000.0f;

	OnActorBeginOverlap.AddDynamic(this, &AObstacle::MyOnActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AObstacle::MyOnActorEndOverlap);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float gameSpeed = GetCustomGameMode<ABountyDashGameModeBase>(GetWorld())->GetInvGameSpeed();

	AddActorLocalOffset(FVector(gameSpeed, 0.0f, 0.0f));

	if (GetActorLocation().X < KillPoint)
	{
		Destroy();
	}
}

void AObstacle::MyOnActorBeginOverlap(AActor* Actor, AActor* OtherActor)
{

}

void AObstacle::MyOnActorEndOverlap(AActor* Actor, AActor* OtherActor)
{

}

void AObstacle::SetKillPoint(float point)
{
	KillPoint = point;
}

float AObstacle::GetKillPoint()
{
	return KillPoint;
}