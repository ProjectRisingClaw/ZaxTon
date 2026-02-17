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


	// path della DataTable  per i nemici
	auto Path = TEXT("/Game/DataTables/BPEnemyTable");

	MyDT =  LoadObject<UDataTable>(nullptr, Path) ; // recupero la DT tramite Path
	// vado a trovare la riga che mi interessa sulla DT
	FEnemyTableRaw* MyRow{ MyDT->FindRow<FEnemyTableRaw>(FName("NemicoA"),TEXT("Context")) };


	// path dell'asset
	if (MyRow->Mesh)
	{
		Collision->SetCapsuleSize(64, 32);
		Collision->SetHiddenInGame(true);
		Body->SetStaticMesh(MyRow->Mesh);
		Body->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
		Body->CastShadow = false;

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
void ABaseFoe::Activate(FVector SpawnLocation, FRotator SpawnRotation, FName NewType)
{
	// carico dati dalla DT
	FEnemyTableRaw* MyRow{ MyDT->FindRow<FEnemyTableRaw>(NewType,TEXT("Context")) };
	if (MyRow)
	{
		Body->SetStaticMesh(MyRow->Mesh);     // copio valore della mesh da DT
		ExplosionEffect =   MyRow->ExplosionFX; // copio valore VFX da Data table;
	}
	//

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

		UE_LOG(LogTemp, Warning, TEXT("Mi rimuovo disponibili = %i"), MyGM->Available.Num());
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}