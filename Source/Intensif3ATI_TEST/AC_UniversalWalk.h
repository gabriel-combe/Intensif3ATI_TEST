// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "AC_UniversalWalk.generated.h"

class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTENSIF3ATI_TEST_API UAC_UniversalWalk : public UActorComponent
{
	GENERATED_BODY()

private:
	TObjectPtr<ACharacter> ControlledCharacter;

	bool bWalkOnWalls = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UAC_UniversalWalk();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActivateUniversalWalk();

	void DeactivateUniversalWalk();
};
