// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_Utility.h"

bool UBFL_Utility::ArcCast(UObject* WorldContextObject, FVector center, FQuat rotation, float angle, float radius, int resolution, FHitResult& outHit, bool gizmo)
{
	rotation *= FQuat::MakeFromEuler(FVector(0, angle / 2.f, 0));

	float dAngle = angle / resolution;
	FVector forwardRadius = FVector::ForwardVector * radius;

	FVector A, B;
	FCollisionQueryParams CollisionParameters;

	const AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
	if (IgnoreActor) CollisionParameters.AddIgnoredActor(IgnoreActor);

	for (int i = 0; i < resolution; i++) {
		A = center + rotation * forwardRadius;
		rotation *= FQuat::MakeFromEuler(FVector(0, -dAngle, 0));
		B = center + rotation * forwardRadius;

		if (WorldContextObject->GetWorld()->LineTraceSingleByChannel(outHit, A, B, ECollisionChannel::ECC_Visibility, CollisionParameters)) {

			if(gizmo)
				DrawDebugLine(WorldContextObject->GetWorld(), A, outHit.Location, FColor(255, 0, 0));
			return true;
		}

		if (gizmo)
			DrawDebugLine(WorldContextObject->GetWorld(), A, B, FColor(255, 0, 0));
	}

	outHit = FHitResult();
	return false;
}

TArray<FScanResult> UBFL_Utility::Scan(UObject* WorldContextObject, FVector position, FQuat rotation, int armCount, float armLength, int armDuplicateCount, int armDuplicate, int armDuplicateAngle, float arcAngle, int arcResolution, bool gizmo)
{
	TArray<FScanResult> points;

	for (int i = 0; i < armCount; i++) {
		float angle = 360 * i / armCount;

		Arm(WorldContextObject,
			position,
			rotation * FQuat::MakeFromEuler(FVector(0, 0, angle)),
			1,
			points,
			angle,
			armLength,
			armDuplicateCount,
			armDuplicate,
			armDuplicateAngle,
			arcAngle,
			arcResolution,
			gizmo);
	}

	return points;
}

void UBFL_Utility::Arm(UObject* WorldContextObject, FVector position, FQuat rotation, int duplicateCount, TArray<FScanResult>& points, float angle, float armLength, int armDuplicateCount, int armDuplicate, int armDuplicateAngle, float arcAngle, int arcResolution, bool gizmo)
{
	FHitResult outHit;

	if (!ArcCast(WorldContextObject, position, rotation, arcAngle, armLength, arcResolution, outHit, gizmo)) return;

	float weight = 1 - (float)(duplicateCount - 1) / armDuplicateCount;

	if (gizmo)
		DrawDebugLine(WorldContextObject->GetWorld(), position, outHit.ImpactPoint, FColor(255, 255, 255, 255 * weight));

	position = outHit.ImpactPoint;
	rotation = FQuat::FindBetweenVectors(rotation * FVector::UpVector, outHit.ImpactNormal) * rotation;

	points.Add(FScanResult(outHit.ImpactNormal, weight));

	if (gizmo)
		DrawDebugSphere(WorldContextObject->GetWorld(), position, 1.f, 8, FColor(255, 255, 255, 255 * weight));

	if (duplicateCount == armDuplicateCount) return;

	for (int i = 0; i < armDuplicate; i++) {
		float angleCrt = armDuplicate == 1 ? 0 : armDuplicateAngle * (-0.5f + (float)i / (armDuplicate - 1));

		Arm(WorldContextObject,
			position,
			rotation * FQuat::MakeFromEuler(FVector(0, 0, angleCrt)),
			duplicateCount+1,
			points,
			angle + angleCrt,
			armLength,
			armDuplicateCount,
			armDuplicate,
			armDuplicateAngle,
			arcAngle,
			arcResolution,
			gizmo);
	}
}