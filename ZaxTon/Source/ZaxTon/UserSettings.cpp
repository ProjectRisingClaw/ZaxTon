// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSettings.h"

void UUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
    Super::ApplySettings(bCheckForCommandLineOverrides);

    // Forza risoluzione 4:3
    SetScreenResolution(FIntPoint(1024, 768));
    ApplyResolutionSettings(false);
}

void UUserSettings::SetToDefaults()
{
    Super::SetToDefaults();

    // Valori di default
    ResolutionSizeX = 1024;
    ResolutionSizeY = 768;
    LastUserConfirmedResolutionSizeX = 1024;
    LastUserConfirmedResolutionSizeY = 768;
    FullscreenMode = EWindowMode::Windowed;
    bUseDesiredScreenHeight = false;
}
