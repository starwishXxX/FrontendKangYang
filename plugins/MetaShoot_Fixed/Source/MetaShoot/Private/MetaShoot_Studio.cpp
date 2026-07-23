// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_Studio.h"

// Sets default values
AMetaShoot_Studio::AMetaShoot_Studio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMetaShoot_Studio::BeginPlay()
{
	Super::BeginPlay();
	
	/*const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	GetWorld()->SpawnActor<AMetaShoot_Cyclorama>(VCyclorama, Location, Rotation);*/
}

// Called every frame
void AMetaShoot_Studio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMetaShoot_Studio::OnConstruction(const FTransform& Transform)
{
	
}