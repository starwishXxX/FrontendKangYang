// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_RectLight.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"


// Sets default values
AMetaShoot_RectLight::AMetaShoot_RectLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RectLightComponent = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight"));

	LightPlane = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightPlane"));

	LightPlane->SetupAttachment(RectLightComponent);

	LightPlane->SetRelativeRotation(FRotator(0.0f, -90.0f, 90.0f));
	LightPlane->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LightPlane->SetRelativeScale3D(FVector(0.64f, 0.64f, 1.0f));
	LightPlane->SetCastShadow(bool(false));

	//LightPlane->SetMaterial(0, );
	//Material'/MetaShotVINZI/Light_Mat.Light_Mat'



	//UMaterialInstanceDynamic* MILight = LightPlane->CreateDynamicMaterialInstance(0, 0, FName("LightMaterial"));
	
	//MILight = LightPlane->CreateDynamicMaterialInstance(0, 0, FName("LightMaterial"));
	
	//LightPlane->SetMaterial(0, MILight);


	//static ConstructorHelpers::FObjectFinder<UStaticMesh> LightPlaneMesh(TEXT(""));

	//if (LightPlaneMesh.Succeeded())
	//{
		//LightPlane->SetStaticMesh(LightPlaneMesh.Object);
		
	//}

	//LightPlane->SetStaticMesh();
	//LightPlane->bReceivesDecals = false;
	//LightPlane->bCastDynamicShadow = true;
	//LightPlane->SetOnlyOwnerSee(false);
	//LightPlane->SetOwnerNoSee(false);

	// set path for static mesh. Check path of mesh in content browser.
	//static ConstructorHelpers::FObjectFinder Shotgun(TEXT("StaticMesh'/Game/shotgun.shotgun'"));

		
	AMetaShoot_RectLight::LookAtTarget();
}

// Called when the game starts or when spawned
void AMetaShoot_RectLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaShoot_RectLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMetaShoot_RectLight::LookAtTarget();

	//if (GEngine)
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));    
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("World delta for current frame equals %f"), GetWorld()->TimeSeconds));
	
}

float RGBIntesity;
float LightTemperature;
float LightTemperature100;
float LightTempR;
float LightTempG;
float LightTempB;
float LightOpacityFactor;


