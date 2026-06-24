// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFoe.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h" // per spawn di niagara system
#include "ZaxTon/ZaxMode.h" 
#include "ZaxTon/Effects/Explosion.h"
#include "ZaxTon/Player/PCamera.h" 
#include "EngineUtils.h"
#include "EBullet.h"
// Sets default values
ABaseFoe::ABaseFoe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// pointer                         tipo da creare          // nome scelto
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// definisco il collision object type del nemico
	Collision->SetCollisionObjectType(ECC_ENEMY_OBJ);
	// deve vedere il giocatore come overlap
	Collision->SetCollisionResponseToChannel(ECC_PLAYER_OBJ, ECR_Overlap);

	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);

	// path della DataTable  per i nemici
	auto Path = TEXT("/Game/DataTables/BPEnemyTable");

	MyDT =  LoadObject<UDataTable>(nullptr, Path) ; // recupero la DT tramite Path
	// vado a trovare la riga che mi interessa sulla DT
	FEnemyTableRaw* MyRow{ MyDT->FindRow<FEnemyTableRaw>(FName("Striker"),TEXT("Context")) };

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


bool ABaseFoe::Hitted()
{
	//UMaterialInstanceDynamic* Coso;
	//Coso->Set
	//Body->CreateDynamicMaterialInstance()

	if (HitPoint > 1)
	{
	    // Creo qui l'istanza dinamica di un materiale, partendo da un materiale statico
		HitDynamic = Body->CreateDynamicMaterialInstance(0,HitMaterial);
		ContLum = 1.f;
		HitDynamic->SetScalarParameterValue("Luminosita", ContLum);
		Body->SetMaterial(0, HitDynamic);

		//FTimerHandle TempTimer;
		GetWorldTimerManager().SetTimer(TempTimer, this, &ABaseFoe::ResetMaterial, 0.1f);
		HitPoint--;

		return false;
	}
	else
	{
		SpawnDieEffect(); // effetto particellare esplosione
		DeActivate();     // rimuovo nemico


		return true;
	}
}

void ABaseFoe::UpdateLoc(float DeltaTime)
{
	switch (WaveMode)
	{
	case EWaveMode::EWM_Straight: WaveStraight(DeltaTime); break;
	case EWaveMode::EWM_Sinus:    WaveSinus(DeltaTime);    break;
	case EWaveMode::EWM_Wait:     WaveWait(DeltaTime);     break;
	case EWaveMode::EWM_Back:     WaveBack(DeltaTime);     break;
	case EWaveMode::EWM_Spin:     WaveSpin(DeltaTime);     break;
	}

	if (GetWorldTimerManager().IsTimerActive(TempTimer)){
		ContLum += DeltaTime * 300;
		HitDynamic->SetScalarParameterValue("Luminosita", ContLum);
		//UE_LOG(LogTemp, Error, TEXT("ContLum: %f"), ContLum);
	}
}

void ABaseFoe::WaveStraight(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
}

void ABaseFoe::WaveSinus(float DeltaTime)
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

void ABaseFoe::WaveWait(float DeltaTime)
{
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
			Vel = MyCamera->GetVel(); // velocità uguale e contraria alla camera
			//UE_LOG(LogTemp, Error, TEXT("Cambio stato!"));
		}

	}
	break; // avanza fino ad una certa distanza dal centro visuale


	case 1: // in questo stato è fermo ed è qui che dovrà sparare

		// scala il tempo in cui stare fermo
		Customf2 -= DeltaTime;

		if (Customf2  > 0)
		{   // si mette in lock con la camera
			SetActorLocation(GetActorLocation() + MyCamera->GetActorUpVector() * DeltaTime * Vel);
		}
		else { Vel = 400.f; SubState = 2; } // assegno nuovamente velcoita in avanti

		// gestione spawn dei proiettili ( se ci sono proiettili )

		if (Customui1 <= 0) break;

		if (Counter > 0) Counter -= DeltaTime;
		else
		{		
	
				if (MyGM->AvailableEBullet.Num() > 0)
				{ FireBullet();}

			Customui1 -= 1;

			Counter    = Customf3; // rimetto il contatore alla dimensione del Dealy
		}

	break; // sta fermo per un certo periodo (eventualmente spara)


	case 2:
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		break; // riprende ad avanzare e successivamente esce dallo schermo

	}
	//MyGM->MyCamera;
}

