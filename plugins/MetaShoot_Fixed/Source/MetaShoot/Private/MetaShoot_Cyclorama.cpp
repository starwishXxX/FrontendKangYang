// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_Cyclorama.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"

// Sets default values
AMetaShoot_Cyclorama::AMetaShoot_Cyclorama()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cyclorama_Root = CreateDefaultSubobject<USceneComponent>(FName("Cyclorama Root"));
	this->SetRootComponent(Cyclorama_Root);

	Cyclorama_C = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cyclorama C"));
	Cyclorama_C->SetupAttachment(Cyclorama_Root);
	Cyclorama_C->SetRelativeLocation(FVector(-XOffset, 0.0f, -0.5f));
	
	Cyclorama_R = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cyclorama R"));
	Cyclorama_R->SetupAttachment(Cyclorama_Root);

	Cyclorama_L = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cyclorama L"));
	Cyclorama_L->SetupAttachment(Cyclorama_Root);


	Cyclorama_Frame00 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame00"));
	Cyclorama_Frame00->SetupAttachment(Cyclorama_L);
	Cyclorama_Frame00->SetRelativeLocation(FVector(-40.0f, 0.0f, 0.0f));

	Cyclorama_Frame01 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame01"));
	Cyclorama_Frame01->SetupAttachment(Cyclorama_L);
	Cyclorama_Frame01->SetRelativeLocation(FVector(120.0, 0.0f, 0.0f));

	Cyclorama_Frame02 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame02"));
	Cyclorama_Frame02->SetupAttachment(Cyclorama_L);
	Cyclorama_Frame02->SetRelativeLocation(FVector(280.0f, 0.0f, 0.0f));

	Cyclorama_Frame03 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame03"));
	Cyclorama_Frame03->SetupAttachment(Cyclorama_L);
	Cyclorama_Frame03->SetRelativeLocation(FVector(440.0f, 0.0f, 0.0f));


	Cyclorama_Frame04 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame04"));
	Cyclorama_Frame04->SetupAttachment(Cyclorama_R);
	Cyclorama_Frame04->SetRelativeLocation(FVector(-40.0f, 0.0f, 0.0f));
	Cyclorama_Frame04->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	Cyclorama_Frame05 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame05"));
	Cyclorama_Frame05->SetupAttachment(Cyclorama_R);
	Cyclorama_Frame05->SetRelativeLocation(FVector(120.0, 0.0f, 0.0f));
	Cyclorama_Frame05->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	Cyclorama_Frame06 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame06"));
	Cyclorama_Frame06->SetupAttachment(Cyclorama_R);
	Cyclorama_Frame06->SetRelativeLocation(FVector(280.0f, 0.0f, 0.0f));
	Cyclorama_Frame06->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	Cyclorama_Frame07 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Frame07"));
	Cyclorama_Frame07->SetupAttachment(Cyclorama_R);
	Cyclorama_Frame07->SetRelativeLocation(FVector(440.0f, 0.0f, 0.0f));
	Cyclorama_Frame07->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	Cyclorama_Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(FName("Fog"));
	Cyclorama_Fog->SetupAttachment(Cyclorama_Root);
	Cyclorama_Fog->SetVolumetricFog(true);


	/*Cyclorama_Frame->AddInstance(FTransform ( FRotator(0.0f, 0.0f, 0.0f), FVector(-40.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f) ) );
	Cyclorama_Frame->AddInstance(FTransform ( FRotator(0.0f, 0.0f, 0.0f), FVector(120.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f) ) );
	Cyclorama_Frame->AddInstance(FTransform ( FRotator(0.0f, 0.0f, 0.0f), FVector(280.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f) ) );
	Cyclorama_Frame->AddInstance(FTransform ( FRotator(0.0f, 0.0f, 0.0f), FVector(440.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f) ) );*/

	//Cyclorama_Decal = CreateDefaultSubobject<UDecalComponent>(FName("Decal"));
	//Cyclorama_Decal->SetupAttachment(Cyclorama_C);

	//Cyclorama_Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(FName("Fog"));
	//Cyclorama_Fog->SetupAttachment(Cyclorama_C);

}

// Called when the game starts or when spawned
void AMetaShoot_Cyclorama::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaShoot_Cyclorama::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMetaShoot_Cyclorama::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AMetaShoot_Cyclorama::UpdateCyclorama();

}

void AMetaShoot_Cyclorama::UpdateCyclorama()
{
	if (BMSAssetActive) {
		this->SetActorHiddenInGame(false);

		Cyclorama_C->SetRelativeScale3D(FVector(1.0f, ((VWidth / 2) - 68.291) / 250, 1.0f));
		Cyclorama_L->SetRelativeLocation(FVector(-XOffset, -((VWidth / 2) - 68.291), -0.5f));
		Cyclorama_R->SetRelativeLocation(FVector(-XOffset, (VWidth / 2) - 68.291, -0.5f));

		if (VUnlit)
		{
			Cyclorama_C->SetMaterial(0, Cyclorama_C->GetMaterial(2));
			Cyclorama_L->SetMaterial(0, Cyclorama_L->GetMaterial(2));
			Cyclorama_R->SetMaterial(0, Cyclorama_R->GetMaterial(2));
		}
		else
		{
			Cyclorama_C->SetMaterial(0, Cyclorama_C->GetMaterial(1));
			Cyclorama_L->SetMaterial(0, Cyclorama_L->GetMaterial(1));
			Cyclorama_R->SetMaterial(0, Cyclorama_R->GetMaterial(1));
		}

		VCycloramaCMI = Cyclorama_C->CreateDynamicMaterialInstance(0, 0, FName("CycloramaCenterMaterial"));
		VCycloramaCMI->SetVectorParameterValue(FName("CycloramaColor"), VColorC);
		VCycloramaCMI->SetScalarParameterValue(FName("Reflection"), VReflection);
		VCycloramaCMI->SetScalarParameterValue(FName("Roughness"), VRoughness);

		VCycloramaSMIR = Cyclorama_R->CreateDynamicMaterialInstance(0, 0, FName("CycloramaSidesMaterial"));
		VCycloramaSMIL = Cyclorama_L->CreateDynamicMaterialInstance(0, 0, FName("CycloramaSidesMaterial"));
		if (VLockColorS)
		{
			VCycloramaSMIR->SetVectorParameterValue(FName("CycloramaColor"), VColorC);
			VCycloramaSMIL->SetVectorParameterValue(FName("CycloramaColor"), VColorC);
		}
		else
		{
			VCycloramaSMIR->SetVectorParameterValue(FName("CycloramaColor"), VColorS);
			VCycloramaSMIL->SetVectorParameterValue(FName("CycloramaColor"), VColorS);
		}
		VCycloramaSMIR->SetScalarParameterValue(FName("Reflection"), VReflection);
		VCycloramaSMIL->SetScalarParameterValue(FName("Reflection"), VReflection);

		VCycloramaSMIR->SetScalarParameterValue(FName("Roughness"), VRoughness);
		VCycloramaSMIL->SetScalarParameterValue(FName("Roughness"), VRoughness);



		Cyclorama_Fog->SetVisibility(VFog);
		Cyclorama_Fog->SetFogDensity(VFogDensity);

		UpdateCycloramaBP();
	}
	else {
		this->SetActorHiddenInGame(true);
	}
}