// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_3DSample.h"

// Sets default values
AMetaShoot_3DSample::AMetaShoot_3DSample()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SampleRoot = CreateDefaultSubobject<USceneComponent>(FName("3D Sample Root"));
	this->SetRootComponent(SampleRoot);

	SphereL = CreateDefaultSubobject<UStaticMeshComponent>(FName("SphereL"));
	SphereL->SetupAttachment(SampleRoot);
	SphereL->SetRelativeLocation(FVector(0.0f, -26.25f, 0.0f));
	SphereL->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	SphereR = CreateDefaultSubobject<UStaticMeshComponent>(FName("SphereR"));
	SphereR->SetupAttachment(SampleRoot);
	SphereR->SetRelativeLocation(FVector(0.0f, 26.25f, 0.0f));

	SphereC = CreateDefaultSubobject<UStaticMeshComponent>(FName("SphereC"));
	SphereC->SetupAttachment(SampleRoot);
	SphereC->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	SphereC->SetVisibility(false);

	Infinite = CreateDefaultSubobject<UStaticMeshComponent>(FName("Infinite"));
	Infinite->SetupAttachment(SampleRoot);
	Infinite->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

void AMetaShoot_3DSample::OnConstruction(const FTransform& Transform)
{
	//AMetaShoot_3DSample::UpdateSample();

}

// Called when the game starts or when spawned
void AMetaShoot_3DSample::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaShoot_3DSample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMetaShoot_3DSample::UpdateSample()
{
	if (VDouble)
	{
		SphereR->SetVisibility(true);
		SphereL->SetVisibility(true);
		SphereC->SetVisibility(false);
	}
	else
	{
		SphereR->SetVisibility(false);
		SphereL->SetVisibility(false);
		SphereC->SetVisibility(true);
	}

	if (VInfinite)
	{
		Infinite->SetVisibility(true);
	}
	else
	{
		Infinite->SetVisibility(false);
	}
}

#if WITH_EDITOR
void AMetaShoot_3DSample::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMetaShoot_3DSample, VInfiniteLoop)) {
		if (VInfiniteLoop)
		{
			Infinite->SetMaterial(0, Infinite->GetMaterial(2));
		}
		else
		{
			Infinite->SetMaterial(0, Infinite->GetMaterial(1));
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMetaShoot_3DSample, VDouble)) {
		if (VDouble)
		{
			SphereR->SetVisibility(true);
			SphereL->SetVisibility(true);
			SphereC->SetVisibility(false);
		}
		else
		{
			SphereR->SetVisibility(false);
			SphereL->SetVisibility(false);
			SphereC->SetVisibility(true);
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMetaShoot_3DSample, VInfinite)) {
		if (VInfinite)
		{
			Infinite->SetVisibility(true);
		}
		else
		{
			Infinite->SetVisibility(false);
		}
	}


}
#endif