// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IBlankPlugin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBlankPlugin, Log, All);
DEFINE_LOG_CATEGORY(LogBlankPlugin);


class FBlankPlugin : public IBlankPlugin
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
};

IMPLEMENT_MODULE( FBlankPlugin, BlankPlugin )



void FBlankPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	UE_LOG(LogBlankPlugin, Log, TEXT("FBlankPlugin::StartupModule"));

}


void FBlankPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogBlankPlugin, Log, TEXT("FBlankPlugin::ShutdownModule"));
}



