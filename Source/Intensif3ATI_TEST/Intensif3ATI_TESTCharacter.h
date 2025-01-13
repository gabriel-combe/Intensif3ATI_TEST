// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BFL_Utility.h"
#include "GameFramework/Character.h"
#include "Intensif3ATI_TESTCharacter.generated.h"

UCLASS(Blueprintable)
class AIntensif3ATI_TESTCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Mesh Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blob|Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Data Table of the Insect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blob|Ability", meta = (AllowPrivateAccess = "true", RowType = "InsectData"))
	class UDataTable* InsectDataTable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AIntensif3ATI_TESTCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Ability component of the blob **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UActorComponent* AbilityComponent;

	/** Returns MeshComp subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComp() const { return MeshComp; }
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Event to implement for the disolve effect **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Blob|Material")
	void CorruptInsect();

	/** Event to implement for the undisolve effect **/
	UFUNCTION(BlueprintImplementableEvent, Category = "Blob|Material")
	void UnCorruptInsect();

	/** Change the current ability **/
	UFUNCTION(BlueprintCallable, Category = "Blob|Ability")
	void AbilityChange(EInsectType type);
};

