// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_SoftboxMaster.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

// Sets default values
AMetaShoot_SoftboxMaster::AMetaShoot_SoftboxMaster()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RectLightComponent = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight"));
	this->SetRootComponent(RectLightComponent);
	RectLightComponent->SetAttenuationRadius(1000);

	LightDiffuser = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightDiffuser"));
	LightDiffuser->SetupAttachment(RectLightComponent);

	LightBox = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightBox"));
	LightBox->SetupAttachment(LightDiffuser);

	LightGrid = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightBoxGrid"));
	LightGrid->SetupAttachment(LightBox);

	LightBoxFrame = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightBoxFrame"));
	LightBoxFrame->SetupAttachment(LightBox);

	LightBulb = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightBulb"));
	LightBulb->SetupAttachment(LightBox);
	LightBulb->SetCastShadow(false);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("PointLight"));
	PointLightComponent->SetupAttachment(LightBulb);
	PointLightComponent->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(FName("SpotLight"));
	SpotLightComponent->SetupAttachment(LightBox);
	SpotLightComponent->SetRelativeLocation(FVector(12.5f, 0.0f, 0.0f));

	//Tripod Meshes
	LightBodyFloor = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightBodyFloor"));
	LightBodyFloor->SetupAttachment(RectLightComponent);
	TripodHinge = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodHinge"));
	TripodHinge->SetupAttachment(LightBodyFloor);
	TripodPipe02 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodPipe02"));
	TripodPipe02->SetupAttachment(TripodHinge);
	TripodLock01 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodLock01"));
	TripodLock01->SetupAttachment(TripodHinge);
	TripodPipe01 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodPipe01"));
	TripodPipe01->SetupAttachment(TripodLock01);
	TripodLock00 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodLock00"));
	TripodLock00->SetupAttachment(TripodHinge);
	TripodPipe00 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodPipe00"));
	TripodPipe00->SetupAttachment(TripodLock00);
	TripodBase = CreateDefaultSubobject<UStaticMeshComponent>(FName("TripodBase"));
	TripodBase->SetupAttachment(TripodHinge);

	//Scissors Meshes
	LightBodyCeiling = CreateDefaultSubobject<UStaticMeshComponent>(FName("LightBodyCeiling"));
	LightBodyCeiling->SetupAttachment(RectLightComponent);
	ScissorsHinge = CreateDefaultSubobject<UStaticMeshComponent>(FName("ScissorsHinge"));
	ScissorsHinge->SetupAttachment(LightBodyCeiling);
	ScissorsPipe = CreateDefaultSubobject<UStaticMeshComponent>(FName("ScissorsPipe"));
	ScissorsPipe->SetupAttachment(ScissorsHinge);

	ScissorsSegment00 = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("ScissorsSegment00"));
	ScissorsSegment00->SetupAttachment(ScissorsHinge);
	ScissorsSegment01 = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("ScissorsSegment01"));
	ScissorsSegment01->SetupAttachment(ScissorsHinge);
	ScissorsSegment02 = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("ScissorsSegment02"));
	ScissorsSegment02->SetupAttachment(ScissorsHinge);

	ScissorsSegment02->ClearInstances();
	ScissorsSegment01->ClearInstances();
	ScissorsSegment00->ClearInstances();

	ScissorsSegment02->AddInstance(FTransform());
	ScissorsSegment02->AddInstance(FTransform());
	ScissorsSegment02->AddInstance(FTransform());
	ScissorsSegment02->AddInstance(FTransform());

	ScissorsSegment01->AddInstance(FTransform());
	ScissorsSegment01->AddInstance(FTransform());
	ScissorsSegment01->AddInstance(FTransform());
	ScissorsSegment01->AddInstance(FTransform());

	for (int i = 0; i < VLightScissorsSegments - 2; i++)
	{
		ScissorsSegment00->AddInstance(FTransform());
		ScissorsSegment00->AddInstance(FTransform());
	}

			
	AMetaShoot_SoftboxMaster::LookAtTarget();
}


// Called when the game starts or when spawned
void AMetaShoot_SoftboxMaster::BeginPlay()
{
	Super::BeginPlay();	
}

void AMetaShoot_SoftboxMaster::UpdateAll(bool UpdateRail)
{
	if (BMSAssetActive) {
		this->SetActorHiddenInGame(false);

		AMetaShoot_SoftboxMaster::LookAtTarget();

		AMetaShoot_SoftboxMaster::UpdateLight();

		AMetaShoot_SoftboxMaster::UpdateLightMeshBP();

		AMetaShoot_SoftboxMaster::UpdateLightColor();

		AMetaShoot_SoftboxMaster::UpdateDiffuser();

		AMetaShoot_SoftboxMaster::UpdateSupportTrace();

		if (UpdateRail) {
			for (TActorIterator<AMetaShoot_RailSystem> It(GetWorld(), AMetaShoot_RailSystem::StaticClass()); It; ++It)
			{
				AMetaShoot_RailSystem* actor = *It;
				actor->UpdateRails();
			}

			AMetaShoot_SoftboxMaster::UpdateSupportTrace();
		}

		AMetaShoot_SoftboxMaster::UpdateSupport();
	}
	else {
		this->SetActorHiddenInGame(true);
	}
}

