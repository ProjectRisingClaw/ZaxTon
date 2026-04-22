// Fill out your copyright notice in the Description page of Project Settings.


#include "EBullet.h"
#include "ZaxTon/Player/PShip.h" // altrimenti non posso utilizzare variabili e funzioni di questa classe
#include "BaseFoe.h" // includo il nemico per poterlo colpire
#include "Components/SphereComponent.h"
#include "ZaxTon/Headers/DataTables.h" 
#include "ZaxTon/Headers/Enumerators.h"
#include "ZaxTon/ZaxMode.h" 

AEBullet::AEBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(26);
	Collision->SetHiddenInGame(true);

	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);


	auto Path = TEXT("/Game/DataTables/BPEnemyBulletTable.BPEnemyBulletTable");
	MyDT = LoadObject<UDataTable>(nullptr, Path); // recupero la DT tramite Path


}

// 
void AEBullet::Activate(FVector SpawnLocation, FRotator SpawnRotation, FName AttackType)
{

	// controlla la riga della tabella e carica dati

	FBulletTableRaw* MyRow{ MyDT->FindRow<FBulletTableRaw>(AttackType,TEXT("Context")) };

	if (MyRow) // ad ogni activate carico dati dalla tabella
	{
		Body->SetStaticMesh(MyRow->Mesh);
		Body->SetRelativeScale3D(FVector(MyRow->Scale));
		Vel = MyRow->Vel;
		BulletKind = MyRow->BulletKind;


		Degree = MyRow->Degree;
		Number = MyRow->SpreadNumber;
	}

	// disattivo collisione proiettile 
	if (BulletKind == EBulletKind::EBK_Spread) // se il proiettile è di tipo spread
	{ // non ha una funzione diretta. quello che farà sarà a sua volta generare un gruppo di proiettili
		// normali in varie direzioni  (basandosi su variabili di gradi e quantità)








		// subito dopo averli creati si disattiva.
		DeActivate();
	}
	else
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//PrimaryActorTick.bCanEverTick = true;
		// nascondo grafica del proiettile
		Body->SetHiddenInGame(false);
		// posiziono l'ogggetto in una zona lontana da quella di azione
		SetActorLocation(SpawnLocation);
		SetActorRotation(SpawnRotation);
		Durata = 5.f; // ripristino durata proiettile
	}
}


void AEBullet::DeActivate()
{
	// disattivo collisione proiettile 

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// disattivo il tick
	//PrimaryActorTick.bCanEverTick = false;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(true);
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(FVector(0, 0, -10000));

	if (MyGM)
	{
		MyGM->InUseEBullet.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyGM->AvailableEBullet.AddUnique(this);
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}

// Called when the game starts or when spawned
void AEBullet::BeginPlay()
{
	Super::BeginPlay();
	//SetLifeSpan(1.5f); // dopo 1.5 secondi viene rimosso

}



void AEBullet::UpdateLoc(float DeltaTime)
{
	if (Durata > 0)
	{
		Durata -= DeltaTime;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
	}
	else DeActivate();
}
