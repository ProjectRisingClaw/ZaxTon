// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ZaxMode.generated.h"





/**
 * 
 */
class ABaseFoe;
class AExplosion;

UCLASS()
class ZAXTON_API AZaxMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	
	// gestione di pool per spawn proiettili
	TArray<ABaseFoe*> Available; // proiettili disponibili
	TArray<ABaseFoe*> InUse;     //proiettili correntemente in uso

	//virtual void StartPlay() override;
	// array per actor con Niagaracomponent.
	TArray<AExplosion*> AvailableEffects;


};
