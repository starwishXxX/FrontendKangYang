// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/RectLightComponent.h"

#include "MetaShoot_OverheadLightBank.generated.h"


UCLASS()
class METASHOOT_API AMetaShoot_OverheadLightBank : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_OverheadLightBank();

	//Settings

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		bool BMSAssetActive = true;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "LightBank", meta = (DisplayName = "Intensity - Candelas", DisplayPriority = 1, ClampMin = "0.0", ClampMax = "2000.0", UIMin = "0.0", UIMax = "750.0"))
		float VLightIntensity = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "LightBank", meta = (DisplayName = "Temperature", DisplayPriority = 3, ClampMin = "1200.0", ClampMax = "12000.0", UIMin = "1200.0", UIMax = "12000.0", EditCondition = "VLightTemperatureBool"))
		int32 VLightTemperature = 6500;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LightBank", meta = (DisplayName = "Use Temperature", DisplayPriority = 4))
		bool VLightTemperatureBool = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "LightBank", meta = (DisplayName = "Tint", DisplayPriority = 5))
		FLinearColor VLightTint = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LightBank", meta = (DisplayName = "Actor to Track", DisplayPriority = 10))
		AActor* VActorToTrack;

	//Light Components

	UPROPERTY()
		USceneComponent* LightBank_Root;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		URectLightComponent* RectLightComponent;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* LightDiffuser;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* LightBox;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* LightBoxFrame;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightMesh00;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightMesh01;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightMesh02;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightMesh03;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightMesh04;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightHingeMesh00;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightHingeMesh01;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightHingeMesh02;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightHingeMesh03;
	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UStaticMeshComponent* SpotlightHingeMesh04;

	//Light Material Instances

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UMaterialInstanceDynamic* VLightMI;

	UPROPERTY(BlueprintReadOnly, Category = "LightBank")
		UMaterialInstanceDynamic* VSpotLightMI;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "LightBank")
		void UpdateAll();

	UFUNCTION(BlueprintCallable, Category = "LightBank")
		void UpdateDiffuser();

	UFUNCTION(BlueprintCallable, Category = "LightBank")
		void UpdateLight();

	UFUNCTION(BlueprintImplementableEvent, Category = "LightBank")
		void UpdateLightMeshBP();

	UFUNCTION(BlueprintCallable, Category = "LightBank")
		void UpdateLightColor();

	UFUNCTION(BlueprintCallable, Category = "LightBank")
		void LookAtTarget();

	//Other Variables

	float LightScale;
	float SB_RGBIntensity;

	//Temperature divided by 100 for calculations
	float KtoRGB_Temperature;

	//RGB components
	float KtoRGB_Red;
	float KtoRGB_Green;
	float KtoRGB_Blue;

	//Final result as an RGB Linear Color
	FLinearColor KtoRGB;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
