// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MetaShoot_DragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class METASHOOTEDITOR_API UMetaShoot_DragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "DragDrop")
		void GetMouseLocationWorld();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DragDrop")
		FVector viewPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DragDrop")
		FRotator viewRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DragDrop")
		FVector spawnLoc;
	
};
