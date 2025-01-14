// Copyright Epic Games, Inc. All Rights Reserved.

#include "Intensif3ATI_TESTPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Intensif3ATI_TESTCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Intensif3ATI_TESTInsect.h"
#include "Insect_DetourCrowdAIController.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AIntensif3ATI_TESTPlayerController::AIntensif3ATI_TESTPlayerController()
{
	bShowMouseCursor = true;
	bReachedLocation = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	StuckCounter = 0.f;
	AcceptanceRadius = 50.0f;
	ThresholdStuck = .5f;
	IsPossessing = false;
}

void AIntensif3ATI_TESTPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ControlledCharacter = Cast<AIntensif3ATI_TESTCharacter>(GetCharacter());
}

void AIntensif3ATI_TESTPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AIntensif3ATI_TESTPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AIntensif3ATI_TESTPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AIntensif3ATI_TESTPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AIntensif3ATI_TESTPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AIntensif3ATI_TESTPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AIntensif3ATI_TESTPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AIntensif3ATI_TESTPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AIntensif3ATI_TESTPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AIntensif3ATI_TESTPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AIntensif3ATI_TESTPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
		HitActor = Hit.GetActor();
	}
	
	// Move towards mouse pointer or touch
	if (ControlledCharacter != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
		PrevDist = FVector::Dist(CachedDestination, ControlledCharacter->GetActorLocation());
		ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AIntensif3ATI_TESTPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there
		bReachedLocation = false;

		if (!IsPossessing && HitActor) {
			
			AIntensif3ATI_TESTInsect* HitInsectCharacter = Cast<AIntensif3ATI_TESTInsect>(HitActor);
			UE_LOG(LogTemp, Warning, TEXT("Character: %s"), *HitInsectCharacter->GetName());
			
			if (HitInsectCharacter) {
				AInsect_DetourCrowdAIController* HitInsect = Cast<AInsect_DetourCrowdAIController>(HitInsectCharacter->GetController());
				UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *HitInsect->GetName());

				if (HitInsect) {
					AddInsectToCrowd(HitInsect);

					ControlledCharacter->DissolveBlob();
					ControlledCharacter->AbilityChange(HitInsect->GetInsectType());
					IsPossessing = true;

					bReachedLocation = true;
				}
			}
		}

		// We spawn some particles
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AIntensif3ATI_TESTPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AIntensif3ATI_TESTPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AIntensif3ATI_TESTPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bReachedLocation) {
		FVector WorldDirection = (CachedDestination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
		ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);

		float dist = FVector::Dist(CachedDestination, ControlledCharacter->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Difference Dist: %f"), FMath::Abs(PrevDist - dist));

		if (FMath::Abs(PrevDist - dist) <= 1.f) {
			StuckCounter = GetWorld()->GetDeltaSeconds();
		}

		PrevDist = dist;

		if (dist <= AcceptanceRadius)
			bReachedLocation = true;

		if (StuckCounter >= ThresholdStuck) {
			bReachedLocation = true;
			StuckCounter = 0.f;
		}
	}	
}

/** Add an insect to the crowd **/
bool AIntensif3ATI_TESTPlayerController::AddInsectToCrowd(AInsect_DetourCrowdAIController* insect)
{
	if (!insect) return false;

	insect->SetCorruption(true);

	int32 idx = InsectCrowd.Emplace(insect);

	if (idx < 0) return false;

	return true;
}

/** Remove all insect from the crowd **/
void AIntensif3ATI_TESTPlayerController::RemoveAllInsectFromCrowd()
{
	if (InsectCrowd.IsEmpty()) return;

	for (AInsect_DetourCrowdAIController* insect : InsectCrowd) {
		insect->SetCorruption(false);
	}

	InsectCrowd.Empty();
	ControlledCharacter->UnDissolveBlob();
	IsPossessing = false;
	ControlledCharacter->AbilityChange(EInsectType::Blob);
}