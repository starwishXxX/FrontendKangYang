// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "MetaShoot_StudioStructure.generated.h"

/**
 * 
 */
class METASHOOT_API MetaShoot_StudioStructure
{
public:
	MetaShoot_StudioStructure();
	~MetaShoot_StudioStructure();
};

UENUM(Category = "Softbox", meta = (DisplayName = "Light Type"))
enum class EVLightTypeMS : uint8
{
	Softbox_60x60,
	Softbox_50x70,
	Softbox_30x120,
	Softbox_70x70_Octagon,
	Spotlight,
	RingLight,
	LightWand
};

UENUM(Category = "Softbox", meta = (DisplayName = "Support"))
enum class EVLightSupportMS : uint8
{
	Automatic,
	Tripod,
	Scissors,
	None
};

UENUM(Category = "Softbox", meta = (DisplayName = "Intensity Units"))
enum class EVLightUnitsMS : uint8
{
	Unitless,
	Candelas,
	Lumens
};

USTRUCT(BlueprintType)
struct FSMetaShoot_Environment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
        FString Cyclorama5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
        FString Turntable5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
        FString RailSystem5;
};

USTRUCT(BlueprintType)
struct FSMetaShoot_Light
{
    GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Light Type", DisplayPriority = 0))
		EVLightTypeMS VLightType = EVLightTypeMS::Softbox_60x60;

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
		EVLightSupportMS VLightSupport = EVLightSupportMS::Automatic;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Scissors Segments", DisplayPriority = 9))
		int32 VLightScissorsSegments = 8;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Softbox", meta = (DisplayName = "Actor to Track", DisplayPriority = 10))
		AActor* VActorToTrack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Softbox", meta = (DisplayName = "Rotation", DisplayPriority = 11, ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0", EditCondition = "VActorToTrack != nullptr"))
		float VLightRotation = 0;
};

USTRUCT(BlueprintType)
struct FSMetaShoot_Studio
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaShoot")
        struct FSMetaShoot_Environment Environment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MetaShoot")
        TArray<FSMetaShoot_Light> Lights;
};