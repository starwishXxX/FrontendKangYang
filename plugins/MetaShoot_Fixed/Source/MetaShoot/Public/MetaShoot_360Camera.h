// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"

#include "MetaShoot_360Camera.generated.h"

/**
 * 
 */
UCLASS()
class METASHOOT_API AMetaShoot_360Camera : public ACineCameraActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "360 Camera")
		bool BMSAssetActive = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Current Camera Settings", meta = (DisplayName = "360 Camera Mesh Hidden in Render", DisplayPriority = 1))
		bool VCameraMeshHidden = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Current Camera Settings", meta = (DisplayName = "360 Camera Mesh Scale", DisplayPriority = 2))
		float VCameraMeshScale = 3;

	UFUNCTION(BlueprintImplementableEvent)
		void Update360Cam();


	virtual void OnConstruction(const FTransform& Transform) override;
	
};
