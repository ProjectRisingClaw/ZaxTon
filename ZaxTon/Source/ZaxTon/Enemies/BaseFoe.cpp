// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFoe.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h" // per spawn di niagara system
#include "ZaxTon/ZaxMode.h" 
#include "ZaxTon/Effects/Explosion.h"

// Sets default values
ABaseFoe::ABaseFoe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// pointer                         tipo da creare          // nome scelto
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
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
		Collision->SetSphereRadius(64);
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


void ABaseFoe::UpdateLoc(float DeltaTime)
{
	switch (WaveMode)
	{
	case EWaveMode::EWM_Straight:
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		break;

	case EWaveMode::EWM_Sinus:
	{
		// customf3 qui mi serve per identificare i gradi
		// se Customf2 vale 360 ci metterò 1 secondo a compiere un 
		// oscillazione
		Customf3 += Customf2 * DeltaTime;
		//float Sin{ FMath::Sin(FMath::DegreesToRadians(Customf3)) };

		float Sin{ MyGM->sinLUT[uint8(Customf3)] };
		float Amp = Sin * Customf1;  // moltiplico il
		// valore del seno (che sta ra -1 e 1) per una mia
		// variabile che rappresenta la larghezza dell'oscillazione
		// si muove seguendo una sinusoide
		// posso decidere l'ampiezza
		FVector Loc = GetActorLocation();
		//	FVector BLoc{ Loc }; // memorizzo locazione prima di cambiarla
		Loc.X -= Vel * DeltaTime;
		Loc.Y = StartLocation.Y + Amp; // sommo sull'asse Y per ottenere effetto sinus


		SetActorLocation(Loc);
		/*FVector DLoc{Loc - BLoc};
		SetActorRotation(DLoc.Rotation());*/

		//float Seno = FMath::sin();
		//SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

	}
	break;

	case EWaveMode::EWM_Wait:
		// va dritto ma entrato nello schermo, per un tempo
		//deciso da noi smette di avanzare (va alla velocità della camera)
		// poi prosegue
		switch (SubState)
		{
		case 0:   break; // avanza fino ad una certa distanza dal centro visuale
		case 1:   break; // sta fermo per un certo periodo (eventualmente spara)
		case 2:   break; // riprende ad avanzare e successivamente esce dallo schermo
		}
		//MyGM->MyCamera;


	break;

	case EWaveMode::EWM_Back:

		// una volta che è arrivato a fondo scherm oanche senza uscire

		// una volta uscito dallo schermo in basso torna indietro 
		// ed esce dalla parte alta
		break;

	}
}






// 
void ABaseFoe::Activate(FVector SpawnLocation, FRotator SpawnRotation, FName NewType)
{
	
	//PrimaryActorTick.bCanEverTick = true;
	StartLocation = SpawnLocation; // salvo locazione al momento di essere attivato

	SubState = 0; // se serve gestire dei sotto stati, azzero l'indice dello switch interno 
	// carico dati dalla DT
	FEnemyTableRaw* MyRow{ MyDT->FindRow<FEnemyTableRaw>(NewType,TEXT("Context")) };
	if (MyRow)
	{
		Body->SetStaticMesh(MyRow->Mesh);     // copio valore della mesh da DT
		ExplosionEffect =   MyRow->ExplosionFX; // copio valore VFX da Data table;
		WaveMode        =   MyRow->WaveMode;
		
		// a seconda del tipo di ondata inzializo i due valori custom
		// in maniera differente
		switch (WaveMode)
		{
		case EWaveMode::EWM_Straight:

		break;

		case EWaveMode::EWM_Sinus:
    	Customf1 = MyRow->SinusAmp;
		Customf2 = MyRow->Frequenza;
		//Customf3 = 0; // inizializzo i gradi a zero
		break;

		case EWaveMode::EWM_Wait:
		break;

		case EWaveMode::EWM_Back:
		break;


		}


	}
	//

	// disattivo collisione proiettile 
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(false);
	// posiziono l'oggetto in una zona lontana da quella di azione
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	//Durata = 1.5; // ripristino durata proiettile

}

void ABaseFoe::SpawnDieEffect()
{
	if (!ExplosionEffect) return; // controllo di sicurezza se non ho definito il particellare esco

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect,GetActorLocation());

	//auto MyGM{ Cast<AZaxMode>(GetOwner()) };
	if (MyGM)
	{
		auto NewEffect{ MyGM->AvailableEffects.Pop() };
		if (!NewEffect) return;
		NewEffect->Activate(GetActorLocation(),FRotator(0),ExplosionEffect);
	}
}


void ABaseFoe::DeActivate()
{
	UE_LOG(LogTemp, Warning, TEXT("chiamo deactive"))

	// disattivo collisione proiettile 
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// disattivo il tick
	//PrimaryActorTick.bCanEverTick = false;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(true);
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(FVector(0, 0, -10000));

	// mi inserisco tra i disponibili
	//auto MyGM{ Cast<AZaxMode>(GetOwner()) }; // controllo che Owner sia di tipo APShip
	if (MyGM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mi rimuovo disponibili"));

		MyGM->InUse.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyGM->Available.AddUnique(this);

		
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}