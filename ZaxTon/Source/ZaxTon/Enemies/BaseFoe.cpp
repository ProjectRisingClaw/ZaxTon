// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFoe.h"
#include "Components/CapsuleComponent.h"
#include "ZaxTon/ZaxMode.h" 
// Sets default values
ABaseFoe::ABaseFoe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// pointer                         tipo da creare          // nome scelto
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);

	//auto Path = TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow01.SM_StarSparrow01'"); // funziona ugualmente ma non più necessario
	auto Path = TEXT("/Game/StarSparrow/Meshes/Examples/SM_StarSparrow17");
	//auto Path = TEXT("SM_StarSparrow01");
	// 
	// per sicurezza (ad esempio aver dato un path sbagliato) controllo con un cast
	// che il puntatore restituito sia effettivamente a StaticMesh


	auto MyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),  // tipo dell'oggetto da trovare
		nullptr,                                  // riferimeto ad oggetto se serve
		Path));

	// path dell'asset
	if (MyMesh)
	{
		Collision->SetCapsuleSize(64, 32);
		Collision->SetHiddenInGame(false);
		Body->SetStaticMesh(MyMesh);
		Body->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" no mesh "));
	}


}

// Called when the game starts or when spawned
void ABaseFoe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseFoe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Durata > 0)
//	{
	//	Durata -= DeltaTime;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
//	}
//	else DeActivate();

}


// 
void ABaseFoe::Activate(FVector SpawnLocation, FRotator SpawnRotation)
{
	// disattivo collisione proiettile 
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// disattivo il tick
	PrimaryActorTick.bCanEverTick = true;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(false);
	// posiziono l'oggetto in una zona lontana da quella di azione
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	//Durata = 1.5; // ripristino durata proiettile

}


void ABaseFoe::DeActivate()
{
	// disattivo collisione proiettile 
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// disattivo il tick
	PrimaryActorTick.bCanEverTick = false;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(true);
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(FVector(0, 0, -10000));

	// mi inserisco tra i disponibili
	auto MyGM{ Cast<AZaxMode>(GetOwner()) }; // controllo che Owner sia di tipo APShip
	if (MyGM)
	{
		MyGM->InUse.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyGM->Available.AddUnique(this);
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}