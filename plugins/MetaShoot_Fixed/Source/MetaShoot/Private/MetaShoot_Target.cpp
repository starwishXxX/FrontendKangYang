// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_Target.h"
#include "GameFramework/Actor.h"



// Sets default values
AMetaShoot_Target::AMetaShoot_Target()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TargetActorBillboard = CreateDefaultSubobject<UBillboardComponent>(FName("TABill"));
	//this->SetRootComponent(TargetActorBillboard);

	TargetActorMaterialBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(FName("TAMatBill"));
	this->SetRootComponent(TargetActorMaterialBillboard);

	//TargetActorBillboard->SetVisibility(false);

}

// Called when the game starts or when spawned
void AMetaShoot_Target::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaShoot_Target::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateLights();

}

void AMetaShoot_Target::OnConstruction(const FTransform& Transform)
{
	UpdateLights();

	for (TActorIterator<AMetaShoot_RectLight> It(GetWorld(), AMetaShoot_RectLight::StaticClass()); It; ++It)
	{
		AMetaShoot_RectLight* actor = *It;
		if (actor->TargetActor == this)
		{
			actor->LookAtTarget();
		}

	}

	/*for (TActorIterator<AMetaShoot_RailSystem> It(GetWorld(), AMetaShoot_RailSystem::StaticClass()); It; ++It)
	{
		AMetaShoot_RailSystem* actor = *It;
		actor->UpdateRails();
		actor->UpdateLightsScissors();
	}*/

	for (TActorIterator<AMetaShoot_SoftboxMaster> It(GetWorld(), AMetaShoot_SoftboxMaster::StaticClass()); It; ++It)
	{
		AMetaShoot_SoftboxMaster* actor = *It;
		if (actor->VActorToTrack == this)
		{
			actor->UpdateAll(true);
		}
		
	}

	for (TActorIterator<AMetaShoot_OverheadLightBank> It(GetWorld(), AMetaShoot_OverheadLightBank::StaticClass()); It; ++It)
	{
		AMetaShoot_OverheadLightBank* actor = *It;
		if (actor->VActorToTrack == this)
		{
			actor->UpdateAll();
		}

	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, TEXT("Const Working"));
}

void AMetaShoot_Target::MoveToActor()
{
	if (ActorToMove)
	{
		this->SetActorLocation(ActorToMove->GetActorLocation());
	}
}

#if WITH_EDITOR
void AMetaShoot_Target::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMetaShoot_Target, ActorToMove)) {
		if (ActorToMove)
		{
			AMetaShoot_Target::MoveToActor();			
			ActorToMove = nullptr;
		}
		else
		{
		}
	}
}
#endif

/*
void AMetaShoot_Target::PostEditMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, TEXT("Target Working"));
}



void AMetaShoot_Target::BroadcastOnActorMoving()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, TEXT("Moved Working"));
}
*/