// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Intensif3ATI_TESTInsect.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "DetourCrowdAIController.h"
#include "Insect_DetourCrowdAIController.generated.h"

/**
 * 
 */
UCLASS()
class INTENSIF3ATI_TEST_API AInsect_DetourCrowdAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
	
private:
	class UAISenseConfig_Sight* SightConfig;

	// Store the possessed insect character
	UPROPERTY(VisibleAnywhere, Category = "Insect")
	TObjectPtr<AIntensif3ATI_TESTInsect> InsectCharacter;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);

	// Stimulus source of the player used by the monster ai
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	// Setup of the stimulus source
	void SetupStimulusSource();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	AInsect_DetourCrowdAIController();

	/** Returns if the insect is corrupted **/
	FORCEINLINE bool IsCorrupted() { return GetBlackboardComponent()->GetValueAsBool("IsCorrupted"); }
	/** Returns the type of the controlled insect **/
	FORCEINLINE EInsectType GetInsectType() { return InsectCharacter->GetInsectType(); }

	/** Mark the insect as corrupted **/
	UFUNCTION(BlueprintCallable, Category = "Insect|AI")
	void SetCorruption(bool isCorrupted);
};
