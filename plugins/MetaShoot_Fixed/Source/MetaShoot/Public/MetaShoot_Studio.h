// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MetaShoot_Manifest.h"

#include "MetaShoot_Studio.generated.h"

UCLASS()
class METASHOOT_API AMetaShoot_Studio : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_Studio();

	UPROPERTY(BlueprintReadOnly, Category = "Studio")
		bool BMSAssetActive = true;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY()
		TSubclassOf<AMetaShoot_Cyclorama> VCyclorama;

};
