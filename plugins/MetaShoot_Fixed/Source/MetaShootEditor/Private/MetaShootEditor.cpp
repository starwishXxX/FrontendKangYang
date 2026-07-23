// Copyright 2023, VINZI Studio S.L. All rights reserved

#include "MetaShootEditor.h"
#include "MetaShootStyle.h"
#include "MetaShootCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"


#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Editor/Blutility/Public/IBlutilityModule.h"
#include "EditorUtilitySubsystem.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetBlueprint.h"

static const FName MetaShootTabName("MetaShoot");

#define LOCTEXT_NAMESPACE "FMetaShootEditorModule"

void FMetaShootEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMetaShootStyle::Initialize();
	FMetaShootStyle::ReloadTextures();

	FMetaShootCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMetaShootCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMetaShootEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMetaShootEditorModule::RegisterMenus));
	
}

void FMetaShootEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMetaShootStyle::Shutdown();

	FMetaShootCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MetaShootTabName);
}


void FMetaShootEditorModule::PluginButtonClicked()
{
	//FGlobalTabmanager::Get()->TryInvokeTab(MetaShootTabName);
	

	//FString MetaShootPluginPath = IPluginManager::Get().FindPlugin(TEXT("MetaShoot"))->GetContentDir().Append("/MetaShoot.MetaShoot");

	//FString MetaShootPluginPath = FPaths::ProjectPluginsDir() + "/MetaShoot/Content/MetaShoot.MetaShoot";

	FString MetaShootPluginPath = "/MetaShoot/Assets/UI/MetaShoot.MetaShoot";

	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Orange, MetaShootPluginPath);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, projectDir);


	//UWidgetBlueprint* Blueprint = LoadObject<UWidgetBlueprint>(nullptr, TEXT(*MetaShootPluginPath), NULL, LOAD_None, NULL);

	//UWidgetBlueprint* Blueprint = LoadObject<UWidgetBlueprint>(nullptr, L"/Game/MetaShot.MetaShot");

	UWidgetBlueprint* Blueprint = LoadObject<UWidgetBlueprint>(NULL, *MetaShootPluginPath);

	if (Blueprint->GeneratedClass->IsChildOf(UEditorUtilityWidget::StaticClass()))
	{
		UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(Blueprint);
		if (EditorWidget)
		{
			UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
			EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget);
		}
	}
}

void FMetaShootEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMetaShootCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMetaShootCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMetaShootEditorModule, MetaShoot)