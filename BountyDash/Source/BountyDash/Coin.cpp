// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "BountyDash.h"
#include "BountyDashCharacter.h"
#include "BountyDashGameModeBase.h"
#include "Components/SphereComponent.h"

ACoin::ACoin()
{

}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddLocalRotation(FRotator(5.0f, 0.0f, 0.0f));
}

void ACoin::MyOnActorOverlap(AActor* Actor, AActor* OtherActor)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		USphereComponent* otherSphere = Cast<USphereComponent>(OtherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere)
		{
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius() * 2.0f) + Collider->GetUnscaledSphereRadius()));
		}
	}
}