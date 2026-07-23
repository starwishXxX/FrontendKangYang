// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_DSLRCamera.h"

#include "Components/StaticMeshComponent.h"

//CameraMesh02 = CreateDefaultSubobject<UStaticMeshComponent>(FName("CameraMesh02"));

void AMetaShoot_DSLRCamera::OnConstruction(const FTransform& Transform)
{
	if (BMSAssetActive) {
		UpdateDSLR();
	}
}

void AMetaShoot_DSLRCamera::DefineFocusTarget()
{
	//this->focus
}



//void AMetaShoot_DSLRCamera::FixDepthOfField()
//{
//	/*FString VCommand = "r.TemporalAA.Upsampling 0";
//	GetWorld()->Exec(GetWorld(), *VCommand);*/
//
//	AMetaShoot_DSLRCamera::FixDOFBP();
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, "DOF Fixed - r.TemporalAA.Upsampling 0. Credit to William Faucher");
//	
//}

//#if WITH_EDITOR
//void AMetaShoot_DSLRCamera::PostEditChangeProperty(struct FPropertyChangedEvent& e)
//{
//	Super::PostEditChangeProperty(e);
//
//	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
//
//	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMetaShoot_DSLRCamera, VCameraMeshHidden)) {
//		CameraMeshHide();
//	}
//
//}
//#endif