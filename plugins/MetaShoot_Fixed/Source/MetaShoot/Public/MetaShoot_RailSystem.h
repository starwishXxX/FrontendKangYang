// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MetaShoot_Manifest.h"

#include "MetaShoot_RailSystem.generated.h"


UCLASS()
class METASHOOT_API AMetaShoot_RailSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_RailSystem();

	//Settings
	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		bool BMSAssetActive = true;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rail System", meta = (DisplayName = "Beam Separation", DisplayPriority = 0, ClampMin = "100.0", ClampMax = "500.0", UIMin = "100.0", UIMax = "500.0"))
		float VSeparation = 375;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rail System", meta = (DisplayName = "Beam Length", DisplayPriority = 1, ClampMin = "500.0", ClampMax = "1500.0", UIMin = "500.0", UIMax = "1500.0"))
		float VLength = 1000;

	//Components

	UPROPERTY()
		USceneComponent* RailSystem_Root;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		UStaticMeshComponent* Beam00;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		UStaticMeshComponent* Beam01;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		UInstancedStaticMeshComponent* BeamRail;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		UInstancedStaticMeshComponent* BeamRunner;



	//Variables

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		TArray<AMetaShoot_SoftboxMaster*> LightsArray;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		TArray<FVector> LightsCoordinatesArray;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		TArray<FVector> RailLocationArray;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		TArray<FRotator> RailRotationArray;

	UPROPERTY(BlueprintReadOnly, Category = "Rail System")
		int32 VTotalRails = 0;


	//Functions

	UFUNCTION(BlueprintCallable, Category = "Rail System")
		void UpdateRails();

	UFUNCTION(BlueprintCallable, Category = "Rail System")
		void UpdateLightsScissors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
