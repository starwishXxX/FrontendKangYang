// Copyright 2023, VINZI Studio S.L. All rights reserved


#include "MetaShoot_EditorWidget.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/DateTime.h"
#include "Kismet/GameplayStatics.h"



//#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

UMetaShoot_EditorWidget::UMetaShoot_EditorWidget()
{
}

UMetaShoot_EditorWidget::~UMetaShoot_EditorWidget()
{
}



//void UMetaShoot_EditorWidget::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("OK"));
//}

void UMetaShoot_EditorWidget::getViewPortCameraPosLoc()
{

	FViewport* activeViewport = GEditor->GetActiveViewport();
	FEditorViewportClient* editorViewClient = (activeViewport != nullptr) ? (FEditorViewportClient*)activeViewport->GetClient() : nullptr;
	if (editorViewClient)
	{
		viewPos = editorViewClient->GetViewLocation();
		viewRot = editorViewClient->GetViewRotation();
	}
}





void UMetaShoot_EditorWidget::GetSample()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::OnResponseReceivedSample);
	Request->SetURL("https://jsonplaceholder.typicode.com/posts/1");
	Request->SetVerb("GET");

	Request->ProcessRequest();
}

void UMetaShoot_EditorWidget::PostSample()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("title", "foo");

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::OnResponseReceivedSample);
	Request->SetURL("https://jsonplaceholder.typicode.com/posts");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	Request->ProcessRequest();
}


void UMetaShoot_EditorWidget::OnResponseReceivedSample(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
	UE_LOG(LogTemp, Display, TEXT("Title: %s"), *ResponseObj->GetStringField("title"));
}









FString keyPath = FPaths::ConvertRelativePathToFull(FPaths::RootDir()) + TEXT("Engine/Plugins/VINZI/MetaShoot/log.txt");

FTimerHandle UpdateLastCheckTimerHandle;
FTimerHandle LoggedOutTimerHandle;

void UMetaShoot_EditorWidget::printRequestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
}

void UMetaShoot_EditorWidget::checkLog()
{
	return;
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*keyPath))
	{
		//if local log file exists

		//Load log txt
		FString FileData = "";
		FFileHelper::LoadFileToString(FileData, *keyPath);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(FileData);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			FString FLocalEmail = JsonObject->GetStringField("email");
			FString FLocalSession = JsonObject->GetStringField("session");

			Vemail = FLocalEmail;

			FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

			TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
			RequestObj->SetStringField("email", FLocalEmail);
			RequestObj->SetStringField("session", FLocalSession);

			FDateTime currentTime = FDateTime::UtcNow();
			FString currentTimeString = currentTime.ToString().Replace(TEXT("."), TEXT(""));
			RequestObj->SetStringField("currentTime", currentTimeString);

			FString RequestBody;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
			FJsonSerializer::Serialize(RequestObj, Writer);

			Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::loginChecked);
			Request->SetURL("https://vinzi.xyz/MetaShoot/license/checkLog.php");
			Request->SetVerb("POST");
			Request->SetHeader("Content-Type", "application/json");
			Request->SetContentAsString(RequestBody);

			Request->ProcessRequest();
		}

		//UMetaShoot_EditorWidget::checkSession();

	}
	else
	{
		//if local log file doesn't exist

		FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

		TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

		FString RequestBody;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
		FJsonSerializer::Serialize(RequestObj, Writer);

		Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::loginChecked);
		Request->SetURL("https://vinzi.xyz/MetaShoot/license/checkLog.php");
		Request->SetVerb("POST");
		Request->SetHeader("Content-Type", "application/json");
		Request->SetContentAsString(RequestBody);

		Request->ProcessRequest();



		/*BMSActive = false;
		VphpResponse = TEXT("");
		UMetaShoot_EditorWidget::LoggedEvent();*/
	}

}

