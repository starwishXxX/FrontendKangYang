// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/RectLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BillboardComponent.h"

#include "MetaShoot_Manifest.h"

#include "MetaShoot_SoftboxMaster.generated.h"

//UENUM(Category = "Softbox", meta = (DisplayName = "Light Type", DisplayPriority = 0))
//enum class EVLightTypeMS : uint8
//{
//	Softbox_60x60,
//	Softbox_50x70,
//	Softbox_30x120,
//	Softbox_70x70_Octagon,
//	Spotlight,
//	RingLight,
//	LightWand
//};
//
//UENUM(Category = "Softbox", meta = (DisplayName = "Support", DisplayPriority = 0))
//enum class EVLightSupportMS : uint8
//{
//	Automatic,
//	Tripod,
//	Scissors,
//	None
//};
//
//UENUM(Category = "Softbox", meta = (DisplayName = "Intensity Units", DisplayPriority = 0))
//enum class EVLightUnitsMS : uint8
//{
//	Unitless,
//	Candelas,
//	Lumens
//};

UCLASS()
class METASHOOT_API AMetaShoot_SoftboxMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_SoftboxMaster();

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		bool BMSAssetActive = true;

	UPROPERTY(BlueprintReadWrite, Category = "SoftboxMaster")
		UBillboardComponent* SpriteComponent;

	UTexture2D* SpriteTexture;

	//Softbox Settings

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Light Struct", DisplayPriority = 0))
		FSMetaShoot_Light FSoftBoxMaster;*/

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Light Type", DisplayPriority = 0))
		EVLightTypeMS VLightType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Intensity - Candelas", DisplayPriority = 1, ClampMin = "0.0", ClampMax = "2000.0", UIMin = "0.0", UIMax = "750.0"))
		float VLightIntensity = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Softbox", meta = (DisplayName = "Intensity Units", DisplayPriority = 2))
		EVLightUnitsMS VLightUnits = EVLightUnitsMS::Candelas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Temperature", DisplayPriority = 3, ClampMin = "1200.0", ClampMax = "12000.0", UIMin = "1200.0", UIMax = "12000.0", EditCondition = "VLightTemperatureBool"))
		int32 VLightTemperature = 6500;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Softbox", meta = (DisplayName = "Use Temperature", DisplayPriority = 4))
		bool VLightTemperatureBool = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Tint", DisplayPriority = 5))
		FLinearColor VLightTint = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Diffuser", DisplayPriority = 6, EditCondition = "VLightType != EVLightTypeMS::Spotlight && VLightType != EVLightTypeMS::Ringlight && VLightType != EVLightTypeMS::LightWand"))
		bool VLightDifuserBool = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Grid", DisplayPriority = 7, EditCondition = "VLightDifuserBool && VLightType != EVLightTypeMS::Spotlight && VLightType != EVLightTypeMS::RingLight && VLightType != EVLightTypeMS::LightWand"))
		bool VLightGridBool = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Support", DisplayPriority = 8))
		EVLightSupportMS VLightSupport;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Scissors Segments", DisplayPriority = 9))
		int32 VLightScissorsSegments = 8;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Softbox", meta = (DisplayName = "Actor to Track", DisplayPriority = 10))
		AActor* VActorToTrack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Rotation", DisplayPriority = 11, ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0", EditCondition = "VActorToTrack != nullptr"))
		float VLightRotation;
	

	//UPROPERTY(BlueprintReadOnly, Category = "Softbox", meta = (DisplayName = "Branding", DisplayPriority = 12, EditCondition = "VLightType != EVLightTypeMS::RingLight && VLightType != EVLightTypeMS::LightWand"))
	UPROPERTY(BlueprintReadOnly, Category = "Softbox")
		bool VBrandingBool = true;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Softbox", meta = (DisplayName = "Branded", DisplayPriority = 12), AdvancedDisplay)
		bool VBranded = true;*/
		
	UPROPERTY(BlueprintReadWrite, Category = "SoftboxMaster")
		uint8 VLightTypeIndex;

	UPROPERTY()
	float TripodLength;

	//Light Components

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		URectLightComponent* RectLightComponent;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UPointLightComponent* PointLightComponent;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		USpotLightComponent* SpotLightComponent;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* LightDiffuser;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* LightBox;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* LightGrid;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* LightBoxFrame;

	UPROPERTY(BlueprintReadWrite, Category = "SoftboxMaster")
		UStaticMeshComponent* LightBulb;

	//Tripod Meshes
	
	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* LightBodyFloor;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodHinge;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodLock00;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodLock01;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodPipe00;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodPipe01;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodPipe02;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* TripodBase;
	
	//Scissors Meshes

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* LightBodyCeiling;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* ScissorsHinge;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UStaticMeshComponent* ScissorsPipe;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UInstancedStaticMeshComponent* ScissorsSegment00;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UInstancedStaticMeshComponent* ScissorsSegment01;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UInstancedStaticMeshComponent* ScissorsSegment02;
	
	//Light Material Instances

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UMaterialInstanceDynamic* VLightMI;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UMaterialInstanceDynamic* VLightBulbMI;

	UPROPERTY(BlueprintReadOnly, Category = "SoftboxMaster")
		UMaterialInstanceDynamic* VBoxMI;
	
	//Functions

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void LookAtTarget();

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void UpdateDiffuser();

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void UpdateLight();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "SoftboxMaster")
		void UpdateLightMeshBP();

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void UpdateLightColor();

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void UpdateSupportTrace();

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void UpdateSupport();

	UFUNCTION(BlueprintCallable, Category = "SoftboxMaster")
		void UpdateAll(bool UpdateRail);

	//Other Variables

	float LightScale;
	float SB_RGBIntensity;
	float SB_LightOpacityFactor;

	//Temperature divided by 100 for calculations
	float KtoRGB_Temperature;

	//RGB components
	float KtoRGB_Red;
	float KtoRGB_Green;
	float KtoRGB_Blue;

	//Final result as an RGB Linear Color
	FLinearColor KtoRGB;

	//Support
	bool bIsHitTripod;
	bool bIsHitScissors;
	FHitResult HitDetailsTripod;
	FHitResult HitDetailsScissors;

	FVector HingeTripodLoc;
	FVector StartTripod;
	FVector EndTripod;

	FVector HingeScissorsLoc;
	FVector StartScissors;
	FVector EndScissors;
	float ScissorsInitialOffsetAdd;

	/*#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif*/

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//virtual void PostEditMove(bool bFinished) override;

private:

	virtual void UpdateTripod();
	virtual void UpdateScissors();

	//TArray<AMetaShoot_SoftboxMaster*> VLightUnitsArray;

};