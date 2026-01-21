// Fill out your copyright notice in the Description page of Project Settings.


#include "PCamera.h"

// Sets default values
APCamera::APCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);


	//memorizzo in una variabile il path dell'asset
	auto Path = TEXT("StaticMesh'/Game/StarSparrow/Meshes/Modules/SM_Parts_StarSparrow_Thruster.SM_Parts_StarSparrow_Thruster'");

	// per sicurezza (ad esempio aver dato un path sbagliato) controllo con un cast
	// che il puntatore restituito sia effettivamente a StaticMesh
	auto MyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),  // tipo dell'oggetto da trovare
		nullptr,                                  // riferimeto ad oggetto se serve
		Path));                                    // path dell'asset

	if (MyMesh)
	{
		Body->SetStaticMesh(MyMesh);
		// l'oggetto è soltanto il mio punto
		// di vista non deve essere visibile
		Body->SetHiddenInGame(true);
	}


}

// Called when the game starts or when spawned
void APCamera::BeginPlay()
{
	Super::BeginPlay();
	
	




}

// Called every frame
void APCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