void UMetaShoot_EditorWidget::authenticate()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("email", Vemail);
	RequestObj->SetStringField("password", Vpassword);

	//Get current UTC universal date - no daylight savings apply
	FDateTime currentTime = FDateTime::UtcNow();
	FString currentTimeString = currentTime.ToString().Replace(TEXT("."), TEXT(""));

	RequestObj->SetStringField("currentTime", currentTimeString);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::loginChecked);
	Request->SetURL("https://vinzi.xyz/MetaShoot/license/authenticate.php");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	Request->ProcessRequest();
}

void UMetaShoot_EditorWidget::loginChecked(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	FString MSRequestResponse = Response->GetContentAsString();

	FString responseLogged, responseCase;
	MSRequestResponse.Split(TEXT("_"), &responseLogged, &responseCase);

	GetWorld()->GetTimerManager().ClearTimer(LoggedOutTimerHandle);

	if (responseLogged == "LoggedIn") {
		BMSActive = true;
		VphpResponse = TEXT("");
		UMetaShoot_EditorWidget::updateLog(responseCase);
	}
	else if(responseLogged == "LoggedOut") {
		BMSActive = false;
		VphpResponse = responseCase;
		UMetaShoot_EditorWidget::loggedOutMessage();
		GetWorld()->GetTimerManager().SetTimer(LoggedOutTimerHandle, this, &UMetaShoot_EditorWidget::loggedOutMessage, 16.0f, true);
	}
	if (responseLogged == "") {
		//OFFLINE MODE
		BMSActive = true;
		VphpResponse = TEXT("");
		Vemail = TEXT("Offline");
	}

	UE_LOG(LogTemp, Display, TEXT("Logged response: %s"), *responseLogged);
	UE_LOG(LogTemp, Display, TEXT("Case response: %s"), *responseCase);

	UMetaShoot_EditorWidget::LoggedEvent();
	UMetaShoot_EditorWidget::applyLicenseToMetaShootActors(responseLogged);
}

void UMetaShoot_EditorWidget::updateLog(FString Case)
{

	//Get email and session variables
	FString updatedEmail;

	if (Case == TEXT("checkLog")) {
		//read local log, get email
		FString FileData = "";
		FFileHelper::LoadFileToString(FileData, *keyPath);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(FileData);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			FString Femail = JsonObject->GetStringField("email");
			FString Fsession = JsonObject->GetStringField("session");

			updatedEmail = Femail;
		}
	}

	if (Case == TEXT("authenticate")) {
		//get email from UI form
		updatedEmail = Vemail;
	}

	//Get current UTC universal date - no daylight savings apply
	FDateTime currentTime = FDateTime::UtcNow();
	FString currentTimeString = currentTime.ToString().Replace(TEXT("."), TEXT(""));

	//Update local log
	TSharedRef<FJsonObject> writeJsonObj = MakeShared<FJsonObject>();
	writeJsonObj->SetStringField("email", updatedEmail);
	writeJsonObj->SetStringField("session", currentTimeString);

	FString writeBody;
	TSharedRef<TJsonWriter<>> writeWriter = TJsonWriterFactory<>::Create(&writeBody);
	FJsonSerializer::Serialize(writeJsonObj, writeWriter);

	FString FileContent = writeBody;
	FFileHelper::SaveStringToFile(FileContent, *keyPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_None);
	//Append text to file instead of replacing it
	//FFileHelper::SaveStringToFile(FileContent, *keyPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);


	//Update server log

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("email", updatedEmail);
	RequestObj->SetStringField("session", currentTimeString);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::printRequestResponse);
	Request->SetURL("https://vinzi.xyz/MetaShoot/license/updateLog.php");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	Request->ProcessRequest();

	UMetaShoot_EditorWidget::updateLastCheck();
	// Start the timer to call updateLastCheck every 25 seconds
	GetWorld()->GetTimerManager().SetTimer(UpdateLastCheckTimerHandle, this, &UMetaShoot_EditorWidget::updateLastCheck, 25.0f, true);

}

