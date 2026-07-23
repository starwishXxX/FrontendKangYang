// Copyright 2023, VINZI Studio S.L. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FMetaShootModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
