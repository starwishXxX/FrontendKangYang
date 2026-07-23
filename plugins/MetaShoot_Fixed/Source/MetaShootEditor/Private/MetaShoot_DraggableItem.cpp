// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_DraggableItem.h"

#include "Kismet/KismetMathLibrary.h"
#include "Engine/GameViewportClient.h"
#include "Editor/EditorEngine.h"


void UMetaShoot_DraggableItem::GetMouseLocationWorld()
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

		//spawnLoc = HitDetailsViewport.ImpactPoint;

		spawnLoc = viewPos + UKismetMathLibrary::GetForwardVector(viewRot) * 250;
	}
}

void UMetaShoot_DraggableItem::GetMouseTimerFunctionBP()
{
	FViewport* activeViewport = GEditor->GetActiveViewport();
	FEditorViewportClient* editorViewClient = (activeViewport != nullptr) ? (FEditorViewportClient*)activeViewport->GetClient() : nullptr;
	if (editorViewClient)
	{
		//mousePosVector = FVector2D(editorViewClient->GetCachedMouseX(), editorViewClient->GetCachedMouseY());
		//mousePosVector = editorViewClient->GetCursorWorldLocationFromMousePos().GetCursorPos();
		mouseDirection = editorViewClient->GetCursorWorldLocationFromMousePos().GetDirection();
	}

		/*FIntPoint mousePos;
		GEditor->GetActiveViewport()->GetMousePos(mousePos, true);
		GEditor->GetLevelViewportClients()->;
		int32 mouseX = mousePos.X;
		int32 mouseY = mousePos.Y;

		float mouseU = float(mouseX) / float(GEditor->GetActiveViewport()->GetSizeXY().X);
		float mouseV = float(mouseY) / float(GEditor->GetActiveViewport()->GetSizeXY().Y);

		mousePosVector = FVector2D(editorViewClient->GetCachedMouseX(), editorViewClient->GetCachedMouseY());*/


	//ViewportClient->GetMousePosition(mousePos);
	GetMouseTimerFunction();
}

void UMetaShoot_DraggableItem::SelectSpawnedActor()
{
	GEditor->SelectNone(false, true, false);
	if (SpawnedActor != nullptr)
	{
		GEditor->SelectActor(SpawnedActor, true, true, true, true);
	}
}