void UMetaShoot_EditorWidget::logout()
{
	FString updatedEmail;

	//read local log, get email
	FString FileData = "";
	FFileHelper::LoadFileToString(FileData, *keyPath);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(FileData);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		FString Femail = JsonObject->GetStringField("email");
		FString Fsession = JsonObject->GetStringField("session");

		updatedEmail = Femail;
	}

	//Get current UTC universal date - no daylight savings apply
	FDateTime currentTime = FDateTime::UtcNow();
	FString currentTimeString = currentTime.ToString().Replace(TEXT("."), TEXT(""));

	//Update local log
	TSharedRef<FJsonObject> writeJsonObj = MakeShared<FJsonObject>();
	writeJsonObj->SetStringField("email", updatedEmail);
	writeJsonObj->SetStringField("session", currentTimeString);

	FString writeBody;
	TSharedRef<TJsonWriter<>> writeWriter = TJsonWriterFactory<>::Create(&writeBody);
	FJsonSerializer::Serialize(writeJsonObj, writeWriter);

	FString FileContent = writeBody;
	FFileHelper::SaveStringToFile(FileContent, *keyPath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_None);


	//Get Logout php
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::loginChecked);
	Request->SetURL("https://vinzi.xyz/MetaShoot/license/logout.php");
	Request->SetVerb("GET");

	Request->ProcessRequest();

	// Stop the timer
	GetWorld()->GetTimerManager().ClearTimer(UpdateLastCheckTimerHandle);
}


void UMetaShoot_EditorWidget::updateLastCheck()
{
	//Get email and session variables
	FString updatedEmail = Vemail;

	//Get current UTC universal date - no daylight savings apply
	FDateTime currentTime = FDateTime::UtcNow();
	FString currentTimeString = currentTime.ToString().Replace(TEXT("."), TEXT(""));

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("email", updatedEmail);
	RequestObj->SetStringField("lastCheck", currentTimeString);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	//Request->OnProcessRequestComplete().BindUObject(this, &UMetaShoot_EditorWidget::printRequestResponse);
	Request->SetURL("https://vinzi.xyz/MetaShoot/license/updateLastCheck.php");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	Request->ProcessRequest();
}



