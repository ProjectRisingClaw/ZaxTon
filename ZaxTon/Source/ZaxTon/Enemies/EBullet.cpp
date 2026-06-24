// Fill out your copyright notice in the Description page of Project Settings.


#include "EBullet.h"
#include "ZaxTon/Player/PShip.h" // altrimenti non posso utilizzare variabili e funzioni di questa classe
#include "BaseFoe.h" // includo il nemico per poterlo colpire
#include "Components/SphereComponent.h"
#include "ZaxTon/Headers/DataTables.h" 
#include "ZaxTon/ZaxMode.h" 
// gestione particellari
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


#include "ZaxTon/Player/PCamera.h"
#include "EngineUtils.h"

AEBullet::AEBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(26);
	Collision->SetHiddenInGame(true);
	// tipo di obj per il proiettile
	Collision->SetCollisionObjectType(ECC_ENEMY_OBJ);
	// deve vedere il giocatore come overlap
	Collision->SetCollisionResponseToChannel(ECC_PLAYER_OBJ, ECR_Overlap);

	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);
	Body->CastShadow      = false;
	Body->bReceivesDecals = false;
	// creazione componetne Niagara
	VfxComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VfxComp"));
	VfxComp->SetupAttachment(Collision);


	auto Path = TEXT("/Game/DataTables/BPEnemyBulletTable.BPEnemyBulletTable");
	MyDT = LoadObject<UDataTable>(nullptr, Path); // recupero la DT tramite Path


}

// 
void AEBullet::Activate(FVector SpawnLocation, FRotator SpawnRotation, FName AttackType)
{

	// controlla la riga della tabella e carica dati

//	UE_LOG(LogTemp,Error,TEXT("proiettile attivato = %s") , *AttackType.ToString())

	// attack type qui è il nome proiettile
	FBulletTableRaw* MyRow{ MyDT->FindRow<FBulletTableRaw>(AttackType,TEXT("Context")) };

	substate = 0; // reset di sotto stato 

	if (MyRow) // ad ogni activate carico dati dalla tabella
	{
		Body->SetStaticMesh(MyRow->Mesh);
		Body->SetRelativeScale3D(FVector(MyRow->Scale));
		Vel = MyRow->Vel;
		BulletKind = MyRow->BulletKind;
		VfxComp->SetAsset(MyRow->MoveFX);

		// spread bullet
		Degree = MyRow->Degree;
		Number = MyRow->SpreadNumber;
		// laser bullet
		Distance = MyRow->Distance;
		Wait     = MyRow->Wait;

	}

	// disattivo collisione proiettile 
	if (BulletKind == EBulletKind::EBK_Spread) // se il proiettile è di tipo spread
	{ // non ha una funzione diretta. quello che farà sarà a sua volta generare un gruppo di proiettili
		// normali in varie direzioni  (basandosi su variabili di gradi e quantità)
		
		// imposto un offset negativo
		float OffsetRot = -Degree * uint8(Number/2);
	
		if (Number % 2 == 0) // correggo l'offset in caso di proiettili pari
		{ OffsetRot += Degree / 2; }

		 // partendo dall'offset negativo incremetno i gradi ad ogni iterazione del ciclo
		for (int i = 0; i < Number; i++)
		{	
			if (MyGM->AvailableEBullet.Num()>0)
			{
			    AEBullet* NewBull{ MyGM->AvailableEBullet.Pop() };	
				FRotator BaseRot{ SpawnRotation };
				BaseRot.Yaw += OffsetRot + Degree * i;
				NewBull->Activate(SpawnLocation, BaseRot);
				MyGM->InUseEBullet.AddUnique(NewBull); // inserisco l'oggetto attivato nella lista in uso
			}
			else UE_LOG(LogTemp, Error, TEXT("nessun proiettile disponibile"));
		}

		// subito dopo averli creati si disattiva.
		DeActivate();
	}
	else
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Collision->SetCollisionObjectType(ECC_ENEMY_OBJ);
		//PrimaryActorTick.bCanEverTick = true;
		// nascondo grafica del proiettile
		Body->SetHiddenInGame(false);
		// posiziono l'ogggetto in una zona lontana da quella di azione
		SetActorLocation(SpawnLocation);
		SetActorRotation(SpawnRotation);
		Durata = 5.f; // ripristino durata proiettile
	
		switch (BulletKind)
		{
		case EBulletKind::EBK_Normal:
			VfxComp->SetVariableVec3("EndPoint", GetActorLocation());
			VfxComp->Activate();
			VfxComp->ActivateSystem();
		break;

		case EBulletKind::EBK_Follow:
		break;

		case EBulletKind::EBK_Spiral:
		break;

		case EBulletKind::EBK_Laser:
			VfxComp->SetVariableVec3("StartPoint", GetActorLocation());
			//VfxComp->SetVariableVec3("StartPoint", GetActorLocation());
			VfxComp->SetVariableVec3("EndPoint", GetActorLocation());

			VfxComp->Activate();
			VfxComp->ActivateSystem();
		break;

	

	
		}

		



	}
}


