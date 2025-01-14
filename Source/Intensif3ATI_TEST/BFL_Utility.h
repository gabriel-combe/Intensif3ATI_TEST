// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "BFL_Utility.generated.h"

UENUM(BlueprintType)
enum class EInsectType : uint8 {
	Blob	UMETA(DisplayName = "Blob"),
	Spider	UMETA(DisplayName = "Spider"),
	Worm	UMETA(DisplayName = "Worm"),
};

USTRUCT(BlueprintType)
struct FScanResult {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
	FVector Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
	float Weight;

	FScanResult()
	{
		Normal = FVector::ZeroVector;
		Weight = 0;
	}

	FScanResult(FVector normal, float weight)
	{
		Normal = normal;
		Weight = weight;
	}
};

USTRUCT(BlueprintType)
struct FInsectData : public FTableRowBase {
	GENERATED_BODY()

	// Type of Insect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect")
	EInsectType Type;

	// Ability Component associated to the Insect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect")
	TSubclassOf<UActorComponent> AbilityComponent;

	// Skeletal Mesh of the Insect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	// Anim Class for the skeletal mesh of the insect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect")
	TObjectPtr<UAnimBlueprint> AnimClass;

	// Mesh of the Insect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect")
	TObjectPtr<UStaticMesh> Mesh;

	// Material of the Insect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insect")
	TObjectPtr<UMaterial> Material;
};

/**
 * 
 */
UCLASS()
class INTENSIF3ATI_TEST_API UBFL_Utility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "RayCast", meta = (WorldContext = "WorldContextObject"))
	static bool ArcCast(UObject* WorldContextObject, FVector center, FQuat rotation, float angle, float radius, int resolution, FHitResult& outHit, AActor* IgnoreActor, bool gizmo);

	UFUNCTION(BlueprintCallable, Category = "Scan", meta = (WorldContext = "WorldContextObject"))
	static TArray<FScanResult> Scan(UObject* WorldContextObject, FVector position, FQuat rotation, int armCount, float armLength, int armDuplicateCount, int armDuplicate, int armDuplicateAngle, float arcAngle, int arcResolution, AActor* IgnoreActor, bool gizmo);
	
	UFUNCTION(BlueprintCallable, Category = "Scan", meta = (WorldContext = "WorldContextObject"))
	static void Arm(UObject* WorldContextObject, FVector position, FQuat rotation, int duplicateCount, TArray<FScanResult>& points, float angle, float armLength, int armDuplicateCount, int armDuplicate, int armDuplicateAngle, float arcAngle, int arcResolution, AActor* IgnoreActor, bool gizmo);
};
