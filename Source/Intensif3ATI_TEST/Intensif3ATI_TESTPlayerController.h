// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Insect_DetourCrowdAIController.h"
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

private:
	/** Destination to reach */
	FVector CachedDestination;

	/** Actor hit during a click */
	TObjectPtr<AActor> HitActor;

	/** Is the blob possessing insects */
	UPROPERTY(VisibleAnywhere, Category = "Blob")
	bool IsPossessing;

	/** Player Character controlled */
	AIntensif3ATI_TESTCharacter* ControlledCharacter;

	/** Auto travel utility */
	bool bReachedLocation;
	float PrevDist;
	float StuckCounter;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	/** Body Part Recovered */
	UPROPERTY(VisibleAnywhere, Category = "Blob")
	int BodyPartPoint;

	/** Array of insect */
	UPROPERTY(VisibleAnywhere, Category = "Blob")
	TArray<AInsect_DetourCrowdAIController*> InsectCrowd;

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

public:
	AIntensif3ATI_TESTPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

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

	/** Returns Body Part number **/
	FORCEINLINE int GetBodyPart() const { return BodyPartPoint; }
	/** Returns number of insects **/
	FORCEINLINE int GetCrowdSize() const { return InsectCrowd.Num(); }

	/** Add a body part **/
	UFUNCTION(BlueprintCallable, Category = "Blob")
	void AddBodyPart() { BodyPartPoint++; }

	/** Add an insect to the crowd **/
	UFUNCTION(BlueprintCallable, Category = "Blob")
	bool AddInsectToCrowd(AInsect_DetourCrowdAIController* insect);

	/** Remove all insect from the crowd **/
	UFUNCTION(BlueprintCallable, Category = "Blob")
	void RemoveAllInsectFromCrowd();
};


