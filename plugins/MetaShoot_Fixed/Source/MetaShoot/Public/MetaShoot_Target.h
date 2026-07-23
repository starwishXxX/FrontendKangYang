// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Components/MaterialBillboardComponent.h"

#include "MetaShoot_Manifest.h"

#include "MetaShoot_Target.generated.h"

UCLASS()
class METASHOOT_API AMetaShoot_Target : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaShoot_Target();

	UPROPERTY(BlueprintReadOnly, Category = "Target Tracker")
		bool BMSAssetActive = true;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/*UPROPERTY()
		USceneComponent* TargetActorRoot;*/

	UPROPERTY()
		UBillboardComponent* TargetActorBillboard;

	UPROPERTY(BlueprintReadWrite, Category = "Target Tracker")
		UMaterialBillboardComponent* TargetActorMaterialBillboard;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Target Tracker", meta = (DisplayName = "Tint", DisplayPriority = 0))
		FLinearColor VTint = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Tracker", meta = (DisplayName = "Move to Actor", DisplayPriority = 1))
		AActor* ActorToMove;

	UFUNCTION()
		void MoveToActor();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateLights();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//virtual void PostEditMove();

	//virtual void BroadcastOnActorMoving();

};