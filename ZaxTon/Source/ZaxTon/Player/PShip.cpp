// Fill out your copyright notice in the Description page of Project Settings.


#include "PShip.h"
// qui includo effettivamente i dati
// della capsula
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h" // per poter utilizzare gli iteratori
#include "PCamera.h"     // includo l'oggetto a cui passare la visuale

// Sets default values
APShip::APShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// pointer                         tipo da creare          // nome scelto
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);

	//memorizzo in una variabile il path dell'asset
	auto Path = TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow01.SM_StarSparrow01'");
	
	// per sicurezza (ad esempio aver dato un path sbagliato) controllo con un cast
	// che il puntatore restituito sia effettivamente a StaticMesh
	auto MyMesh = Cast<UStaticMesh> ( StaticLoadObject(UStaticMesh::StaticClass(),  // tipo dell'oggetto da trovare
		                              nullptr,                                  // riferimeto ad oggetto se serve
		                              Path) );                                    // path dell'asset
	
	if (MyMesh)
	{
		Collision->SetCapsuleSize(64,32);
		Collision->SetHiddenInGame(false); 
		Body->SetStaticMesh(MyMesh);
		Body->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	}

}

// Called when the game starts or when spawned
void APShip::BeginPlay()
{
	Super::BeginPlay();
	
	// Arg1: inizializzo un array di puntatori con lista di oggetti del tipo desiderato 
	// Arg2: il nome dell'array, diventa falso quando l'array è finito
	// Arg3: incremento passo all'elemento successivo dell'array
	for (TActorIterator<APCamera> CamList(GetWorld()); CamList; ++CamList)
	{
		auto MyCamera = *CamList;

		if (MyCamera) // il primo oggetto pcam della lista, va bene dato che ne metto solo uno
		{
			// devo passare la visuale dal giocatore a questo oggetto
			auto MyController{ Cast<APlayerController>(GetController()) };
			MyController->SetViewTargetWithBlend(MyCamera,1.5f);
			// primo argomento, oggetto a cui voglio passare la visuale
			// secondo argomento, in quanto tempo

			return;
		}

	}

}

// Called every frame
void APShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

