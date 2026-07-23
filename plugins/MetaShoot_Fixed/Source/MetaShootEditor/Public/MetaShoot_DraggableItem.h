// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MetaShoot_DraggableItem.generated.h"

/**
 * 
 */
UCLASS()
class METASHOOTEDITOR_API UMetaShoot_DraggableItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Draggable")
		void GetMouseLocationWorld();

	UFUNCTION(BlueprintImplementableEvent, Category = "Draggable")
		void GetMouseTimerFunction();

	UFUNCTION(BlueprintCallable, Category = "Draggable")
		void GetMouseTimerFunctionBP();

	UFUNCTION(BlueprintCallable, Category = "Draggable")
		void SelectSpawnedActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Draggable")
		FVector viewPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Draggable")
		FRotator viewRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Draggable")
		FVector spawnLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Draggable")
		FVector2D mousePosVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Draggable")
		FVector mouseDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Draggable")
		AActor* SpawnedActor;
	
};