// Called every frame
void AMetaShoot_SoftboxMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AMetaShoot_SoftboxMaster::UpdateAll(false);
}




void AMetaShoot_SoftboxMaster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AMetaShoot_SoftboxMaster::UpdateAll(true);
}

//#if WITH_EDITOR
//void AMetaShoot_SoftboxMaster::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	AMetaShoot_SoftboxMaster::LookAtTarget();
//
//	AMetaShoot_SoftboxMaster::UpdateLight();
//
//	AMetaShoot_SoftboxMaster::UpdateLightMeshBP();
//
//	AMetaShoot_SoftboxMaster::UpdateLightColor();
//
//	AMetaShoot_SoftboxMaster::UpdateDiffuser();
//
//	AMetaShoot_SoftboxMaster::UpdateSupportTrace();
//
//	for (TActorIterator<AMetaShoot_RailSystem> It(GetWorld(), AMetaShoot_RailSystem::StaticClass()); It; ++It)
//	{
//		AMetaShoot_RailSystem* actor = *It;
//		actor->UpdateRails();
//	}
//
//	AMetaShoot_SoftboxMaster::UpdateSupportTrace();
//
//	AMetaShoot_SoftboxMaster::UpdateSupport();
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, TEXT("Updating"));
//}
//#endif


void AMetaShoot_SoftboxMaster::LookAtTarget()
{
	if (IsValid(VActorToTrack))
	{
		this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), VActorToTrack->GetActorLocation()));
		if (VLightType != EVLightTypeMS::Spotlight && VLightType != EVLightTypeMS::RingLight && VLightType != EVLightTypeMS::LightWand)
		{
			this->AddActorLocalRotation(FRotator(0.0f, 0.0f, VLightRotation));
		}
		
	}
}




