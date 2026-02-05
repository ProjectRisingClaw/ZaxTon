// Fill out your copyright notice in the Description page of Project Settings.


#include "ZaxMode.h"
#include "Enemies/BaseFoe.h"
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

	for (int i = 0; i < 40; i++)
	{
		// creo istanza
		auto Enemy{ GetWorld()->SpawnActor<ABaseFoe>(ABaseFoe::StaticClass()) };
		if (Enemy)
		{
			Enemy->SetOwner(this); // come creo il proiettile
			// gli lascio un riferimento all'oggetto che lo ha creato
			Enemy->DeActivate(); // disattivo istanza
		}
		//Available.AddUnique(bull); // inserisco in array dei disponibili.
	}


}