void UMetaShoot_EditorWidget::applyLicenseToMetaShootActors(FString Case)
{
	
	if (Case == "LoggedIn" || Case == "") {
		//Case "" is Offline Mode
		for (TObjectIterator<AMetaShoot_SoftboxMaster> Itr; Itr; ++Itr)
		{
			AMetaShoot_SoftboxMaster* MyObject = *Itr;
			//UE_LOG(LogTemp, Display, TEXT("MyObject: %s"), *MyObject->GetName());

			MyObject->BMSAssetActive = true;

			TArray<UActorComponent*> Components;
			MyObject->GetComponents(Components);
			for (UActorComponent* Component : Components)
			{
				if (Component->IsA<UStaticMeshComponent>()) // check if the component is a Static Mesh Component
				{
					UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
					StaticMesh->SetVisibility(true);
				}
			}
			
			MyObject->UpdateAll(true);
		}
		for (TObjectIterator<AMetaShoot_OverheadLightBank> Itr; Itr; ++Itr)
		{
			AMetaShoot_OverheadLightBank* MyObject = *Itr;
			//UE_LOG(LogTemp, Display, TEXT("MyObject: %s"), *MyObject->GetName());

			MyObject->BMSAssetActive = true;

			TArray<UActorComponent*> Components;
			MyObject->GetComponents(Components);
			for (UActorComponent* Component : Components)
			{
				if (Component->IsA<UStaticMeshComponent>()) // check if the component is a Static Mesh Component
				{
					UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
					StaticMesh->SetVisibility(true);
				}
			}

			MyObject->UpdateAll();
		}
		for (TObjectIterator<AMetaShoot_Cyclorama> Itr; Itr; ++Itr)
		{
			AMetaShoot_Cyclorama* MyObject = *Itr;
			//UE_LOG(LogTemp, Display, TEXT("MyObject: %s"), *MyObject->GetName());
			MyObject->BMSAssetActive = true;

			TArray<UActorComponent*> Components;
			MyObject->GetComponents(Components);
			for (UActorComponent* Component : Components)
			{
				if (Component->IsA<UStaticMeshComponent>()) // check if the component is a Static Mesh Component
				{
					UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
					StaticMesh->SetVisibility(true);
				}
			}

			MyObject->UpdateCyclorama();
			MyObject->UpdateCycloramaBP();
		}
		/*for (TObjectIterator<AMetaShoot_DSLRCamera> Itr; Itr; ++Itr)
		{
			AMetaShoot_DSLRCamera* MyObject = *Itr;
			MyObject->BMSAssetActive = true;
		}*/
	}
	else if (Case == "LoggedOut") {
		for (TObjectIterator<AMetaShoot_SoftboxMaster> Itr; Itr; ++Itr)
		{
			AMetaShoot_SoftboxMaster* MyObject = *Itr;
			MyObject->BMSAssetActive = false;

			MyObject->RectLightComponent->SetVisibility(false);
			MyObject->PointLightComponent->SetVisibility(false);
			MyObject->SpotLightComponent->SetVisibility(false);

			TArray<UActorComponent*> Components;
			MyObject->GetComponents(Components);
			for (UActorComponent* Component : Components)
			{
				if (Component->IsA<UStaticMeshComponent>()) // check if the component is a Static Mesh Component
				{
					UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
					StaticMesh->SetVisibility(false);
				}
			}

			MyObject->UpdateAll(true);
		}
		for (TObjectIterator<AMetaShoot_OverheadLightBank> Itr; Itr; ++Itr)
		{
			AMetaShoot_OverheadLightBank* MyObject = *Itr;
			MyObject->BMSAssetActive = false;

			MyObject->RectLightComponent->SetVisibility(false);

			TArray<UActorComponent*> Components;
			MyObject->GetComponents(Components);
			for (UActorComponent* Component : Components)
			{
				if (Component->IsA<UStaticMeshComponent>()) // check if the component is a Static Mesh Component
				{
					UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
					StaticMesh->SetVisibility(false);
				}
			}

			MyObject->UpdateAll();
		}
		for (TObjectIterator<AMetaShoot_Cyclorama> Itr; Itr; ++Itr)
		{
			AMetaShoot_Cyclorama* MyObject = *Itr;
			//UE_LOG(LogTemp, Display, TEXT("MyObject: %s"), *MyObject->GetName());
			MyObject->BMSAssetActive = false;

			TArray<UActorComponent*> Components;
			MyObject->GetComponents(Components);
			for (UActorComponent* Component : Components)
			{
				if (Component->IsA<UStaticMeshComponent>()) // check if the component is a Static Mesh Component
				{
					UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
					StaticMesh->SetVisibility(false);
				}
			}

			MyObject->UpdateCyclorama();
		}
		/*for (TObjectIterator<AMetaShoot_DSLRCamera> Itr; Itr; ++Itr)
		{
			AMetaShoot_DSLRCamera* MyObject = *Itr;
			MyObject->BMSAssetActive = false;
		}*/
	}

}

void UMetaShoot_EditorWidget::loggedOutMessage()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Yellow, TEXT("Please login to MetaShoot to access its interface and activate its assets. You can find it in Window/MetaShoot"));
}



void UMetaShoot_EditorWidget::MSRender()
{
	UMovieScene* MSMovieScene = MSLevelSequence->GetMovieScene();

	FFrameRate FrameRate = MSMovieScene->GetTickResolution();

	//int32 EndFrame = (MSEndFrame / FrameRate.Numerator);
	int32 EndFrame = 5;

	FFrameNumber StartFrame = 0;

	//MSEndFrame = MSMovieScene->GetPlaybackRange().GetUpperBoundValue();

	FFrameNumber EndFrameTest = MSMovieScene->GetPlaybackRange().GetUpperBoundValue();

	FFrameNumber test01 = 10;

	//TRange Test1 = 0;

	MSMovieScene->SetPlaybackRange(EndFrameTest, 20);




	//TRange<FFrameNumber> PlaybackRange = MSMovieScene->GetPlaybackRange();

	//// Set the playback range end to the desired frame time
	//PlaybackRange.SetUpperBoundValue(EndFrame);
	//MSMovieScene->SetPlaybackRange(PlaybackRange);

}