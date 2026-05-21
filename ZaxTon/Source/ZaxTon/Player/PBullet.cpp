// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"
#include "PShip.h" // altrimenti non posso utilizzare variabili e funzioni di questa classe
#include "ZaxTon/Enemies/BaseFoe.h" // includo il nemico per poterlo colpire
#include "Components/SphereComponent.h"
#include "ZaxTon/Headers/DataTables.h" 
#include "ZaxTon/ZaxMode.h"
#include "ZaxTon/Effects/Explosion.h" // include per l'actor che gestisce esplosioni semplici
// gestione particellari
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APBullet::APBullet()
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

	auto Path = TEXT("/Game/DataTables/BPPlayerBulletTable");
	MyDT = LoadObject<UDataTable>(nullptr, Path); // recupero la DT tramite Path

	/*
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
	*/

}

// 
void APBullet::Activate(FVector SpawnLocation, FRotator SpawnRotation, FName AttackType)
{

	// flag che indica se attivare inseguimento per questo proiettile
	if (AttackType == "SpecialBullet")
	{
		bFollow = true;
		Durata = 4.2f; // ripristino durata proiettile
		Timer  = 0.22f; // il tempo da passare andando dritto
	}
	else
	{
		bFollow = false;
		Durata = 1.f;
	}

	substate = 0; // reset del sotto stato

	// controlla la riga della tabella e carica dati
	FBulletTableRaw* MyRow{ MyDT->FindRow<FBulletTableRaw>(AttackType,TEXT("Context")) };

	if (MyRow) // ad ogni activate carico dati dalla tabella
	{

		Body->SetStaticMesh(MyRow->Mesh);
		Body->SetRelativeScale3D(FVector(MyRow->Scale));
		Vel = MyRow->Vel;
		VfxComp->SetAsset(MyRow->MoveFX); // assegno al componente il particellare in loop
		NSImpact = MyRow->ExplosionFX;
	
		//VfxImpact->SetAsset(MyRow->ExplosionFX); // effetto da passare all'actor delle esposioni
	}

	// disattivo collisione proiettile 
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// deve vedere il giocatore come overlap
	Collision->SetCollisionResponseToChannel(ECC_ENEMY_OBJ, ECR_Overlap);

	//PrimaryActorTick.bCanEverTick = true;
	// nascondo grafica del proiettile
	Body->SetHiddenInGame(false);
	// posiziono l'ogggetto in una zona lontana da quella di azione
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);


	VfxComp->SetVariableVec3("StartPoint", GetActorLocation());
	//VfxComp->SetVariableVec3("StartPoint", GetActorLocation());
	VfxComp->SetVariableVec3("EndPoint",   GetActorLocation());

	VfxComp->Activate();
	VfxComp->ActivateSystem();

}


void APBullet::DeActivate()
{
	// disattivo collisione proiettile 
	VfxComp->Deactivate();


	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// disattivo il tick
	//PrimaryActorTick.bCanEverTick = false;
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

	// in ogni caso con l'impatto, creo un oggetto per i particellari

	if (MyGM->AvailableEffects.Num() > 0)
	{
		auto Effect{ MyGM->AvailableEffects.Pop() };
		Effect->Activate(GetActorLocation(), FRotator(0), NSImpact);
	}
	
	Foe->Hitted(); // informo il nemico di essere stato colpito, gestira lui rimozione HP

	//Foe->HitPoint 

	VfxComp->DeactivateImmediate();
	DeActivate();          // rimuovo me stesso


}


ABaseFoe* APBullet::GetRandomActiveEnemy()
{
	int8 Size{ int8(MyGM->InUse.Num()) };

	if (Size > 0)
	{ 
		return MyGM->InUse[FMath::RandRange(0, Size - 1)];

	}
    else return nullptr;
}


ABaseFoe* APBullet::GetCloseActiveEnemy()
{

	ABaseFoe* TargetOut  { nullptr };
	float     MinDistance{ 5000000 };

	for (int i = 0; i < MyGM->InUse.Num(); i++)
	{
		float TempDist = (GetActorLocation() - MyGM->InUse[i]->GetActorLocation()).Size();
	
		if (TempDist < MinDistance)
		{
			MinDistance = TempDist;
			TargetOut = MyGM->InUse[i];
		}
	}

	return TargetOut;
}


void APBullet::UpdateLoc(float DeltaTime)
{

	if (Durata > 0)
	{
		Durata -= DeltaTime;

		if (bFollow)
		{  // gestisco inseguimento nemico più vicino
			switch (substate)
			{
			case 0: // prosegue diritto per un tot di secondi

			Target = nullptr;
			if (Timer > 0)Timer -= DeltaTime; else substate = 1;

			FRotator ActualRot{ GetActorRotation() };
			ActualRot.Pitch = FMath::FInterpConstantTo(ActualRot.Pitch,0,DeltaTime,40);
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
			SetActorRotation(ActualRot);

			break;


			case 1: // decide nemico da colpire
			Target = GetCloseActiveEnemy();
			substate = 2;
			if (!Target) { substate = 0; Timer = 1.f;}

			break;


			case 2: // finche il nemico esiste lo insegue.

				if (Target) 
				{
					//UE_LOG(LogTemp, Warning, TEXT("look for path"));

					//Target->GetActorLocation();
					//FRotator NewRot{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()) };		
					// trovo angolazione desiderata
					FRotator NewRot((Target->GetActorLocation() - GetActorLocation()).Rotation());	
					// ruoto ma lentamente
					NewRot = FMath::RInterpConstantTo(GetActorRotation(), NewRot, DeltaTime,360);
					SetActorRotation(NewRot);
					SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel*2);

					if (!MyGM->InUse.Contains(Target))
					{ Target = nullptr; }


			    } else substate = 1;

			break;





			}


			//float Lunghezz{ (1.f - Durata) * 500.f };
			VfxComp->SetVariableVec3("EndPoint", GetActorLocation() - GetActorForwardVector());
			//VfxComp->SetVariableVec3("EndPoint", GetActorLocation());



		}
		else  // proiettile normale va semplicemente diritto
		{

			float Lunghezza{ (1.f - Durata) * 500.f };
			VfxComp->SetVariableVec3("StartPoint", GetActorLocation() - GetActorForwardVector() * Lunghezza);
			VfxComp->SetVariableVec3("EndPoint", GetActorLocation());
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);
		}


		
	}
	else
	{
		
		VfxComp->DeactivateImmediate();
		DeActivate();
	}
}

