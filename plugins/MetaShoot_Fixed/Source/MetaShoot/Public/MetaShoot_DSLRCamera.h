// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"

#include "MetaShoot_DSLRCamera.generated.h"

/**
 * 
 */
UCLASS()
class METASHOOT_API AMetaShoot_DSLRCamera : public ACineCameraActor
{
	GENERATED_BODY()

public:

	/*#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif*/

	/*UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UStaticMeshComponent* CameraMesh02;*/

	UPROPERTY(BlueprintReadOnly, Category = "DSLR")
		bool BMSAssetActive = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Current Camera Settings", meta = (DisplayName = "DSLR Mesh Hidden in Render", DisplayPriority = 1))
		bool VCameraMeshHidden;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Current Camera Settings", meta = (DisplayName = "DSLR Mesh Scale", DisplayPriority = 2))
		float VCameraMeshScale = 2.5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Current Camera Settings", meta = (DisplayName = "Target", DisplayPriority = 3))
		AActor* VActorToTrack;

	//UFUNCTION(BlueprintCallable, CallInEditor, Category = "Current Camera Settings", meta = (DisplayName = "Fix DOF", DisplayPriority = 0, ToolTip = "Runs the command r.TemporalAA.Upsampling 0 to fix the Depth Of Field. Credit to William Faucher"))
	//	void FixDepthOfField();

	UFUNCTION(BlueprintImplementableEvent, Category = "DSLR")
		void FixDOFBP();

	UFUNCTION(BlueprintImplementableEvent, Category = "DSLR")
		void UpdateDSLR();

	UFUNCTION(BlueprintCallable, Category = "DSLR")
		void DefineFocusTarget();



	virtual void OnConstruction(const FTransform& Transform) override;
	
};
