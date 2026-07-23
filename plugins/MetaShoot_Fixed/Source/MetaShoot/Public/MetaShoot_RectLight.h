// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/RectLightComponent.h"

#include "MetaShoot_RectLight.generated.h"

UCLASS()
class METASHOOT_API AMetaShoot_RectLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_RectLight();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Light", meta=(ExposeFunctionCategories="RectLight,Rendering|Lighting"))
		URectLightComponent* RectLightComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light")
		AActor* TargetActor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Light")
		UStaticMeshComponent* LightPlane;

	UPROPERTY(BlueprintReadOnly, Category = "Light")
		UMaterialInstanceDynamic* MILight;

	UPROPERTY(BlueprintReadWrite, Category = "Light")
		UMaterialInterface* TheMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Light")
		UTexture* LightPlaneTexture;

	UFUNCTION(BlueprintCallable, Category = "Light")
		void LookAtTargetBP();

	UFUNCTION(BlueprintCallable, Category = "Light")
		void UpdateLightPlaneBP();


	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MyBoolVar;
	*/
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void LookAtTarget();
	
private:
	

};
