// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MetaShootStyle.h"

class FMetaShootCommands : public TCommands<FMetaShootCommands>
{
public:

	FMetaShootCommands()
		: TCommands<FMetaShootCommands>(TEXT("MetaShoot"), NSLOCTEXT("Contexts", "MetaShoot", "MetaShoot Plugin"), NAME_None, FMetaShootStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};