void AMetaShoot_SoftboxMaster::UpdateLight()
{

	LightScale = this->GetActorScale().X;

	RectLightComponent->SetIntensity(VLightIntensity);
	RectLightComponent->SetTemperature(VLightTemperature);
	RectLightComponent->SetUseTemperature(VLightTemperatureBool);
	RectLightComponent->SetLightColor(VLightTint);

	PointLightComponent->SetIntensity(VLightIntensity / 2);
	PointLightComponent->SetTemperature(VLightTemperature);
	PointLightComponent->SetUseTemperature(VLightTemperatureBool);
	PointLightComponent->SetLightColor(VLightTint);

	SpotLightComponent->SetIntensity(VLightIntensity);
	SpotLightComponent->SetTemperature(VLightTemperature);
	SpotLightComponent->SetUseTemperature(VLightTemperatureBool);
	SpotLightComponent->SetLightColor(VLightTint);


	if (VLightUnits == EVLightUnitsMS::Unitless)
	{
		RectLightComponent->SetIntensityUnits(ELightUnits::Unitless);
		PointLightComponent->SetIntensityUnits(ELightUnits::Unitless);
		SpotLightComponent->SetIntensityUnits(ELightUnits::Unitless);
	}
	if (VLightUnits == EVLightUnitsMS::Candelas)
	{
		RectLightComponent->SetIntensityUnits(ELightUnits::Candelas);
		PointLightComponent->SetIntensityUnits(ELightUnits::Candelas);
		SpotLightComponent->SetIntensityUnits(ELightUnits::Candelas);
	}
	if (VLightUnits == EVLightUnitsMS::Lumens)
	{
		RectLightComponent->SetIntensityUnits(ELightUnits::Lumens);
		PointLightComponent->SetIntensityUnits(ELightUnits::Lumens);
		SpotLightComponent->SetIntensityUnits(ELightUnits::Lumens);
	}


	//Default Settings

	RectLightComponent->SetVisibility(VLightDifuserBool);
	RectLightComponent->SetBarnDoorAngle(45);
	RectLightComponent->SetBarnDoorLength(20 * LightScale);
	RectLightComponent->SetAttenuationRadius(1000 * LightScale);

	PointLightComponent->SetVisibility(!VLightDifuserBool);
	PointLightComponent->SetAttenuationRadius(1000);
	PointLightComponent->SetSourceRadius(3 * LightScale);
	LightBulb->SetVisibility(!VLightDifuserBool);
	LightBulb->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpotLightComponent->SetAttenuationRadius(0);
	SpotLightComponent->SetVisibility(false);
	SpotLightComponent->SetSourceRadius(0);
	SpotLightComponent->SetOuterConeAngle(25);
	SpotLightComponent->SetInnerConeAngle(10);

	LightDiffuser->SetVisibility(VLightDifuserBool);
	LightDiffuser->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LightDiffuser->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	LightGrid->SetVisibility(VLightGridBool && VLightDifuserBool);

	LightBodyFloor->SetRelativeLocation(FVector(-40.0f, 0.0f, 0.0f));
	LightBodyCeiling->SetRelativeLocation(FVector(-40.0f, 0.0f, 0.0f));

	TripodHinge->SetRelativeLocation(FVector(0.0f, 0.0f, -5.5f));
	ScissorsHinge->SetRelativeLocation(FVector(0.0f, 0.0f, -5.5f));

	TripodBase->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	TripodPipe02->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ScissorsPipe->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ScissorsPipe->SetVisibility(true);

	if (VLightGridBool && VLightType != EVLightTypeMS::RingLight)
	{
		RectLightComponent->SetBarnDoorAngle(0);
		RectLightComponent->SetBarnDoorLength(0);
	}


	//Light Type Settings

	if (VLightType == EVLightTypeMS::Softbox_60x60)
	{
		VLightTypeIndex = 0;
		RectLightComponent->SetSourceWidth(60 * LightScale);
		RectLightComponent->SetSourceHeight(60 * LightScale);
	}
	if (VLightType == EVLightTypeMS::Softbox_50x70)
	{
		VLightTypeIndex = 1;
		RectLightComponent->SetSourceWidth(50 * LightScale);
		RectLightComponent->SetSourceHeight(70 * LightScale);
	}
	if (VLightType == EVLightTypeMS::Softbox_30x120)
	{
		VLightTypeIndex = 2;
		RectLightComponent->SetSourceWidth(30 * LightScale);
		RectLightComponent->SetSourceHeight(120 * LightScale);
	}
	if (VLightType == EVLightTypeMS::Softbox_70x70_Octagon)
	{
		VLightTypeIndex = 3;
		RectLightComponent->SetSourceWidth(70 * LightScale);
		RectLightComponent->SetSourceHeight(70 * LightScale);
	}
	if (VLightType == EVLightTypeMS::Spotlight)
	{
		VLightTypeIndex = 4;

		RectLightComponent->SetVisibility(false);
		RectLightComponent->SetSourceWidth(0);
		RectLightComponent->SetSourceHeight(0);
		RectLightComponent->SetBarnDoorLength(0);
		RectLightComponent->SetAttenuationRadius(0);

		PointLightComponent->SetVisibility(false);
		PointLightComponent->SetAttenuationRadius(0);
		PointLightComponent->SetSourceRadius(0);

		SpotLightComponent->SetVisibility(true);
		SpotLightComponent->SetAttenuationRadius(1000);
		SpotLightComponent->SetSourceRadius(5 * LightScale);

		LightDiffuser->SetVisibility(true);
		LightDiffuser->SetRelativeLocation(FVector(-12.5f, 0.0f, 0.0f));
		LightBulb->SetVisibility(false);

		LightBodyFloor->SetRelativeLocation(FVector(-12.5f, 0.0f, 0.0f));
		LightBodyCeiling->SetRelativeLocation(FVector(-12.5f, 0.0f, 0.0f));

		TripodHinge->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ScissorsHinge->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		//TripodBase->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		TripodPipe02->SetRelativeLocation(FVector(0.0f, 0.0f, -12.5f));
		ScissorsPipe->SetVisibility(false);
	}
	if (VLightType == EVLightTypeMS::RingLight)
	{
		VLightTypeIndex = 5;

		RectLightComponent->SetVisibility(true);
		RectLightComponent->SetSourceWidth(47 * LightScale);
		RectLightComponent->SetSourceHeight(47 * LightScale);

		PointLightComponent->SetVisibility(false);
		PointLightComponent->SetAttenuationRadius(0);
		PointLightComponent->SetSourceRadius(0);

		LightDiffuser->SetRelativeLocation(FVector(-2.0f, 0.0f, 0.0f));
		LightDiffuser->SetVisibility(true);
		LightBulb->SetVisibility(false);

		LightBodyCeiling->SetRelativeLocation(FVector(-2.0f, 0.0f, 24.5f));
		LightBodyFloor->SetRelativeLocation(FVector(-2.0f, 0.0f, -24.5f));

		TripodHinge->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ScissorsHinge->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		TripodPipe02->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
		ScissorsPipe->SetRelativeLocation(FVector(0.0f, 0.0f, -3.0f));
	}
	if (VLightType == EVLightTypeMS::LightWand)
	{
		VLightTypeIndex = 6;

		RectLightComponent->SetVisibility(true);
		RectLightComponent->SetSourceWidth(4.3 * LightScale);
		RectLightComponent->SetSourceHeight(79.3 * LightScale);
		RectLightComponent->SetBarnDoorAngle(0);
		RectLightComponent->SetBarnDoorLength(2);

		PointLightComponent->SetVisibility(false);
		PointLightComponent->SetAttenuationRadius(0);
		PointLightComponent->SetSourceRadius(0);

		LightDiffuser->SetRelativeLocation(FVector(-1.0f, 0.0f, 0.0f));
		LightDiffuser->SetVisibility(true);
		LightBulb->SetVisibility(false);

		LightBodyCeiling->SetRelativeLocation(FVector(-1.0f, 0.0f, 43.25f));
		LightBodyFloor->SetRelativeLocation(FVector(-1.0f, 0.0f, -43.25f));

		TripodHinge->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ScissorsHinge->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		TripodPipe02->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
		ScissorsPipe->SetRelativeLocation(FVector(0.0f, 0.0f, -3.0f));
	}
}