void AEBullet::DeActivate()
{
	// disattivo VFX ( se non era stato attivato non dovrebbe succeder nulla)
	VfxComp->ResetSystem();
	VfxComp->DeactivateImmediate();
	
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
		// memorizzo il pointer alla camera
	for (TActorIterator<APCamera> CamList(GetWorld()); CamList; ++CamList)
	{
		MyCamera = *CamList;
	}
}



void AEBullet::UpdateLoc(float DeltaTime)
{

	switch (BulletKind)
	{

	case EBulletKind::EBK_Follow:
		break;
	case EBulletKind::EBK_Spiral:
		break;
	case EBulletKind::EBK_Laser:
	{

		FVector StartPoint{ GetOwner()->GetActorLocation() };

		if (Wait > 0)  Wait -= DeltaTime;
		else           substate = 2;

		switch (substate)
		{
		case 0: // avanzo fino a raggiungere la distanza desiderata
		
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

			FVector Diff{ GetActorLocation() - StartPoint };
			if (Diff.Size() > Distance)
			{
				substate = 1;
				Vel = MyCamera->GetVel();
			}

			//Distance 

		break;

		case 1: // attivo il counter e aspetto il tempo di wait

			SetActorLocation(GetActorLocation() + MyCamera->GetActorUpVector() * DeltaTime * Vel);

			break;

		case 2: // mi disattivo 
			DeActivate();
			break;

		}



		// controllo di collisione tra i due punti con linetrace
		FHitResult MyHit; // variabile contenitore di informazioni sull'oggetto colpito
		// la passero per riferimento e la funzione  ci inserirà i dati necessari

		//FCollisionQueryParams Parametri;

		//Parametri.AddIgnoredActors(Cast<AActor>(MyGM->InUse));

		//GetWorld()->SweepSingleByChannel(,);

		//GetWorld()->LineTrace

		bool Hitsomething = GetWorld()->LineTraceSingleByChannel(
			MyHit,
			StartPoint,      // punti di inziio e fine per riferimento ma const
			GetActorLocation(), // serve solo a non copiare la variabile, il limite è che non posso mettere direttamente un valore
			ECC_PLAYER_TRACE);// ECC_PLAYER

		

		if (Hitsomething)
		{
			//UE_LOG(LogTemp, Error, TEXT("Hit Something %s"), *MyHit.GetActor()->GetName());
			APShip* ToDestroy{ Cast<APShip>(MyHit.GetActor()) };
			if (ToDestroy) ToDestroy->HitGeneral();

		}

		// aggiornamento grafico dei particellari
		VfxComp->SetVariableVec3("EndPoint", GetActorLocation());
		VfxComp->SetVariableVec3("StartPoint", StartPoint);

	}
    break;
	
	default:

		if (Durata > 0)
		{	
			Durata -= DeltaTime;
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
			VfxComp->SetVariableVec3("EndPoint", GetActorLocation());
		}
		else DeActivate();


	}

	


}
