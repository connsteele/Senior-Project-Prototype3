// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySlime.h"

// Constructor
AEnemySlime::AEnemySlime() : AEnemyClass()
{
	// Set the slime's vision radius
	visionSphere->SetSphereRadius(200.f);
	
}

//----- Override Functions from Parent Class
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

	//--- Stat Related Variable Initialization
	maxHealth = 40.f;
	currHealth = maxHealth;
	maxTurnAP = 100.f;
	turnAP = maxTurnAP;
	distanceToAP = 10.f; // Used as divisor to turn the length of distance moved into AP, higher = more per turn movement
	isMoveable = true; // Allow actor to move
	rewardExp = 10;
}

void AEnemySlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//----- Turn Related Functions
void AEnemySlime::endTurn()
{
	Super::endTurn();
	isTurn = false;
	// D("Slime Turn End");
}

void AEnemySlime::startTurn()
{
	// D("Slime Turn Start");
	isTurn = true;
	// isBattling = true;
	turnAP = maxTurnAP; // Reset AP for new turn
	lastRecordedLocation = GetActorLocation();
}


//----- Functions Bound For Collisions
// Called when the enemy slime's vision collider overlaps with a component
void AEnemySlime::inSight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//D("Slime hit");
	if ((Cast<AProtagClass>(OtherActor) != NULL) && (Cast<UCapsuleComponent>(OtherComp)) && (isBattling == false))
	{
		isBattling = true;
		AHSPGameGameModeBase* gameModeref = (AHSPGameGameModeBase*)GetWorld()->GetAuthGameMode();
		gameModeref->addToBattle(this, "Slime"); // maybe cast this to (ABase2DCharacter *)
		//gameModeref->aliveCharacters += 1;
		//D("Slime Enters Battle");
	}
}
// Called when the enemy slime's root component (capsule component) is hit
void AEnemySlime::charHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	//die();
	// GetWorld()->ForceGarbageCollection(true);
	//Destroy();
	// If you hit the player's body
	if ((isTurn == true) && (Cast<AProtagClass>(OtherActor) != NULL) && (Cast<UCapsuleComponent>(OtherComp)))
	{
		// D("Hit Player");
		AProtagClass * playerRef = Cast<AProtagClass>(OtherActor);
		playerRef->currHealth -= 10.0f;
		endTurn();
	}
}

