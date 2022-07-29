// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BountyDashCharacter.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABountyDashCharacter();

	void ScoreUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Will handle moving the target location of the player left and right
	void MoveRight();
	void MoveLeft();

	// Overlap functions to be used upon capsule component collision
	UFUNCTION()
	void MyOnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MyOnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Camera boom for maintaining camera distance to the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// Camera we will use as the target view
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	// Player score
	UPROPERTY(BlueprintReadOnly)
	int32 Score;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Array of movement locations
	UPROPERTY(EditAnywhere, Category = "Logic")
	TArray<class ATargetPoint*> TargetArray;

	// Character lane swap speed
	UPROPERTY(EditAnywhere, Category = "Logic")
	float CharSpeed;

	// Audio component for obstacle hit sound
	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* hitObstacleSound;

	// Audio component for coin pickup sound
	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* dingSound;

private:
	// Data for character lane positioning
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;
};
