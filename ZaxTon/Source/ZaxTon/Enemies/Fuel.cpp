// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuel.h"
#include "../Player/PShip.h"
#include "../ZaxMode.h"
#include "../Headers/DataTables.h"

// Sets default values
AFuel::AFuel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);
	Body->SetCollisionObjectType(ECC_ENEMY_OBJ);
	// deve vedere il giocatore come overlap
	Body->SetCollisionResponseToChannel(ECC_PLAYER_OBJ, ECR_Overlap);

	DirezioneX = FMath::RandRange(-1000.f, 1000.f);
	DirezioneY = FMath::RandRange(-1000.f, 1000.f);

}

// Called when the game starts or when spawned
void AFuel::BeginPlay()
{
	Super::BeginPlay();

	auto Path = TEXT("/Game/Import/Croccantini.Croccantini");
	auto MyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, Path));
	Body->SetStaticMesh(MyMesh);
	SetActorRotation(FRotator(0, -90.f, 90.f));
	SetActorScale3D(FVector(.6));
}

// Called every frame
void AFuel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRelativeRotation(GetActorRotation() + FRotator(.0, 1, .0) * DeltaTime * 100.f);

	FVector RandomDirection(DirezioneX, DirezioneY, 0.f);

	RandomDirection.Normalize();

	AddActorWorldOffset(RandomDirection * DeltaTime * 300.f);
}

void AFuel::Activate(FVector Posizione)
{
	SetActorLocation(Posizione);
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Body->SetHiddenInGame(false);
	MyGM->InUseFuel.AddUnique(this);
	GetWorld()->GetTimerManager().SetTimer(FtimerHandleRespawn, this, &AFuel::DeActivate, 4.f, false);
}

void AFuel::DeActivate()
{
	SetActorLocation(FVector(0, 0, 100));
	Body->SetHiddenInGame(true);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// mi inserisco tra i disponibil
	if (MyGM)
	{

		MyGM->InUseFuel.Remove(this); // remove toglie un elemento da un array, se lo trova
		MyGM->AvailableFuel.AddUnique(this);
	}

}

