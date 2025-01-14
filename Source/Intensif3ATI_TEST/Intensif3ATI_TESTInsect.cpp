// Fill out your copyright notice in the Description page of Project Settings.


#include "Intensif3ATI_TESTInsect.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/Material.h"

// Sets default values
AIntensif3ATI_TESTInsect::AIntensif3ATI_TESTInsect()
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
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InsectMesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Relocate the meshes
	GetMesh()->AddRelativeLocation(FVector(0, 0, -20));
	GetMeshComp()->AddRelativeLocation(FVector(0, 0, -20));

	// Add Tag
	Tags.Add(FName("Insect"));

	AbilityComponent = CreateDefaultSubobject<UActorComponent>(FName("Ability Component"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void AIntensif3ATI_TESTInsect::BeginPlay()
{
	Super::BeginPlay();

	TypeChange();	
}

#if WITH_EDITOR

void AIntensif3ATI_TESTInsect::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AIntensif3ATI_TESTInsect, InsectType))
	{
		TypeChange();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called every frame
void AIntensif3ATI_TESTInsect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIntensif3ATI_TESTInsect::SetBlackboard(UBlackboardComponent* b)
{
	Blackboard = b;
}

// Called to bind functionality to input
void AIntensif3ATI_TESTInsect::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AIntensif3ATI_TESTInsect::SetInsectType(EInsectType type)
{
	InsectType = type;
	TypeChange();
}

void AIntensif3ATI_TESTInsect::TypeChange()
{
	// Find Row name based on the enum type
	FString name;
	UEnum::GetValueAsName(InsectType).ToString().Split(TEXT("::"), nullptr, &name);

	// Get the row from the insect table
	FInsectData* row = InsectDataTable->FindRow<FInsectData>(FName(name), "InsectData");

	// Clear any previous data on the insect
	if (AbilityComponent)
		AbilityComponent->DestroyComponent();

	GetMesh()->SetSkeletalMesh(nullptr);
	GetMesh()->SetAnimInstanceClass(nullptr);
	GetMesh()->SetMaterial(0, nullptr);
	GetMeshComp()->SetStaticMesh(nullptr);
	GetMeshComp()->SetMaterial(0, nullptr);

	// Check if row exist
	if (!row) return;

	// Create the new ability actor component
	AbilityComponent = AActor::AddComponentByClass(row->AbilityComponent, false, FTransform::Identity, false);

	// If there is a skeletal mesh set it up (skmesh + mat + anim)
	if (row->SkeletalMesh) {
		GetMesh()->SetSkeletalMesh(row->SkeletalMesh);
		GetMesh()->SetMaterial(0, row->Material);
		
		if (row->AnimClass)
			GetMesh()->SetAnimInstanceClass(row->AnimClass->GetAnimBlueprintGeneratedClass());
	}

	// If there is a mesh set it up (mesh + mat)
	if (row->Mesh) {
		GetMeshComp()->SetStaticMesh(row->Mesh);
		GetMeshComp()->SetMaterial(0, row->Material);
	}
}

