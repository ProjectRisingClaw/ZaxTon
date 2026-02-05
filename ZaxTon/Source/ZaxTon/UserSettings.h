// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "UserSettings.generated.h"



UCLASS()
class ZAXTON_API UUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

    virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;


    virtual void SetToDefaults() override;
 
};
