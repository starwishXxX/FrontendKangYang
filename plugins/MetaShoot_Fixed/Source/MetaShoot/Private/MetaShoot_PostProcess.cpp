// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_PostProcess.h"

// Sets default values
AMetaShoot_PostProcess::AMetaShoot_PostProcess()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(FName("Post Process"));

}

// Called when the game starts or when spawned
void AMetaShoot_PostProcess::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaShoot_PostProcess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