void AMetaShoot_SoftboxMaster::UpdateLightColor()
{
	SB_RGBIntensity = RectLightComponent->Intensity / 25.5 / 10;
	if (VLightType == EVLightTypeMS::Spotlight || VLightType == EVLightTypeMS::RingLight || VLightType == EVLightTypeMS::LightWand)
	{
		SB_RGBIntensity *= 10;
	}

	//Kelvin Temperature to RGB | Credit to Tanner Helland for the base algorithm
	//Port to C++ for Unreal Engine by Jorge Valle Hurtado - byValle

	KtoRGB_Temperature = RectLightComponent->Temperature / 100;

	KtoRGB_Red = 0;
	KtoRGB_Green = 0;
	KtoRGB_Blue = 0;

	if (RectLightComponent->bUseTemperature == true)
	{
		//RED
		if (KtoRGB_Temperature <= 66)
		{
			KtoRGB_Red = 255;
		}

		else
		{
			KtoRGB_Red = 329.698727446 * pow(KtoRGB_Temperature - 60, -0.1332047592);
		}

		if (KtoRGB_Red < 0)
		{
			KtoRGB_Red = 0;
		}

		if (KtoRGB_Red > 255)
		{
			KtoRGB_Red = 255;
		}

		//GREEN
		if (KtoRGB_Temperature <= 66)
		{
			KtoRGB_Green = 99.4708025861 * log(KtoRGB_Temperature) - 161.1195681661;
		}

		else
		{
			KtoRGB_Green = 288.1221695283 * pow(KtoRGB_Temperature - 60, -0.0755148492);
		}

		if (KtoRGB_Green < 0)
		{
			KtoRGB_Green = 0;
		}
		if (KtoRGB_Green > 255)
		{
			KtoRGB_Green = 255;
		}

		//BLUE
		if (KtoRGB_Temperature >= 66)
		{
			KtoRGB_Blue = 255;
		}

		else
		{
			if (KtoRGB_Temperature <= 19)
			{
				KtoRGB_Blue = 0;
			}

			else
			{
				KtoRGB_Blue = 138.5177312231 * log(KtoRGB_Temperature - 10) - 305.0447927307;
			}
		}

		if (KtoRGB_Blue < 0)
		{
			KtoRGB_Blue = 0;
		}
		if (KtoRGB_Blue > 255)
		{
			KtoRGB_Blue = 255;
		}

		KtoRGB_Red = KtoRGB_Red / 10;
		KtoRGB_Green = KtoRGB_Green / 10;
		KtoRGB_Blue = KtoRGB_Blue / 10;
	}
	else
	{
		KtoRGB_Red = 25.5f;
		KtoRGB_Green = 25.5f;
		KtoRGB_Blue = 25.5f;
	}

	//FINAL RESULT AS FLINEARCOLOR
	KtoRGB = FLinearColor(KtoRGB_Red, KtoRGB_Green, KtoRGB_Blue, 1.0f);

	//Kelvin Temperature to RGB | Credit to Tanner Helland for the base algorithm
	//Port to C++ for Unreal Engine by Jorge Valle Hurtado - byValle
}


