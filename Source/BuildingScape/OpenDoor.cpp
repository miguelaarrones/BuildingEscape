// Copyright Miguel A. Arrones 2020

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no PressurePlate set!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	} else if(GetWorld()->GetTimeSeconds() - DoorLastOpen >= DoorCloseDelay) {
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	//CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * 1.f);
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);

	FRotator OpenDoor = GetOwner()->GetActorRotation();
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);

	FRotator CloseDoor = GetOwner()->GetActorRotation();
	CloseDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(CloseDoor);
}