void ABaseFoe::WaveSpin(float DeltaTime)
{
	// va dritto ma entrato nello schermo, per un tempo
	// deciso da noi smette di avanzare (va alla velocità della camera)
	// poi prosegue
	

	// Customf1  - Distanza dalla camera a cui fermarsi
	// Customf2  - tempo complessivo di attesa in wait
	// Customf3  - intervallo tempo per sparare
	// Customf4  - vel  rotazione

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
			Vel = MyCamera->GetVel(); // velocità uguale e contraria alla camera
			//UE_LOG(LogTemp, Error, TEXT("Cambio stato!"));

			BaseOrientation = GetActorQuat(); // prima di cambiare sottostato
			// memoirzzo orientamento attuale in Quat
			CurrentLoopAngle = 0; // l'angolazione iiziale a zero
 		}

	}
	break; // avanza fino ad una certa distanza dal centro visuale


	case 1: // in questo stato è fermo ed è qui che dovrà sparare

		// ruoto usando i QUAT
		CurrentLoopAngle += DeltaTime * FMath::DegreesToRadians(Customf4);

		FQuat LoopRotation{ GetActorUpVector(),CurrentLoopAngle};
		// calcolo l'orientamento complessivo
		// per farlo con i quat, mi basta
		// moltiplicare l'orientamento su un asse per quello 
		//dello sguardo originale
		FQuat FinalOrientation{ LoopRotation * BaseOrientation };
		// assegno l'orientamento calcolato
		SetActorRotation(FinalOrientation);

		// scala il tempo in cui stare fermo
		Customf2 -= DeltaTime;

		if (Customf2 > 0)
		{   // si mette in lock con la camera
			SetActorLocation(GetActorLocation() + MyCamera->GetActorUpVector() * DeltaTime * Vel);
		}
		else { Vel = 400.f; SubState = 2; } // assegno nuovamente velcoita in avanti

		// gestione spawn dei proiettili ( se ci sono proiettili )

		if (Counter > 0) Counter -= DeltaTime;
		else
		{
			if (MyGM->AvailableEBullet.Num() > 0)
			{
				FireBullet();
			}
			Counter = Customf3; // rimetto il contaatore alla dimensione del Dealy
		}

		break; // sta fermo per un certo periodo (eventualmente spara)


	case 2:
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		break; // riprende ad avanzare e successivamente esce dallo schermo

	}
	//MyGM->MyCamera;
}

void ABaseFoe::WaveBack(float DeltaTime)
{
	// una volta che è arrivato a fondo schermo anche senza uscire
	// una volta uscito dallo schermo in basso torna indietro 
	// ed esce dalla parte alta

	// customf1 distanzatra noi e la camera sotto la quale si passa di stato
	// Customf2 offset per calcolare la distanza
	// Customf3  gradi da ruotare prima di smettere (360 gradi rotazione completa)

	switch (SubState)
	{
	case 0:
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

		FVector CamLocation{ MyCamera->GetActorLocation() }; // memorizzo loc camera
		CamLocation.Z = GetActorLocation().Z;

		// posizione precisa rispetto alla camera, dove si passa di stato
		FVector Point{ (CamLocation + MyCamera->GetActorUpVector() * Customf2) };  // 400.f

		//DrawDebugSphere(GetWorld(), Point, Customf1,32,FColor::Red);

		double  Dist{ abs(Point.X - GetActorLocation().X) };

		if (Dist < Customf1)
		{
			SubState = 1;
			// Memorizzo l'orientamento iniziale 
			// della nave
			BaseOrientation = GetActorQuat();

			//UE_LOG(LogTemp, Error, TEXT(" Orient Quat = %s"), *BaseOrientation.ToString());
			//UE_LOG(LogTemp, Error, TEXT(" Orient Rot = %s"), *BaseOrientation.Rotator().ToString());
			// memorizzo il vettore attorno a cui ruotare
			//LoopAxis        = GetActorForwardVector();
			// dovrebbe già essere normalizzato
			// ma me ne assicuro per non sbagliare
			//LoopAxis.Normalize();
			// memorizzo punto iniziale di rotazione e finale in radianti
			CurrentLoopAngle = 0;
			TargetLoopAngle = FMath::DegreesToRadians(Customf3);

			if (Customui1 > 0) // primo attacco, prima del turn
			{
				FireBullet();
				Customui1 -= 1;
				
			}

		}

	}
	break; // avanza fino ad una certa distanza dal centro visuale

	case 1:
	{
		// Angolo di rotazione continuo
		// calcolo di quanto incremento la rotazione ogni frame

		// se l'angolo attuale supera quello finale esco
		//0  -1 -2 -3     //-180

		if (TargetLoopAngle > 0)  // se maggiore di zero la nave ruota alla sua sinistra 
		{
			CurrentLoopAngle += DeltaTime * FMath::DegreesToRadians(Customf4);
			if (CurrentLoopAngle >= TargetLoopAngle)
			{
				CurrentLoopAngle = TargetLoopAngle;
				Vel *= 8; //aumento la velocità di 8 volte per tornare indietro rapidamente
				SubState = 2;
			}
		}
		else
		{
			CurrentLoopAngle -= DeltaTime * FMath::DegreesToRadians(Customf4);
			if (CurrentLoopAngle <= TargetLoopAngle)
			{
				CurrentLoopAngle = TargetLoopAngle;
				Vel *= 8; //aumento la velocità di 8 volte per tornare indietro rapidamente
				SubState = 2;

			

			}
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
		if (bCustomBool)
		{
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		}
	}
	break;

	case 2:

		if (Customui1  > 0) // secondo attacco dopo il turn in caso di altri proiettili
		{
			FireBullet();
			Customui1 -= 1;	
		}

		SubState = 3;
	break;

	case 3:
	{ SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel); }
	break; // riprende ad avanzare e successivamente esce dallo schermo

	}
}




