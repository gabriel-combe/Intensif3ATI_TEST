// Fill out your copyright notice in the Description page of Project Settings.


#include "Intensif3ATI_TESTInsect.h"

// Sets default values
AIntensif3ATI_TESTInsect::AIntensif3ATI_TESTInsect()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIntensif3ATI_TESTInsect::BeginPlay()
{
	Super::BeginPlay();
	
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

void AIntensif3ATI_TESTInsect::SetInsectType(EInsectType type)
{
	InsectType = type;
	TypeChange();
}

void AIntensif3ATI_TESTInsect::TypeChange()
{
	FString name;
	UEnum::GetValueAsName(InsectType).ToString().Split(TEXT("::"), nullptr, &name);

	FInsectData* row = InsectDataTable->FindRow<FInsectData>(FName(name), "InsectData");

	if (AbilityComponent)
		AbilityComponent->DestroyComponent();

	if (!row) return;

	AbilityComponent = AActor::AddComponentByClass(row->AbilityComponent, false, FTransform::Identity, false);

	if (row->SkeletalMesh) {
		GetMesh()->SetSkeletalMesh(row->SkeletalMesh);
		GetMesh()->SetMaterial(0, row->Material);
	}

	if (row->Mesh) {
		GetMeshComp()->SetStaticMesh(row->Mesh);
		GetMeshComp()->SetMaterial(0, row->Material);
	}
}

