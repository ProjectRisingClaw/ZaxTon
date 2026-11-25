// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuel.h"
#include "../Player/PShip.h"
#include "../ZaxMode.h"
#include "../Headers/DataTables.h"

// Sets default values
AFuel::AFuel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);
	Body->SetCollisionObjectType(ECC_ENEMY_OBJ);
	// deve vedere il giocatore come overlap
	Body->SetCollisionResponseToChannel(ECC_PLAYER_OBJ, ECR_Overlap);

}

// Called when the game starts or when spawned
void AFuel::BeginPlay()
{
	Super::BeginPlay();
	
	auto Path = TEXT("/Game/Import/Carburante.Carburante");
	auto MyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, Path));
	Body->SetStaticMesh(MyMesh);
}

// Called every frame
void AFuel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFuel::Activate(FVector Posizione)
{
	SetActorLocation(Posizione);
	Body->SetHiddenInGame(false);
	MyGM->InUseFuel.AddUnique(this);
}

void AFuel::DeActivate()
{
	SetActorLocation(FVector(0, 0, -10000));
	Body->SetHiddenInGame(true);

	// mi inserisco tra i disponibil
	if (MyGM)
	{

		MyGM->InUseFuel.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyGM->AvailableFuel.AddUnique(this);
	}

}

