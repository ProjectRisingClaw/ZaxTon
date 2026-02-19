// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"
#include "PShip.h" // altrimenti non posso utilizzare variabili e funzioni di questa classe
#include "ZaxTon/Enemies/BaseFoe.h" // includo il nemico per poterlo colpire
#include "Components/SphereComponent.h"

// Sets default values
APBullet::APBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(26);
	Collision->SetHiddenInGame(true);

	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);

	//memorizzo in una variabile il path dell'asset
	auto Path = TEXT("StaticMesh'/Engine/VREditor/TransformGizmo/SM_Sequencer_Node.SM_Sequencer_Node'");

	// per sicurezza (ad esempio aver dato un path sbagliato) controllo con un cast
	// che il puntatore restituito sia effettivamente a StaticMesh
	auto MyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),  // tipo dell'oggetto da trovare
		nullptr,                                  // riferimeto ad oggetto se serve
		Path));                                    // path dell'asset

	if (MyMesh)
	{
		Body->SetStaticMesh(MyMesh);
		Body->SetRelativeScale3D(FVector(3));
	}


}

// 
void APBullet::Activate(FVector SpawnLocation, FRotator SpawnRotation)
{
	// disattivo collisione proiettile 
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// disattivo il tick
	PrimaryActorTick.bCanEverTick = true;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(false);
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	Durata = 1.5; // ripristino durata proiettile

}


void APBullet::DeActivate()
{
	// disattivo collisione proiettile 

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// disattivo il tick
	PrimaryActorTick.bCanEverTick = false;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(true);
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(FVector(0, 0, -10000));

	// mi inserisco tra i disponibili
	auto MyShip{ Cast<APShip>(GetOwner()) }; // controllo che Owner sia di tipo APShip
	if (MyShip)
	{
		MyShip->InUse.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyShip->Available.AddUnique(this);
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	//SetLifeSpan(1.5f); // dopo 1.5 secondi viene rimosso

	// attivo evento di overlap ad inizio gioco
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APBullet::HitEnemy);

}

void APBullet::HitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Foe{ Cast<ABaseFoe>(OtherActor) }; // controllo che in overlap ci sia un basefoe
	if (!Foe) return;

	if (!Cast<USphereComponent>(OtherComp)) return; // desidero l'overlap preciso con la capsula

	Foe->SpawnDieEffect(); // effetto particellare esplosione

	Foe->DeActivate();  // rimuovo nemico
	
	DeActivate(); // rimuovo me stesso


}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// prosegue diritto di fronte a se
	if (Durata > 0)
	{
		Durata -= DeltaTime;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
	}
	else DeActivate();

}

