// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_DragDropOperation.h"

#include "Kismet/KismetMathLibrary.h"

void UMetaShoot_DragDropOperation::GetMouseLocationWorld()
{
	FViewport* activeViewport = GEditor->GetActiveViewport();
	FEditorViewportClient* editorViewClient = (activeViewport != nullptr) ? (FEditorViewportClient*)activeViewport->GetClient() : nullptr;
	if (editorViewClient)
	{
		viewPos = editorViewClient->GetViewLocation();
		viewRot = editorViewClient->GetViewRotation();

		bool bIsHitTripod;

		//Trace Params
		FCollisionQueryParams TraceParams(FName(TEXT("LineTraceParameters")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;
			
		//Trace
		FVector StartTrace = viewPos;
		FVector EndTrace = viewPos + UKismetMathLibrary::GetForwardVector(viewRot) * 10000;
		//EndTrace = FVector(0.0f, 0.0f, 0.0f);

		FHitResult HitDetailsViewport = FHitResult(ForceInit);

		bIsHitTripod = GetWorld()->LineTraceSingleByChannel(HitDetailsViewport, StartTrace, EndTrace, ECC_Visibility, TraceParams);

		spawnLoc = HitDetailsViewport.ImpactPoint;

		spawnLoc = viewPos + UKismetMathLibrary::GetForwardVector(viewRot) * 250;
	}
}