void AMetaShoot_RectLight::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	AMetaShoot_RectLight::LookAtTarget();

	RGBIntesity = RectLightComponent->Intensity/25.5;
	LightTemperature = RectLightComponent->Temperature;

	//Kelvin Temperature to RGB | Credit to Tanner Helland for the base algorithm
	LightTemperature100 = LightTemperature / 100;
	LightTempR = 0;
	LightTempG = 0;
	LightTempB = 0;

	if (RectLightComponent->bUseTemperature == true)
	{
		
		//Red
		if (LightTemperature100 <= 66)
		{
			LightTempR = 255;
		}
		else
		{
			LightTempR = LightTemperature100 - 60;
			LightTempR = 329.698727446 * pow(LightTempR, -0.1332047592);
		}
		if (LightTempR < 0)
		{
			LightTempR = 0;
		}
		if (LightTempR > 255)
		{
			LightTempR = 255;
		}

		//Green
		if (LightTemperature100 <= 66)
		{
			LightTempG = LightTemperature100;
			LightTempG = 99.4708025861 * log(LightTempG) - 161.1195681661;
		}
		else
		{
			LightTempG = LightTemperature100 - 60;
			LightTempG = 288.1221695283 * pow(LightTempG, -0.0755148492);
		}
		if (LightTempG < 0)
		{
			LightTempG = 0;
		}
		if (LightTempG > 255)
		{
			LightTempG = 255;
		}

		//Blue
		if (LightTemperature100 >= 66)
		{
			LightTempB = 255;
		}
		else
		{
			if (LightTemperature100 <= 19)
			{
				LightTempB = 0;
			}
			else
			{
				LightTempB = LightTemperature100 - 10;
				LightTempB = 138.5177312231 * log(LightTempB) - 305.0447927307;
			}
		}
		if (LightTempB < 0)
		{
			LightTempB = 0;
		}
		if (LightTempB > 255)
		{
			LightTempB = 255;
		}

		LightTempR = LightTempR / 10;
		LightTempG = LightTempG / 10;
		LightTempB = LightTempB / 10;

	}
	else
	{
		LightTempR = 25.5f;
		LightTempG = 25.5f;
		LightTempB = 25.5f;
	}


	float LightWidth = RectLightComponent->SourceWidth/100;
	float LightHeight = RectLightComponent->SourceHeight/100;

	LightOpacityFactor = LightWidth * LightHeight * 1;

	LightPlaneTexture = RectLightComponent->SourceTexture;

	

	//static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("MetaShotVINZI/Light_Mat.Light_Mat"));
	/*
	if (Material.Object != NULL)
	{
		TheMaterial = (UMaterial*)Material.Object;
		FString TheFloatStr = FString::SanitizeFloat(RectLightComponent->Intensity);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, *TheFloatStr);
	}*/

	/*MILight = LightPlane->CreateDynamicMaterialInstance(0, 0, FName("LightMaterial"));
	MILight->SetVectorParameterValue(FName("LightIntensity_Param"), FLinearColor(RGBIntesity, RGBIntesity, RGBIntesity, 1.0));
	MILight->SetScalarParameterValue(FName("LightTemperature_Param"), LightTemperature);
	MILight->SetTextureParameterValue(FName("LightTexture_Param"), LightPlaneTexture);
	MILight->SetScalarParameterValue(FName("OpacityFactor_Param"), LightOpacityFactor);
	MILight->SetVectorParameterValue(FName("LightTemperatureRGB_Param"), FLinearColor(LightTempR, LightTempG, LightTempB, 1.0));
	if (LightPlaneTexture)
	{
		MILight->SetScalarParameterValue(FName("SourceTextureActive_Param"), 1.0f);
	}*/


	MILight = LightPlane->CreateDynamicMaterialInstance(0, 0, FName("LightMaterial"));

	AMetaShoot_RectLight::UpdateLightPlaneBP();

	LightPlane->SetRelativeScale3D(FVector(LightWidth, LightHeight, 1.0f));

	//FString TheFloatStr = FString::SanitizeFloat(RectLightComponent->IntensityUnits);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, *TheFloatStr);

	//FString TheFloatStr = FString::SanitizeFloat(RectLightComponent->Intensity);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, *TheFloatStr);

}

void AMetaShoot_RectLight::LookAtTargetBP()
{
	AMetaShoot_RectLight::LookAtTarget();
}

void AMetaShoot_RectLight::LookAtTarget()
{
	if (IsValid(TargetActor))
	{
		this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), TargetActor->GetActorLocation()));
	}
}

void AMetaShoot_RectLight::UpdateLightPlaneBP()
{
	
	MILight->SetVectorParameterValue(FName("LightIntensity_Param"), FLinearColor(RGBIntesity, RGBIntesity, RGBIntesity, 1.0));
	MILight->SetScalarParameterValue(FName("LightTemperature_Param"), LightTemperature);
	MILight->SetTextureParameterValue(FName("LightTexture_Param"), LightPlaneTexture);
	MILight->SetScalarParameterValue(FName("OpacityFactor_Param"), LightOpacityFactor);
	MILight->SetVectorParameterValue(FName("LightTemperatureRGB_Param"), FLinearColor(LightTempR, LightTempG, LightTempB, 1.0));
	if (LightPlaneTexture)
	{
		MILight->SetScalarParameterValue(FName("SourceTextureActive_Param"), 1.0f);
	}
	else
	{
		MILight->SetScalarParameterValue(FName("SourceTextureActive_Param"), 0.0f);
	}
}