// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BFL_Utility.h"
#include "GameFramework/Character.h"
#include "Intensif3ATI_TESTInsect.generated.h"

UCLASS()
class INTENSIF3ATI_TEST_API AIntensif3ATI_TESTInsect : public ACharacter
{
	GENERATED_BODY()

private:
	/** Mesh Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect|Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;

	/** Data Table of the Insect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Insect|Ability", meta = (AllowPrivateAccess = "true", RowType = "InsectData"))
	class UDataTable* InsectDataTable;

	/** Type of the Insect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect", meta = (AllowPrivateAccess = "true"))
	EInsectType InsectType;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AIntensif3ATI_TESTInsect();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Ability component of the blob **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UActorComponent* AbilityComponent;

	/** Returns MeshComp subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComp() const { return MeshComp; }

	/** Event to implement for the disolve effect **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Insect|Material")
	void CorruptInsect();

	/** Event to implement for the undisolve effect **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Insect|Material")
	void UnCorruptInsect();

	/** Change the current insect type **/
	UFUNCTION(BlueprintCallable, Category = "Insect|Ability")
	void SetInsectType(EInsectType type);

	/** Change the current ability **/
	void TypeChange();
};
