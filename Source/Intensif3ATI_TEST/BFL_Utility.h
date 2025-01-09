// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Utility.generated.h"

USTRUCT(BlueprintType)
struct FScanResult {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
	FVector normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
	float weight;

	FScanResult()
	{
		normal = FVector::ZeroVector;
		weight = 0;
	}

	FScanResult(FVector normal, float weight)
	{
		normal = normal;
		weight = weight;
	}
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
	static bool ArcCast(UObject* WorldContextObject, FVector center, FQuat rotation, float angle, float radius, int resolution, FHitResult& outHit, bool gizmo);

	UFUNCTION(BlueprintCallable, Category = "Scan", meta = (WorldContext = "WorldContextObject"))
	static TArray<FScanResult> Scan(UObject* WorldContextObject, FVector position, FQuat rotation, int armCount, float armLength, int armDuplicateCount, int armDuplicate, int armDuplicateAngle, float arcAngle, int arcResolution, bool gizmo);
	
	UFUNCTION(BlueprintCallable, Category = "Scan", meta = (WorldContext = "WorldContextObject"))
	static void Arm(UObject* WorldContextObject, FVector position, FQuat rotation, int duplicateCount, TArray<FScanResult>& points, float angle, float armLength, int armDuplicateCount, int armDuplicate, int armDuplicateAngle, float arcAngle, int arcResolution, bool gizmo);
};
