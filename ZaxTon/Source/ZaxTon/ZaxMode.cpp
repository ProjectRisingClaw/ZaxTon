// Fill out your copyright notice in the Description page of Project Settings.


#include "ZaxMode.h"
#include "Enemies/BaseFoe.h"
#include "Effects/Explosion.h"
#include "Player/PCamera.h"
#include "EngineUtils.h" // per iteratore

/*
void AZaxMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Error, TEXT("Start"));
}*/

void AZaxMode::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Error, TEXT("Begin"));

	for (TActorIterator<APCamera> CamList(GetWorld()); CamList; ++CamList)
	{
		MyCamera = *CamList;
	}


	for (int i = 0; i < 40; i++)
	{
		// creo istanza
		ABaseFoe* Enemy{ GetWorld()->SpawnActor<ABaseFoe>(ABaseFoe::StaticClass()) };
	
		if (Enemy)
		{
		 //Enemy->SetOwner(this); // come creo il proiettile
			Enemy->MyGM = this;
			// gli lascio un riferimento all'oggetto che lo ha creato
			Enemy->DeActivate(); // disattivo istanza
		
			//Available.AddUnique(Enemy);

		   // UE_LOG(LogTemp, Error, TEXT("Size %i"), Available.Num());
		}
		//Available.AddUnique(bull); // inserisco in array dei disponibili.

		AExplosion* Effect{ GetWorld()->SpawnActor<AExplosion>(AExplosion::StaticClass()) };

		if (Effect)
		{
			Effect->SetOwner(this);	
			Effect->DeActivate(); 
			//AvailableEffects.AddUnique(Effect);

		    UE_LOG(LogTemp, Error, TEXT("Av Effects %i"), AvailableEffects.Num());
		}

	}


}

void AZaxMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < InUse.Num(); i++)
	{
		InUse[i]->UpdateLoc(DeltaTime);
	}

}

