// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_UniversalWalk.h"
#include "BFL_Utility.h"

// Sets default values for this component's properties
UAC_UniversalWalk::UAC_UniversalWalk()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_UniversalWalk::BeginPlay()
{
	Super::BeginPlay();

	AController* controller = Cast<AController>(GetOwner());

	if (controller)
		ControlledCharacter = Cast<AIntensif3ATI_TESTCharacter>(controller->GetCharacter());	
}


// Called every frame
void UAC_UniversalWalk::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bWalkOnWalls) return;

	TArray<FScanResult> points = UBFL_Utility::Scan(GetWorld(), ControlledCharacter->GetMesh()->GetComponentLocation(), ControlledCharacter->GetActorQuat(), 5, 20, 3, 2, 54, 145, 4, true);

	if (points.Num() == 0) return;

	FVector normalAvg = FVector::ZeroVector;

	for (const FScanResult& point : points) {

		if (point.Normal.IsNearlyZero()) continue;

		normalAvg += point.Normal * point.Weight;
	}

	normalAvg /= points.Num();

	ControlledCharacter->GetCharacterMovement()->SetGravityDirection(-normalAvg);
}

void UAC_UniversalWalk::ActivateUniversalWalk()
{
	bWalkOnWalls = true;
}

void UAC_UniversalWalk::DeactivateUniversalWalk()
{
	bWalkOnWalls = false;
}
