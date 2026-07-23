// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_360Camera.h"

#include "Components/StaticMeshComponent.h"

//CameraMesh02 = CreateDefaultSubobject<UStaticMeshComponent>(FName("CameraMesh02"));

void AMetaShoot_360Camera::OnConstruction(const FTransform& Transform)
{
	Update360Cam();
}