// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinSpawner.generated.h"

UCLASS()
class BOUNTYDASH_API ACoinSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoinSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnCoin();
	void SpawnCoinSet();
	void MoveSpawner();

	int32 NumCoinsToSpawn;
	int32 TargetLoc;

	float KillPoint;
	float SpawnPoint;

	FTimerHandle CoinSetTimerHandle;
	FTimerHandle CoinTimerHandle;
	FTimerHandle SpawnMoveTimerHandle;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACoin> CoinObject;

	UPROPERTY()
	TArray<class ATargetPoint*> SpawnTransforms;

	UPROPERTY()
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	int32 MinSetCoins;

	UPROPERTY(EditAnywhere)
	int32 MaxSetCoins;

	UPROPERTY(EditAnywhere)
	float CoinSetTimeInterval;

	UPROPERTY(EditAnywhere)
	float CoinTimeInterval;

	UPROPERTY(EditAnywhere)
	float MovementTimeInterval;
};
