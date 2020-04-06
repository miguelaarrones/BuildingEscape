// Copyright Miguel A. Arrones 2020

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {
	} else {
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent not found in %s"), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("InputController found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	} else {
		UE_LOG(LogTemp, Error, TEXT("InputController not found in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// Draw a Line from player showing the REACH
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner()
	);
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		PlayerViewPointLocation,
		LineTraceEnd, 
		// Collision Object Type
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *ActorHit->GetName());
	}
	
	// See what it hits
}


