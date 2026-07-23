// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/PostProcessComponent.h"

#include "MetaShoot_PostProcess.generated.h"

UCLASS()
class METASHOOT_API AMetaShoot_PostProcess : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_PostProcess();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PostProcess")
		UPostProcessComponent* PostProcess;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
