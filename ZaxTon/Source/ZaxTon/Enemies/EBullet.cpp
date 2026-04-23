// Fill out your copyright notice in the Description page of Project Settings.


#include "EBullet.h"
#include "ZaxTon/Player/PShip.h" // altrimenti non posso utilizzare variabili e funzioni di questa classe
#include "BaseFoe.h" // includo il nemico per poterlo colpire
#include "Components/SphereComponent.h"
#include "ZaxTon/Headers/DataTables.h" 
#include "ZaxTon/Headers/Enumerators.h"
#include "ZaxTon/ZaxMode.h" 
// gestione particellari
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


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

	FBulletTableRaw* MyRow{ MyDT->FindRow<FBulletTableRaw>(AttackType,TEXT("Context")) };

	if (MyRow) // ad ogni activate carico dati dalla tabella
	{
		Body->SetStaticMesh(MyRow->Mesh);
		Body->SetRelativeScale3D(FVector(MyRow->Scale));
		Vel = MyRow->Vel;
		BulletKind = MyRow->BulletKind;
		VfxComp->SetAsset(MyRow->MoveFX);


		Degree = MyRow->Degree;
		Number = MyRow->SpreadNumber;
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

		if (BulletKind == EBulletKind::EBK_Laser)
		{
			VfxComp->SetVariableVec3("EndPoint", GetActorLocation());
		}

		Durata -= DeltaTime;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
	}
	else DeActivate();
}
