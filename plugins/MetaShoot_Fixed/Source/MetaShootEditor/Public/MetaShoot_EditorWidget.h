// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"

#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/MovieScene/Public/MovieScene.h"
#include "Sections/MovieSceneCameraCutSection.h"
#include "Tracks/MovieSceneCameraCutTrack.h"
#include "Misc/FrameNumber.h"

#include "HTTP.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "MetaShoot_Manifest.h"

#include "EditorUtilityWidgetBlueprint.h"
#include "MetaShoot_EditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class METASHOOTEDITOR_API UMetaShoot_EditorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
	public:
	UMetaShoot_EditorWidget();
	~UMetaShoot_EditorWidget();

	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void checkLog();

	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void authenticate();

	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void logout();


	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void GetSample();

	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void PostSample();


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "EditorWidget")
		void LoggedEvent();


	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void MSRender();



	UFUNCTION(BlueprintCallable, Category = "EditorWidget")
		void getViewPortCameraPosLoc();





	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		FVector viewPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		FRotator viewRot;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		FString Vemail;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		FString Vpassword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EditorWidget")
		FString VLog;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		FString VphpResponse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EditorWidget")
		bool BMSActive = !false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		ULevelSequence* MSLevelSequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EditorWidget")
		int32 MSEndFrame = 1;

	//virtual void Tick(float DeltaTime) override;

private:

	void OnResponseReceivedSample(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

	void printRequestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

	void loginChecked(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

	void updateLog(FString Case);

	void updateLastCheck();

	void applyLicenseToMetaShootActors(FString Case);

	void loggedOutMessage();
	
};