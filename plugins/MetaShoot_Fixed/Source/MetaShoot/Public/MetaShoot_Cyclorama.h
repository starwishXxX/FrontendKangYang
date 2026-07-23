// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


#include "MetaShoot_Cyclorama.generated.h"

UCLASS()
class METASHOOT_API AMetaShoot_Cyclorama : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_Cyclorama();

	//Variables

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		bool BMSAssetActive = true;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Width", DisplayPriority = 0, ClampMin = "150.0", ClampMax = "1500.0", UIMin = "150.0", UIMax = "1500.0"))
		float VWidth = 1000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Center Color", DisplayPriority = 1))
		FLinearColor VColorC = FLinearColor(0.8f, 0.8f, 0.8f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Sides Color", DisplayPriority = 2, EditCondition = "!VLockColorS"))
		FLinearColor VColorS = FLinearColor(0.05f, 0.05f, 0.05f);;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Single Color", DisplayPriority = 3))
		bool VLockColorS = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Unlit", DisplayPriority = 4))
		bool VUnlit = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Reflection", DisplayPriority = 5, ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float VReflection = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Roughness", DisplayPriority = 6, ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float VRoughness = 0.5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Fog", DisplayPriority = 7))
		bool VFog;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Cyclorama", meta = (DisplayName = "Fog Density", DisplayPriority = 8, ClampMin = "0.0", ClampMax = "5.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "VFog"))
		float VFogDensity = 0.2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cyclorama")
		float XOffset = 216.0;

	//Components

	UPROPERTY()
		USceneComponent* Cyclorama_Root;

	UPROPERTY(BlueprintReadWrite, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_C;

	UPROPERTY(BlueprintReadWrite, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_R;

	UPROPERTY(BlueprintReadWrite, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_L;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame00;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame01;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame02;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame03;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame04;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame05;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame06;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UStaticMeshComponent* Cyclorama_Frame07;

	/*UPROPERTY(BlueprintReadOnly)
		UDecalComponent* Cyclorama_Decal;*/

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UExponentialHeightFogComponent* Cyclorama_Fog;

	//Material Instances

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UMaterialInstanceDynamic* VCycloramaCMI;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UMaterialInstanceDynamic* VCycloramaSMIR;

	UPROPERTY(BlueprintReadOnly, Category = "Cyclorama")
		UMaterialInstanceDynamic* VCycloramaSMIL;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "Cyclorama")
		void UpdateCyclorama();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Cyclorama")
		void UpdateCycloramaBP();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
