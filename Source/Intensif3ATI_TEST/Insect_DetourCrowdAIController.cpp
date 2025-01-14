// Fill out your copyright notice in the Description page of Project Settings.


#include "Insect_DetourCrowdAIController.h"
#include "Intensif3ATI_TESTPlayerController.h"
#include "Kismet/GameplayStatics.h"

AInsect_DetourCrowdAIController::AInsect_DetourCrowdAIController()
{
	SetupPerceptionSystem();

	// Call the stimulus setup function
	SetupStimulusSource();

	GetPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
}

void AInsect_DetourCrowdAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InsectCharacter = Cast<AIntensif3ATI_TESTInsect>(InPawn);
	if (!InsectCharacter) return;

	const TObjectPtr<UBehaviorTree> tree = InsectCharacter->GetBehaviourTree();
	if (!tree) return;

	UBlackboardComponent* b;
	UseBlackboard(tree->BlackboardAsset, b);
	Blackboard = b;

	InsectCharacter->SetBlackboard(b);

	RunBehaviorTree(tree);
}

void AInsect_DetourCrowdAIController::SetCorruption(bool isCorrupted)
{
	GetBlackboardComponent()->SetValueAsBool("IsCorrupted", isCorrupted);

	//GetPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), isCorrupted);

	if (isCorrupted)
		InsectCharacter->CorruptInsect();
	else
		InsectCharacter->UnCorruptInsect();
}

void AInsect_DetourCrowdAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (!SightConfig) return;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = 150.0f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AInsect_DetourCrowdAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AInsect_DetourCrowdAIController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
{
	// Get Insect character
	const TObjectPtr<AInsect_DetourCrowdAIController> Insect = Cast<AInsect_DetourCrowdAIController>(Actor);
	if (!Insect || Insect->GetBlackboardComponent()->GetValueAsBool("IsCorrupted") || GetInsectType() != Insect->GetInsectType()) return;

	if (!Stimulus.WasSuccessfullySensed()) return;

	TObjectPtr<AIntensif3ATI_TESTPlayerController> Player = Cast<AIntensif3ATI_TESTPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Player->AddInsectToCrowd(Insect);
}

void AInsect_DetourCrowdAIController::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (!StimulusSource) return;

	StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimulusSource->RegisterWithPerceptionSystem();
}