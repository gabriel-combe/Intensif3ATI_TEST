// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AC_UniversalWalk.h"
#include "Intensif3ATI_TESTCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Intensif3ATI_TESTPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UCharacterMovementComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AIntensif3ATI_TESTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AIntensif3ATI_TESTPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** Can the character walk on the walls*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bWalkOnWalls = true;

	/** Acceptance radius to reach a location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AcceptanceRadius;

	/** Threshold in second where we consider being stuck */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ThresholdStuck;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	/** Activate the UniversalWalkComp */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ActivateUniversalWalkComp();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

private:
	FVector CachedDestination;

	AIntensif3ATI_TESTCharacter* ControlledCharacter;

	bool bReachedLocation;
	float PrevDist;
	float StuckCounter;

	TObjectPtr<UAC_UniversalWalk> UniversalWalkComp;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


