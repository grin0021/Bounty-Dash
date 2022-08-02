// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinSpawner.h"
#include "BountyDash.h"
#include "Coin.h"
#include "Floor.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ACoinSpawner::ACoinSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MaxSetCoins = 5;
	MinSetCoins = 3;
	CoinSetTimeInterval = 4.0f;
	CoinTimeInterval = 0.5f;
	MovementTimeInterval = 1.0f;
}

// Called when the game starts or when spawned
void ACoinSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		SpawnTransforms.Add(*TargetIter);
	}

	for (TActorIterator<AFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}

	FTimerManager& worldTimerManager = GetWorld()->GetTimerManager();

	worldTimerManager.SetTimer(CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
	worldTimerManager.SetTimer(SpawnMoveTimerHandle, this, &ACoinSpawner::MoveSpawner, MovementTimeInterval, true);
}

void ACoinSpawner::SpawnCoin()
{
	FActorSpawnParameters spawnParams;

	FTransform coinTransform = SpawnTransforms[TargetLoc]->GetTransform();
	coinTransform.SetLocation(FVector(SpawnPoint, coinTransform.GetLocation().Y, coinTransform.GetLocation().Z));

	ACoin* spawnedCoin = GetWorld()->SpawnActor<ACoin>(CoinObject, coinTransform, spawnParams);

	if (spawnedCoin)
	{
		USphereComponent* coinSphere = Cast<USphereComponent>(spawnedCoin->GetComponentByClass(USphereComponent::StaticClass()));

		if (coinSphere)
		{
			float offset = coinSphere->GetUnscaledSphereRadius();

			spawnedCoin->AddActorLocalOffset(FVector(0.0f, 0.0f, offset));
		}

		NumCoinsToSpawn--;
	}

	if (NumCoinsToSpawn <= 0)
	{
		FTimerManager& worldTimerManager = GetWorld()->GetTimerManager();
		worldTimerManager.SetTimer(CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
		worldTimerManager.ClearTimer(CoinTimerHandle);
	}
}

void ACoinSpawner::SpawnCoinSet()
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);

	FTimerManager& worldTimerManager = GetWorld()->GetTimerManager();

	// Swap active timers
	worldTimerManager.SetTimer(CoinTimerHandle, this, &ACoinSpawner::SpawnCoin, CoinTimeInterval, true);
}

void ACoinSpawner::MoveSpawner()
{
	TargetLoc = FMath::Rand() % SpawnTransforms.Num();
}