void ABaseFoe::FireBullet()
{

		if (MyGM->AvailableEBullet.Num() > 0)
		{	
		//AEBullet* NewBull{ MyGM->AvailableEBullet.Pop() };
		LastBullet = MyGM->AvailableEBullet.Pop();

      	FVector SpawnLocation{ GetActorLocation() + GetActorForwardVector() * 100 };
		LastBullet->Activate(SpawnLocation, GetActorRotation(), BulletName);
		LastBullet->SetOwner(this); // do al proiettile un riferimento a chi lo ha attivatoà

		// controllo su Wait e posizione statica nemico
		if (LastBullet->GetWait() > Customf2) LastBullet->SetWait(Customf2);

		// 

		MyGM->InUseEBullet.AddUnique(LastBullet); // inserisco l'oggetto attivato nella lista in uso
		}
		else UE_LOG(LogTemp, Error, TEXT("Nessun proiettile disponibile"));
	
}


// 
void ABaseFoe::Activate(FVector SpawnLocation, FRotator SpawnRotation, FName NewType)
{
	//PrimaryActorTick.bCanEverTick = true;
	StartLocation = SpawnLocation; // salvo locazione al momento di essere attivato
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);


	SubState = 0; // se serve gestire dei sotto stati, azzero l'indice dello switch interno 
	// carico dati dalla DT
	FEnemyTableRaw* MyRow{ MyDT->FindRow<FEnemyTableRaw>(NewType,TEXT("Context")) };
	if (MyRow)
	{
		Body->SetStaticMesh(MyRow->Mesh);     // copio valore della mesh da DT
		ExplosionEffect =   MyRow->ExplosionFX; // copio valore VFX da Data table;
		WaveMode        =   MyRow->WaveMode;
		HitPoint        =   MyRow->HitPoint; // memorizzo i colpi che si possono subire
		HitMaterial     =   MyRow->HitMaterial;

		StandardMaterial = Cast<UMaterial>(MyRow->Mesh->GetMaterial(0));
		ResetMaterial();

		switch (MyRow->OrientVector)
				{
				case EOrientVector::EOV_Forward: LoopAxis   = GetActorForwardVector();  break;
				case EOrientVector::EOV_Right:   LoopAxis   = GetActorRightVector();    break;
				case EOrientVector::EOV_Up:      LoopAxis   = GetActorUpVector();       break;			
				}

		LoopAxis.Normalize();
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
		Customf2 = MyRow->EnemyDelay;

		Customui1 = MyRow->BulletNumber; // numero di colpi da sparare
		Customf3  = MyRow->BulletDelay;  //tempo tra un colpo e l'altro

		break;

		case EWaveMode::EWM_Back:
		Customf1    = MyRow->CamDistance;
		Customf2    = MyRow->CameraOffset;  
		Customf3    = MyRow->AngleAmp;
		Customf4    = MyRow->RotationSpeed;

		Customui1   = MyRow->BulletNumber; // numero di colpi da sparare se si hanno

		bCustomBool = MyRow->bMoveWhileRotating;
		break;


		case EWaveMode::EWM_Spin:
			Customf1 = MyRow->CamDistance;
			Customf2 = MyRow->EnemyDelay;
			Customf3 = MyRow->BulletDelay;
			Customf4 = MyRow->RotationSpeed;
	    break;
		}

		// il proiettile lo setto in ogni caso per tutti
		// se non viene usato no nsuccede nulla
		switch (MyRow->BulletKind)
		{
		case EBulletKind::EBK_Normal: BulletName = "NormalBullet"; break;
		case EBulletKind::EBK_Laser:  BulletName = "LaserBullet";  break;
		case EBulletKind::EBK_Follow:   break;
		case EBulletKind::EBK_Spiral:   break;
		case EBulletKind::EBK_Spread: BulletName = "SpreadBullet"; break;
		case EBulletKind::EBK_Speed:  BulletName = "SpeedBullet";  break;
			//case EBulletKind::EBK_: BulletName = "SpreadBullet"; break;
		}



	}
	//

	// disattivo collisione proiettile 
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(false);
	// posiziono l'oggetto in una zona lontana da quella di azione

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
	// nel caso del laser, quando il nemico si disattiva, si porta dietro il proiettile
	if (BulletName == "LaserBullet" && LastBullet) LastBullet->DeActivate();

	LastBullet = nullptr; // svuoo pointer per evitare riferimenti a vecchi proiettili precedenti

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
	ResetMaterial();
}


void ABaseFoe::ResetMaterial()
{
	if (StandardMaterial)
	{
		Body->SetMaterial(0, StandardMaterial);
		UE_LOG(LogTemp, Error, TEXT("resetmat %s"), *StandardMaterial->GetName());
	}
}