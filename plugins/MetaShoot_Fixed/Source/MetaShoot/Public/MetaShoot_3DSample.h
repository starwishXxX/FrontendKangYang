// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetaShoot_3DSample.generated.h"

UCLASS()
class METASHOOT_API AMetaShoot_3DSample : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_3DSample();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY()
		USceneComponent* SampleRoot;

	UPROPERTY(BlueprintReadWrite, Category = "3DSample")
		UStaticMeshComponent* SphereL;

	UPROPERTY(BlueprintReadWrite, Category = "3DSample")
		UStaticMeshComponent* SphereR;

	UPROPERTY(BlueprintReadWrite, Category = "3DSample")
		UStaticMeshComponent* SphereC;

	UPROPERTY(BlueprintReadWrite, Category = "3DSample")
		UStaticMeshComponent* Infinite;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "3D Sample", meta = (DisplayName = "Double Sample", DisplayPriority = 1))
		bool VDouble = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "3D Sample", meta = (DisplayName = "Infinite Shape", DisplayPriority = 2))
		bool VInfinite = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "3D Sample", meta = (DisplayName = "Infinite Colour", DisplayPriority = 3, EditCondition = "VInfinite == true"))
		bool VInfiniteLoop = false;


	UFUNCTION(BlueprintCallable, Category = "3DSample")
		void UpdateSample();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
