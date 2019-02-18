// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySlime.h"

AEnemySlime::AEnemySlime() : AEnemyClass()
{
	// Set the slime's vision radius
	visionSphere->SetSphereRadius(200.f);
	//Setup vision hit


	
}

void AEnemySlime::BeginPlay()
{
	Super::BeginPlay(); // Need to do this or nothing will work

	//--- Event Setup
	GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &AEnemySlime::charHit);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemySlime::charHit);
	visionSphere->SetGenerateOverlapEvents(true);
	visionSphere->SetNotifyRigidBodyCollision(true);
	visionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AEnemySlime::inSight);
	visionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemySlime::inSight);

	// Allow actor to move
	isMoveable = true;
}

void AEnemySlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEnemySlime::endTurn()
{
	Super::endTurn();
	isTurn = false;
}

void AEnemySlime::startTurn()
{
	D("ENEMY SLIME TURN START!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	isTurn = true;
	isBattling = true;
}


//--- Functions Bound Using AddDynamic
void AEnemySlime::inSight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	D("Slime hit");
	if ((Cast<AProtagClass>(OtherActor) != NULL) && (Cast<UCapsuleComponent>(OtherComp)) && (isBattling == false))
	{
		isBattling = true;
		AHSPGameGameModeBase* gameModeref = (AHSPGameGameModeBase*)GetWorld()->GetAuthGameMode();
		gameModeref->addToBattle(this, "Slime"); // maybe cast this to (ABase2DCharacter *)
		D("Slime Enters Battle");
	}
}

void AEnemySlime::charHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	//die();
	// GetWorld()->ForceGarbageCollection(true);
	//Destroy();
	// If you hit the player's body
	if ((isTurn == true) && (Cast<AProtagClass>(OtherActor) != NULL) && (Cast<UCapsuleComponent>(OtherComp)))
	{
		D("Hit Player");
		AProtagClass * playerRef = Cast<AProtagClass>(OtherActor);
		playerRef->currHealth -= 5.0f;
		endTurn();
	}
}