void AMetaShoot_SoftboxMaster::UpdateDiffuser()
{
	if (IsValid(LightDiffuser->GetMaterial(0))) {
		VLightMI = LightDiffuser->CreateDynamicMaterialInstance(0, 0, FName("DiffuserMaterial"));
	}

	if (IsValid(LightBulb->GetMaterial(0))) {
		VLightBulbMI = LightBulb->CreateDynamicMaterialInstance(0, 0, FName("BulbMaterial"));
	}

	if (IsValid(LightBox->GetMaterial(0))) {
		VBoxMI = LightBox->CreateDynamicMaterialInstance(0, 0, FName("BoxMaterial"));
	}
	

	VLightMI->SetVectorParameterValue(FName("LightIntensity_Param"), FLinearColor(SB_RGBIntensity, SB_RGBIntensity, SB_RGBIntensity, 1.0));
	VLightMI->SetVectorParameterValue(FName("LightTemperatureRGB_Param"), KtoRGB);
	VLightMI->SetVectorParameterValue(FName("LightColor_Param"), VLightTint);

	VLightBulbMI->SetVectorParameterValue(FName("LightIntensity_Param"), FLinearColor(SB_RGBIntensity, SB_RGBIntensity, SB_RGBIntensity, 1.0));
	VLightBulbMI->SetVectorParameterValue(FName("LightTemperatureRGB_Param"), KtoRGB);
	VLightBulbMI->SetVectorParameterValue(FName("LightColor_Param"), VLightTint);

	if (VLightType != EVLightTypeMS::Spotlight && VLightType != EVLightTypeMS::RingLight && VLightType != EVLightTypeMS::LightWand)
	{
		VBoxMI->SetScalarParameterValue(FName("AlphaBoolean"), VBrandingBool);
	}

	if (VLightType == EVLightTypeMS::Spotlight)
	{
		VLightMI->SetScalarParameterValue(FName("AlphaBoolean"), VBrandingBool);
	}
	

	//VBoxMI->SetVectorParameterValue(FName("LightIntensity_Param"), FLinearColor(SB_RGBIntensity, SB_RGBIntensity, SB_RGBIntensity, 1.0));

	//VBoxMI->SetScalarParameterValue(FName("AlphaBoolean"), 0.0f);
	
}

