// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BFL_Utility.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Intensif3ATI_TESTInsect.generated.h"

UCLASS()
class INTENSIF3ATI_TEST_API AIntensif3ATI_TESTInsect : public ACharacter
{
	GENERATED_BODY()

private:
	// Blackboard of the controller
	TObjectPtr<UBlackboardComponent> Blackboard;

	/** Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Insect|Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComp;

	/** Data Table of the Insect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Insect|Ability", meta = (AllowPrivateAccess = "true", RowType = "InsectData"))
	TObjectPtr<UDataTable> InsectDataTable;

	/** Type of the Insect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect", meta = (AllowPrivateAccess = "true"))
	EInsectType InsectType;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Behaviour tree of the controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> Tree;

public:	
	/** Ability component of the blob **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UActorComponent> AbilityComponent;

	// Sets default values for this character's properties
	AIntensif3ATI_TESTInsect();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get the Blackboard associated with the controller
	void SetBlackboard(UBlackboardComponent* b);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns MeshComp subobject **/
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetMeshComp() const { return MeshComp; }
	/** Returns Behaviour Tree subobject **/
	FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviourTree() const { return Tree; }
	/** Returns the insect type **/
	FORCEINLINE EInsectType GetInsectType() const { return InsectType; }

	/** Event to implement for the corruption effect **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Insect|Corruption")
	void CorruptInsect();

	/** Event to implement for the uncorruption effect **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Insect|Corruption")
	void UnCorruptInsect();

	/** Change the current insect type **/
	UFUNCTION(BlueprintCallable, Category = "Insect|Ability")
	void SetInsectType(EInsectType type);

	/** Change the current ability **/
	void TypeChange();
};
