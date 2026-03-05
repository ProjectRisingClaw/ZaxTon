// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFoe.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h" // per spawn di niagara system
#include "ZaxTon/ZaxMode.h" 
#include "ZaxTon/Effects/Explosion.h"
#include "ZaxTon/Player/PCamera.h" 
#include "EngineUtils.h"

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
	
	// memorizzo il pointer alla camera
	for (TActorIterator<APCamera> CamList(GetWorld()); CamList; ++CamList)
	{ MyCamera = *CamList; }

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
		// valore del seno (che sta tra -1 e 1) per una mia
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
		// deciso da noi smette di avanzare (va alla velocità della camera)
		// poi prosegue
		switch (SubState)
		{
		case 0:
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

			double Dist{ abs(MyCamera->GetActorLocation().X - GetActorLocation().X) };
			//+ Customf1
			if (Dist < Customf1)
			{
				SubState = 1;
				Vel = -MyCamera->GetVel();
				//UE_LOG(LogTemp, Error, TEXT("Cambio stato!"));
			}

		}
		break; // avanza fino ad una certa distanza dal centro visuale
		
	
		case 1:  

			Customf2 -= DeltaTime;

			if (Customf2  > 0)
			{
				SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
			}
			else { Vel = 400.f; SubState = 2; }
			
		break; // sta fermo per un certo periodo (eventualmente spara)


		case 2:  
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		break; // riprende ad avanzare e successivamente esce dallo schermo

		}
		//MyGM->MyCamera;


	break;

	case EWaveMode::EWM_Back:

		// una volta che è arrivato a fondo schermo anche senza uscire
		// una volta uscito dallo schermo in basso torna indietro 
		// ed esce dalla parte alta
		
		switch (SubState)
		{
		case 0:
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

			FVector CamLocation{ MyCamera->GetActorLocation() }; // memorizzo loc camera
			        CamLocation.Z = GetActorLocation().Z;

			FVector Point{ (CamLocation + MyCamera->GetActorUpVector() * -400.f) };  // 400.f

			double Dist{ abs(Point.X - GetActorLocation().X) };
	
			if (Dist < Customf1)
			{
				SubState = 1;
				// Memorizzo l'orientamento iniziale 
				// della nave
				BaseOrientation = GetActorQuat();

				// memorizzo il vettore attorno a cui ruotare
				LoopAxis        = GetActorRightVector();
				// dovrebbe già essere normalizzato
				// ma me ne assicuro per non sbagliare
				LoopAxis.Normalize();
				// memorizzo punto iniziale di rotazione e finale in radianti
				CurrentLoopAngle = 0;
				TargetLoopAngle  = FMath::DegreesToRadians(-180);

				//TargetRotation.is
			
			}

		}
		break; // avanza fino ad una certa distanza dal centro visuale

		case 1:
		{
			// Angolo di rotazione continuo
			//

			// calcolo di quanto incremento la rotazione ogni frame
			CurrentLoopAngle -= DeltaTime * FMath::DegreesToRadians(180);

			// se l'angolo attuale supera quello finale esco
			if (CurrentLoopAngle <= TargetLoopAngle)
			{
				CurrentLoopAngle = TargetLoopAngle;
				Vel *= 8;
				SubState = 2;
			}
		
			// gestione di cambio effettivo dell'orientamento
			// tramite i Quat

			// qui calcolo la mia posizione attorno al pitch
			// non tiene in considerazione l'orientamento iniziale
			// utilizzare direttamente questo risultato
			// mi porterebbe sempre ad allinearmi all'asse Y 
			// del mondo
			FQuat LoopRotation{ LoopAxis,CurrentLoopAngle };
			// calcolo l'orientamento complessivo
			// per farlo con i quat, mi basta
			// moltiplicare l'orientamento su un asse per quello 
			//dello sguardo originale
			FQuat FinalOrientation{ LoopRotation * BaseOrientation };

			// assegno l'orientamento calcolato
			SetActorRotation(FinalOrientation);

			// non faccio avanzare sul forward la nave mentre ruota
			// per non fargli cambiare altezza
			//SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

		}
		break; // sta fermo per un certo periodo (eventualmente spara)


		case 2:
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		}
		break; // riprende ad avanzare e successivamente esce dallo schermo

		}

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
		case EWaveMode::EWM_Straight: break;

		case EWaveMode::EWM_Sinus:
    	Customf1 = MyRow->SinusAmp;
		Customf2 = MyRow->Frequenza;
		//Customf3 = 0; // inizializzo i gradi a zero
		break;

		case EWaveMode::EWM_Wait:
		Customf1 = MyRow->CamDistance;
		Customf2 = MyRow->Counter;
		break;

		case EWaveMode::EWM_Back:
		Customf1 = MyRow->CamDistance;
		Customf2 = MyRow->Counter;
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
		//UE_LOG(LogTemp, Warning, TEXT("Mi rimuovo disponibili"));

		MyGM->InUse.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyGM->Available.AddUnique(this);

		
	}
	// se sto tra quelli in uso, mi rimuovo dalla lista
}