void AMetaShoot_SoftboxMaster::UpdateSupportTrace()
{
	LightScale = this->GetActorScale().X;

	FRotator LightBodyFloorRot = UKismetMathLibrary::FindLookAtRotation(LightBodyFloor->GetComponentLocation(), this->GetActorLocation());
	LightBodyFloor->SetWorldRotation(LightBodyFloorRot);

	FRotator LightBodyCeilingRot = UKismetMathLibrary::ComposeRotators(UKismetMathLibrary::FindLookAtRotation(LightBodyCeiling->GetComponentLocation(), this->GetActorLocation()), FRotator(0.0f, 0.0f, 180.0f));
	LightBodyCeiling->SetWorldRotation(FRotator(-LightBodyCeilingRot.Pitch, -LightBodyCeilingRot.Yaw, LightBodyCeilingRot.Roll));

	//Trace Params
	FCollisionQueryParams TraceParams(FName(TEXT("LineTraceParameters")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);
	for (TActorIterator<AMetaShoot_SoftboxMaster> It(GetWorld(), AMetaShoot_SoftboxMaster::StaticClass()); It; ++It)
	{
		AMetaShoot_SoftboxMaster* actor = *It;
		TraceParams.AddIgnoredActor(actor);
	}
	

	//TRIPOD

	//Trace
	HingeTripodLoc = TripodHinge->GetComponentLocation();
	StartTripod = HingeTripodLoc;
	EndTripod = HingeTripodLoc + FVector(0.0f, 0.0f, -2500.0f);

	HitDetailsTripod = FHitResult(ForceInit);

	bIsHitTripod = GetWorld()->LineTraceSingleByChannel(HitDetailsTripod, StartTripod, EndTripod, ECC_Visibility, TraceParams);
	TripodLength = HitDetailsTripod.Distance;

	//SCISSORS

	//Trace

	if (VLightType == EVLightTypeMS::Spotlight)
	{
		ScissorsInitialOffsetAdd = 6 * LightScale;
	}
	else
	{
		ScissorsInitialOffsetAdd = 0;
	}

	HingeScissorsLoc = ScissorsHinge->GetComponentLocation();
	StartScissors = HingeScissorsLoc + FVector(0.0f, 0.0f, ScissorsInitialOffsetAdd);
	EndScissors = HingeScissorsLoc + FVector(0.0f, 0.0f, 2500.0f);

	HitDetailsScissors = FHitResult(ForceInit);

	bIsHitScissors = GetWorld()->LineTraceSingleByChannel(HitDetailsScissors, StartScissors, EndScissors, ECC_Visibility, TraceParams);
}


void AMetaShoot_SoftboxMaster::UpdateSupport()
{

	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);

	if (VLightSupport == EVLightSupportMS::Tripod)
	{
		AMetaShoot_SoftboxMaster::UpdateTripod();
	}
	if (VLightSupport == EVLightSupportMS::Scissors)
	{
		AMetaShoot_SoftboxMaster::UpdateScissors();
	}
	if (VLightSupport == EVLightSupportMS::Automatic)
	{
		if (bIsHitTripod && bIsHitScissors)
		{
			if (HitDetailsTripod.Distance < HitDetailsScissors.Distance)
			{
				AMetaShoot_SoftboxMaster::UpdateTripod();
			}
			else
			{
				AMetaShoot_SoftboxMaster::UpdateScissors();
			}
		}
		else if (bIsHitTripod && HitDetailsTripod.Distance > 38 * LightScale)
		{
			AMetaShoot_SoftboxMaster::UpdateTripod();
		}
		else if (bIsHitScissors)
		{
			AMetaShoot_SoftboxMaster::UpdateScissors();
		}
		//else if (!bIsHitTripod && !bIsHitScissors)
		else
		{
			LightBodyFloor->SetVisibility(false, true);
			LightBodyFloor->SetVisibility(true, false);
			LightBodyFloor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LightBodyCeiling->SetVisibility(false, true);
			LightBodyCeiling->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodHinge->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodLock00->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodLock01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodPipe00->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodPipe01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			TripodPipe02->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}
	if (VLightSupport == EVLightSupportMS::None)
	{
		LightBodyFloor->SetVisibility(false, true);
		LightBodyFloor->SetVisibility(true, false);
		LightBodyFloor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LightBodyCeiling->SetVisibility(false, true);
		LightBodyCeiling->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodHinge->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodLock00->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodLock01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodPipe00->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodPipe01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TripodPipe02->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);


	if (VLightType == EVLightTypeMS::Spotlight || VLightType == EVLightTypeMS::RingLight || VLightType == EVLightTypeMS::LightWand)
	{
		LightBodyFloor->SetVisibility(false, false);
		LightBodyCeiling->SetVisibility(false, false);
	}

}

void AMetaShoot_SoftboxMaster::UpdateTripod()
{
	LightBodyFloor->SetVisibility(true, true);
	LightBodyFloor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LightBodyCeiling->SetVisibility(false, true);
	LightBodyCeiling->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodHinge->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodLock00->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodLock01->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodPipe00->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodPipe01->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodPipe02->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (bIsHitTripod)
	{
		//Get Trace Info
		//HitDetailsTripod.ImpactPoint;
		//HitDetailsTripod.Distance;

		//Calculate Impact point relative to the Hinge so all coordinates are based on Z
		FVector ImpactPointTripodRelative = UKismetMathLibrary::InverseTransformLocation(TripodHinge->GetComponentTransform(), HitDetailsTripod.ImpactPoint);

		//Rotate Lightbody so it stays straight
		FRotator HingeToFloor = UKismetMathLibrary::FindLookAtRotation(StartTripod, EndTripod);
		float LightBodyYaw = LightBodyFloor->GetComponentRotation().Yaw;
			

		//TripodHinge->SetWorldRotation(FRotator(HingeToFloor.Yaw, LightBodyYaw, HingeToFloor.Roll));
		TripodHinge->SetWorldRotation(FRotator(0.0f, this->GetActorRotation().Yaw, 0.0f));

		if (VLightType == EVLightTypeMS::Spotlight)
		{
			//TripodHinge->AddLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
		}
		if (VLightType == EVLightTypeMS::RingLight)
		{
			LightDiffuser->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
		if (VLightType == EVLightTypeMS::LightWand)
		{
			LightDiffuser->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}

		//Locate Tripod parts
		TripodBase->SetWorldLocation(HitDetailsTripod.ImpactPoint);
		TripodBase->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f) * LightScale);

		float TripodLock01Z = ((((ImpactPointTripodRelative.Z * -1) - 38) - 6) / -3) - 6;
		float TripodLock00Z = (((((ImpactPointTripodRelative.Z * -1) - 38) - 6) / -3) * 2) - 6;
		float TripodPipeZScale = LightScale * (((ImpactPointTripodRelative.Z * -1) - (TripodLock00Z * -1)) - 18) / 50;

		TripodLock01->SetRelativeLocation(FVector(0.0f, 0.0f, TripodLock01Z));
		TripodLock00->SetRelativeLocation(FVector(0.0f, 0.0f, TripodLock00Z));
		TripodPipe00->SetWorldScale3D(FVector(LightScale, LightScale, TripodPipeZScale));
		TripodPipe01->SetWorldScale3D(FVector(LightScale, LightScale, TripodPipeZScale));
		TripodPipe02->SetWorldScale3D(FVector(LightScale, LightScale, TripodPipeZScale));

	}
	else
	{

	}
}

