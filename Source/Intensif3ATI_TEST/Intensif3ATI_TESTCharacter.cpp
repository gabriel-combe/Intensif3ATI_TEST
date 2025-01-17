// Copyright Epic Games, Inc. All Rights Reserved.

#include "Intensif3ATI_TESTCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AIntensif3ATI_TESTCharacter::AIntensif3ATI_TESTCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(20.f, 20.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a mesh component
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlobMesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Add Tag
	Tags.Add(FName("Player"));

	AbilityComponent = CreateDefaultSubobject<UActorComponent>(FName("Ability Component"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AIntensif3ATI_TESTCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AIntensif3ATI_TESTCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AIntensif3ATI_TESTCharacter::AbilityChange(EInsectType type)
{
	FString name;
	UEnum::GetValueAsName(type).ToString().Split(TEXT("::"), nullptr, &name);

	FInsectData* row = InsectDataTable->FindRow<FInsectData>(FName(name), "InsectData");

	if (AbilityComponent)
		AbilityComponent->DestroyComponent();

	if (!row) return;

	AbilityComponent = AActor::AddComponentByClass(row->AbilityComponent, false, FTransform::Identity, false);
}