// Copyright 2023, VINZI Studio S.L. All rights reserved

#include "MetaShootCommands.h"

#define LOCTEXT_NAMESPACE "FMetaShootModule"

void FMetaShootCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MetaShoot", "Bring up MetaShoot window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