void AMetaShoot_SoftboxMaster::UpdateScissors()
{
	LightBodyFloor->SetVisibility(false, true);
	LightBodyFloor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LightBodyCeiling->SetVisibility(true, true);
	LightBodyCeiling->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TripodBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodHinge->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodLock00->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodLock01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodPipe00->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodPipe01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TripodPipe02->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	TArray<USceneComponent*> LightBodyFloorArray;
	LightBodyCeiling->GetChildrenComponents(true, LightBodyFloorArray);

	int32 Num = LightBodyFloorArray.Num();
	for (int32 i = 0; i < Num; i++)
	{
		UStaticMeshComponent* USMTest;
		USMTest = Cast<UStaticMeshComponent>(LightBodyFloorArray[i]);
		USMTest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	*/

	float ScissorsSegments = VLightScissorsSegments;
	float ScissorsSegmentLength = 28.2 * LightScale;
	float ScissorsInitialOffset = 8 * LightScale;
	float ScissorsZOffset = (HitDetailsScissors.Distance - ScissorsInitialOffset) / ScissorsSegments;

	if (bIsHitScissors)
	{
		//Get Trace Info
		//HitDetailsScissors.ImpactPoint;
		//HitDetailsScissors.Distance;

		//Calculate Impact point relative to the Hinge so all coordinates are based on Z
		FVector ImpactPointScissorsRelative = UKismetMathLibrary::InverseTransformLocation(ScissorsHinge->GetComponentTransform(), HitDetailsScissors.ImpactPoint);

		//Rotate Lightbody so it stays straight
		FRotator HingeToCeiling = UKismetMathLibrary::FindLookAtRotation(StartScissors, EndScissors);
		float LightBodyYaw = LightBodyCeiling->GetComponentRotation().Yaw;

		//ScissorsHinge->SetWorldRotation(FRotator(HingeToCeiling.Yaw, LightBodyYaw, HingeToCeiling.Roll));
		ScissorsHinge->SetWorldRotation(FRotator(0.0f, this->GetActorRotation().Yaw, 180.0f));


		if (VLightType == EVLightTypeMS::Spotlight)
		{
			//ScissorsHinge->AddLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
		}

		if (VLightType == EVLightTypeMS::RingLight)
		{
			LightDiffuser->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
		}

		if (VLightType == EVLightTypeMS::LightWand)
		{
			LightDiffuser->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
		}

		
		//Calculate Rotator
		FRotator ScissorsSegmentR01 = FRotator(UKismetMathLibrary::DegAsin((ScissorsZOffset / ScissorsSegmentLength)), 0.0f, 0.0f);
		FRotator ScissorsSegmentR02 = FRotator(UKismetMathLibrary::DegAsin((-ScissorsZOffset / ScissorsSegmentLength)), 0.0f, 0.0f);

		//Calculate Location Segment 02
		FVector ScissorsSegment02L01 = FVector(0.0f, 0.0f, (-ScissorsInitialOffset - ScissorsInitialOffsetAdd) / LightScale);
		//FVector ScissorsSegment02L02 = FVector(0.0f, 0.0f, -HitDetailsScissors.Distance);
		FVector ScissorsSegment02L02 = FVector(0.0f, 0.0f, (-ScissorsInitialOffset - ScissorsInitialOffsetAdd - ScissorsZOffset * ScissorsSegments) / LightScale);

		//Calculate Location Segment 01
		FVector ScissorsSegment01L01 = FVector(0.0f, 0.0f, (-ScissorsInitialOffset - ScissorsInitialOffsetAdd - ScissorsZOffset * 0.5) / LightScale);
		//FVector ScissorsSegment01L02 = FVector(0.0f, 0.0f, -HitDetailsScissors.Distance + ScissorsZOffset * 0.5);
		FVector ScissorsSegment01L02 = FVector(0.0f, 0.0f, (-ScissorsInitialOffset - ScissorsInitialOffsetAdd - ScissorsZOffset * ScissorsSegments + ScissorsZOffset * 0.5) / LightScale);

		//Scale
		FVector ScissorsSegmentsS = FVector(1.0f, 1.0f, 1.0f) * 1;

		//Transform Segment 02
		FTransform ScissorsSegment02T01 = FTransform(ScissorsSegmentR02, ScissorsSegment02L01, ScissorsSegmentsS);
		FTransform ScissorsSegment02T02 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR02, FRotator(0.0f, 180.0f, 0.0f)), ScissorsSegment02L01, ScissorsSegmentsS);
		FTransform ScissorsSegment02T03 = FTransform(ScissorsSegmentR01, ScissorsSegment02L02, ScissorsSegmentsS);
		FTransform ScissorsSegment02T04 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR01, FRotator(0.0f, 180.0f, 0.0f)), ScissorsSegment02L02, ScissorsSegmentsS);

		//Transform Segment 01
		FTransform ScissorsSegment01T01 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR01, FRotator(0.0f, 0.0f, 180.0f)), ScissorsSegment01L01, ScissorsSegmentsS);
		FTransform ScissorsSegment01T02 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR01, FRotator(0.0f, 180.0f, 180.0f)), ScissorsSegment01L01, ScissorsSegmentsS);
		FTransform ScissorsSegment01T03 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR02, FRotator(0.0f, 0.0f, 180.0f)), ScissorsSegment01L02, ScissorsSegmentsS);
		FTransform ScissorsSegment01T04 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR02, FRotator(0.0f, 180.0f, 180.0f)), ScissorsSegment01L02, ScissorsSegmentsS);

		//Add Necessary Segment 02
		if (ScissorsSegment02->GetInstanceCount() < 4)
		{
			for (int i = 0; i < 4; i++)
			{
				ScissorsSegment02->AddInstance(FTransform());
			}
		}
		if (ScissorsSegment02->GetInstanceCount() > 4)
		{
			for (int i = 0; i < ScissorsSegment02->GetInstanceCount() - 4; i++)
			{
				ScissorsSegment02->RemoveInstance(ScissorsSegment02->GetInstanceCount() - 1);
			}
		}

		//Update Segment 02
		ScissorsSegment02->UpdateInstanceTransform(0, ScissorsSegment02T01);
		ScissorsSegment02->UpdateInstanceTransform(1, ScissorsSegment02T02);
		ScissorsSegment02->UpdateInstanceTransform(2, ScissorsSegment02T03);
		ScissorsSegment02->UpdateInstanceTransform(3, ScissorsSegment02T04);

		//Add Necessary Segment 01
		if (ScissorsSegment01->GetInstanceCount() < 4)
		{
			for (int i = 0; i < 4; i++)
			{
				ScissorsSegment01->AddInstance(FTransform());
			}
		}
		if (ScissorsSegment01->GetInstanceCount() > 4)
		{
			for (int i = 0; i < ScissorsSegment01->GetInstanceCount() - 4; i++)
			{
				ScissorsSegment01->RemoveInstance(ScissorsSegment01->GetInstanceCount() - 1);
			}
		}

		//Update Segment 01
		ScissorsSegment01->UpdateInstanceTransform(0, ScissorsSegment01T01);
		ScissorsSegment01->UpdateInstanceTransform(1, ScissorsSegment01T02);
		ScissorsSegment01->UpdateInstanceTransform(2, ScissorsSegment01T03);
		ScissorsSegment01->UpdateInstanceTransform(3, ScissorsSegment01T04);

		//Add Necessary Segment 00
		if (ScissorsSegment00->GetInstanceCount() < (ScissorsSegments - 2) * 2)
		{
			for (int i = 0; i < (ScissorsSegments - 2) * 2 - ScissorsSegment00->GetInstanceCount(); i++)
			{
				ScissorsSegment00->AddInstance(FTransform());
				ScissorsSegment00->AddInstance(FTransform());
			}
		}

		//Remove Inecessary Segment 00
		if (ScissorsSegment00->GetInstanceCount() > (ScissorsSegments - 2) * 2)
		{

			for (int i = 0; i < ScissorsSegment00->GetInstanceCount() - (ScissorsSegments - 2) * 2; i++)
			{
				ScissorsSegment00->RemoveInstance(ScissorsSegment00->GetInstanceCount() - 1);
				ScissorsSegment00->RemoveInstance(ScissorsSegment00->GetInstanceCount() - 1);
			}
		}

		//Update Segment 00
		for (int i = 0; i < ScissorsSegments - 2; i++)
		{
			FVector ScissorsSegment00L = FVector(0.0f, 0.0f, (-ScissorsInitialOffset - ScissorsInitialOffsetAdd - ScissorsZOffset * (i + 1) - ScissorsZOffset / 2) / LightScale);
			FTransform ScissorsSegment00T01 = FTransform(ScissorsSegmentR02, ScissorsSegment00L, ScissorsSegmentsS);
			ScissorsSegment00->UpdateInstanceTransform(i * 2, ScissorsSegment00T01);
			FTransform ScissorsSegment00T02 = FTransform(UKismetMathLibrary::ComposeRotators(ScissorsSegmentR02, FRotator(0.0f, 180.0f, 0.0f)), ScissorsSegment00L, ScissorsSegmentsS);
			ScissorsSegment00->UpdateInstanceTransform(i * 2 + 1, ScissorsSegment00T02);
		}

	}
	else
	{

	}
}

//#if WITH_EDITOR
//AMetaShoot_SoftboxMaster::PostEditMove(bool bFinished)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, TEXT("Hello"));
//}
//#endif