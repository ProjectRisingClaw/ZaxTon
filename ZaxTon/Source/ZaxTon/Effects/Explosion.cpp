// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "ZaxTon/ZaxMode.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Comp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Comp"));
	SetRootComponent(Comp);
	Comp->SetAutoDestroy(false);
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();	
	// bind di evento fine particellare
	Comp->OnSystemFinished.AddDynamic(this,&AExplosion::SystemEnd);
}

// evento che sarà richiamato af ine effetto particellare
void AExplosion::SystemEnd(UNiagaraComponent* FinishedComponent)
{
	DeActivate();
}

// cose da fare quando si attiva l'actor per i particellari
void AExplosion::Activate(FVector SpawnLocation, FRotator SpawnRotation, UNiagaraSystem* ToSpawn)
{
	// disattivo collisione proiettile 
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);

	Comp->SetAsset(ToSpawn); // assegno al componente l'effett oda utilizzare
	Comp->ActivateSystem(); // attivo l'effetto sul componente

	//Comp->DeactivateImmediate();
	//Durata = 1.5; // ripristino durata proiettile

}


void AExplosion::DeActivate()
{

	Comp->Deactivate();
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(FVector(0, 0, -10000));

	// mi inserisco tra i disponibili
	
	auto MyGM{Cast<AZaxMode>(GetOwner())}; // controllo che Owner sia di tipo APShip
	if (MyGM)
	{
		MyGM->AvailableEffects.AddUnique(this);
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}


// Called every frame
/*void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

