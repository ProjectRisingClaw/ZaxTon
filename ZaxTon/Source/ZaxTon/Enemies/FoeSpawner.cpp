// Fill out your copyright notice in the Description page of Project Settings.


#include "FoeSpawner.h"
#include "Components/SphereComponent.h"
#include "../ZaxMode.h"
#include "BaseFoe.h"
#include "ZaxTon/Player/PCamera.h"
#include "Components/ArrowComponent.h"
//#include "ZaxTon/ZaxMode.h";

// Sets default values
AFoeSpawner::AFoeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnPoint"));
	SetRootComponent(SpawnPoint);
	SpawnPoint->bHiddenInGame = true;

	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));
	Direction->SetupAttachment(SpawnPoint);

}

void AFoeSpawner::Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!MyGameMode) return;

	if (!Cast<APCamera>(OtherActor)) return;

	for (int i = 0; i < NumFoe; i++)
	{
		if (MyGameMode->Available.Num() < 1) return;

		ABaseFoe* ToSpawn{ MyGameMode->Available.Pop() };

		if (ToSpawn)
		{
			//UE_LOG(LogTemp, Error, TEXT("Spawn"));

			ToSpawn->Activate(GetActorLocation() - GetActorForwardVector()* (Gap*i), GetActorRotation(), TipiNemici[uint8(EnemyType)]);
			ToSpawn->SetVel(Vel);
			switch (EnemyType)
			{
			case EEnemyType::EET_NemicoA:
			break;
			case EEnemyType::EET_NemicoB: // per il nemico B passo i gradi inziali per la sinusoide
			ToSpawn->SetCustom3(20 * i);
			break;
			case EEnemyType::EET_NemicoC:
			break;
			case EEnemyType::EET_NemicoD:
			break;
		
			}


		

			MyGameMode->InUse.AddUnique(ToSpawn);

		}
	}

}

// Called when the game starts or when spawned
void AFoeSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnPoint->OnComponentBeginOverlap.AddDynamic(this, &AFoeSpawner::Overlap);
	// memorizzo puntatore al mio game mode
	MyGameMode = Cast<AZaxMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
/*void AFoeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}*/

