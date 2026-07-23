// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_RailSystem.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMetaShoot_RailSystem::AMetaShoot_RailSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components

	RailSystem_Root = CreateDefaultSubobject<USceneComponent>(FName("Rail System Root"));
	this->SetRootComponent(RailSystem_Root);

	Beam00 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Beam 0"));
	Beam00->SetupAttachment(RailSystem_Root);

	Beam01 = CreateDefaultSubobject<UStaticMeshComponent>(FName("Beam 1"));
	Beam01->SetupAttachment(RailSystem_Root);
	Beam01->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	BeamRail = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Beam Rail"));
	BeamRail->SetupAttachment(RailSystem_Root);

	BeamRunner = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Beam Runner"));
	BeamRunner->SetupAttachment(RailSystem_Root);

}

// Called when the game starts or when spawned
void AMetaShoot_RailSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaShoot_RailSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMetaShoot_RailSystem::UpdateRails();

}

void AMetaShoot_RailSystem::OnConstruction(const FTransform& Transform)
{
	AMetaShoot_RailSystem::UpdateRails();

	AMetaShoot_RailSystem::UpdateLightsScissors();
}

void AMetaShoot_RailSystem::UpdateRails()
{
	Beam00->SetRelativeLocation(FVector(VSeparation / 2, 0.0f, 0.0f));
	Beam01->SetRelativeLocation(FVector(-VSeparation / 2, 0.0f, 0.0f));

	float RailSystemScale = this->GetActorScale3D().X;

	Beam00->SetWorldScale3D(FVector(1.0f, VLength / 1000, 1.0f) * RailSystemScale);
	Beam01->SetWorldScale3D(FVector(1.0f, VLength / 1000, 1.0f) * RailSystemScale);

	float BeamZ = this->GetActorLocation().Z;
	float RailZ = this->GetActorLocation().Z - (21.5 * RailSystemScale);

	FPlane VCenterPlane = UKismetMathLibrary::MakePlaneFromPointAndNormal(this->GetActorLocation(), UKismetMathLibrary::GetForwardVector(this->GetActorRotation()));
	FPlane VBeam0Plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(Beam00->GetComponentLocation(), UKismetMathLibrary::GetForwardVector(this->GetActorRotation()));
	FPlane VBeam1Plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(Beam01->GetComponentLocation(), UKismetMathLibrary::GetForwardVector(this->GetActorRotation()));

	//UKismetSystemLibrary::DrawDebugPlane(GetWorld(), VBeam0Plane, this->GetActorLocation(), 200, FColor::Orange, 5.0f);
	//UKismetSystemLibrary::DrawDebugPlane(GetWorld(), VBeam1Plane, this->GetActorLocation(), 200, FColor::Blue, 5.0f);

	LightsArray.Empty();
	LightsCoordinatesArray.Empty();
	RailLocationArray.Empty();
	RailRotationArray.Empty();
	BeamRail->ClearInstances();
	BeamRunner->ClearInstances();
	VTotalRails = 0;

	//FPlane RailZPlane = UKismetMathLibrary::MakePlaneFromPointAndNormal(FVector(this->GetActorLocation().X, this->GetActorLocation().Y, RailZ), UKismetMathLibrary::GetUpVector(this->GetActorRotation()));
	//UKismetSystemLibrary::DrawDebugPlane(GetWorld(), RailZPlane, FVector(this->GetActorLocation().X, this->GetActorLocation().Y, RailZ), 200, FColor::Orange, 5.0f);

	//Get Softboxes under rail system
	for (TActorIterator<AMetaShoot_SoftboxMaster> It(GetWorld(), AMetaShoot_SoftboxMaster::StaticClass()); It; ++It)
	{
		AMetaShoot_SoftboxMaster* actor = *It;
		if (actor != NULL && actor->VLightSupport != EVLightSupportMS::Tripod)
		{
			//actor->ScissorsHinge->GetComponentLocation();

			actor->UpdateSupportTrace();

			if (actor->ScissorsHinge->GetComponentLocation().Z < RailZ)
			{
				if (FMath::Abs(UKismetMathLibrary::InverseTransformLocation(this->GetActorTransform(), actor->ScissorsHinge->GetComponentLocation()).X) < VSeparation / 2 + 40)
				{
					if (FMath::Abs(UKismetMathLibrary::InverseTransformLocation(this->GetActorTransform(), actor->ScissorsHinge->GetComponentLocation()).Y) < VLength / 2)
					{
						if (actor->VLightSupport == EVLightSupportMS::Scissors)
						{
							LightsArray.Add(actor);
							LightsCoordinatesArray.Add(actor->ScissorsHinge->GetComponentLocation());
						}
						if (actor->VLightSupport == EVLightSupportMS::Automatic && RailZ - actor->ScissorsHinge->GetComponentLocation().Z < actor->TripodLength)
						{
							LightsArray.Add(actor);
							LightsCoordinatesArray.Add(actor->ScissorsHinge->GetComponentLocation());
						}
					}
				}
			}
		}
	}

	//Detect softboxes sharing one rail
	for (int32 i1 = 0; i1 < LightsCoordinatesArray.Num(); i1++)
	{
		FVector VCoordinate1 = LightsCoordinatesArray.GetData()[i1];
		for (int32 i2 = 0; i2 < LightsCoordinatesArray.Num(); i2++)
		{
			if (VCoordinate1 == LightsCoordinatesArray.GetData()[i1])
			{
				FVector VCoordinate2 = LightsCoordinatesArray.GetData()[i2];

				FVector VCoordinate1Relative = UKismetMathLibrary::InverseTransformLocation(this->GetActorTransform(), VCoordinate1);
				FVector VCoordinate2Relative = UKismetMathLibrary::InverseTransformLocation(this->GetActorTransform(), VCoordinate2);

				float VRatioCoordinates = FMath::Abs((VCoordinate1Relative.X - VCoordinate2Relative.X) / (VCoordinate1Relative.Y - VCoordinate2Relative.Y));
				if (VRatioCoordinates > 2)
				{
					LightsCoordinatesArray.Remove(VCoordinate1);
					LightsCoordinatesArray.Remove(VCoordinate2);
					VTotalRails += 1;

					FVector VStart = VCoordinate1 - (UKismetMathLibrary::GetDirectionUnitVector(VCoordinate1, VCoordinate2) * 5000);
					FVector VEnd = VCoordinate2 + (UKismetMathLibrary::GetDirectionUnitVector(VCoordinate1, VCoordinate2) * 5000);

					FRotator VRailRotation = UKismetMathLibrary::FindLookAtRotation(VCoordinate1, VCoordinate2);

					RailLocationArray.Add(FMath::LinePlaneIntersection(VStart, VEnd, VCenterPlane) * FVector(1.0f, 1.0f, 0.0f) + FVector(0.0f, 0.0f, RailZ));
					RailRotationArray.Add(FRotator(0.0f, VRailRotation.Yaw, 0.0f));
				}
			}
		}
	}

	//Normal straight rails
	for (int32 i1 = 0; i1 < LightsCoordinatesArray.Num(); i1++)
	{
		VTotalRails += 1;

		RailLocationArray.Add(UKismetMathLibrary::ProjectPointOnToPlane(LightsCoordinatesArray.GetData()[i1], this->GetActorLocation(), UKismetMathLibrary::GetForwardVector(this->GetActorRotation())) * FVector(1.0f, 1.0f, 0.0f) + FVector(0.0f, 0.0f, RailZ));
		RailRotationArray.Add(this->GetActorRotation());
	}

	//Update Rails
	for (int32 i1 = 0; i1 < VTotalRails; i1++)
	{
		if (BeamRail->GetInstanceCount() < VTotalRails)
		{
			BeamRail->AddInstance(FTransform(RailRotationArray.GetData()[i1], RailLocationArray.GetData()[i1], this->GetActorScale3D()));
		}
		if (BeamRail->GetInstanceCount() > VTotalRails)
		{
			BeamRail->RemoveInstance(BeamRail->GetInstanceCount() - 1);
		}

		BeamRail->UpdateInstanceTransform(i1, FTransform(RailRotationArray.GetData()[i1], RailLocationArray.GetData()[i1], this->GetActorScale3D()), true);
	}

	//Update Runners
	for (int32 i1 = 0; i1 < BeamRail->GetInstanceCount(); i1++)
	{
		FTransform VBeamTransform;
		BeamRail->GetInstanceTransform(i1, VBeamTransform, true);
		FVector VBeamLocation = VBeamTransform.GetLocation();
		FRotator VBeamRotation = VBeamTransform.GetRotation().Rotator();
		FVector VBeamDirection = VBeamLocation + UKismetMathLibrary::GetForwardVector(VBeamRotation) * 2000;
		FVector VBeamRunnerLocation1 = FMath::LinePlaneIntersection(VBeamLocation, VBeamDirection, VBeam0Plane) + FVector(0.0f, 0.0f, 21.5 * RailSystemScale);
		FVector VBeamRunnerLocation2 = FMath::LinePlaneIntersection(VBeamLocation, VBeamDirection, VBeam1Plane) + FVector(0.0f, 0.0f, 21.5 * RailSystemScale);

		if (BeamRunner->GetInstanceCount() / 2 < BeamRail->GetInstanceCount())
		{
			BeamRunner->AddInstance(FTransform(this->GetActorRotation(), VBeamRunnerLocation1, this->GetActorScale3D()));
			BeamRunner->AddInstance(FTransform(this->GetActorRotation(), VBeamRunnerLocation2, this->GetActorScale3D()));
		}
		if (BeamRunner->GetInstanceCount() / 2 > BeamRail->GetInstanceCount())
		{
			BeamRunner->RemoveInstance(BeamRunner->GetInstanceCount() - 1);
			BeamRunner->RemoveInstance(BeamRunner->GetInstanceCount() - 1);
		}

		BeamRunner->UpdateInstanceTransform(i1*2, FTransform(this->GetActorRotation(), VBeamRunnerLocation1, this->GetActorScale3D()), true);
		BeamRunner->UpdateInstanceTransform(i1*2+1, FTransform(this->GetActorRotation(), VBeamRunnerLocation2, this->GetActorScale3D()), true);

	}
}

void AMetaShoot_RailSystem::UpdateLightsScissors()
{
	for (int i = 0; i < LightsArray.Num(); i++)
	{
		LightsArray[i]->UpdateSupportTrace();
		LightsArray[i]->UpdateSupport();
	}

	/*for (TActorIterator<AMetaShoot_SoftboxMaster> It(GetWorld(), AMetaShoot_SoftboxMaster::StaticClass()); It; ++It)
	{
		AMetaShoot_SoftboxMaster* actor = *It;
		actor->UpdateSupport();
	}*/
}