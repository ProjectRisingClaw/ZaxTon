// Fill out your copyright notice in the Description page of Project Settings.


#include "PShip.h"
// qui includo effettivamente i dati
// della capsula
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h" // per poter utilizzare gli iteratori
#include "PCamera.h"     // includo l'oggetto a cui passare la visuale
#include "GameFramework/PlayerInput.h" // per selezione tasti input
#include "PBullet.h"

// funzione di init input
static void InitDefaultKeys()
{
	// comandi direzione avanti e indietro
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveFW", EKeys::Gamepad_LeftY));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveFW", EKeys::W, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveFW", EKeys::S, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveFW", EKeys::Gamepad_DPad_Down , -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveFW", EKeys::Gamepad_DPad_Up,    1));

	// comandi sinistra destra
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRT", EKeys::Gamepad_LeftX));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRT", EKeys::A, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRT", EKeys::D, 1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRT", EKeys::Gamepad_DPad_Left, -1));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRT", EKeys::Gamepad_DPad_Right, 1));

	// tasto di fuoco
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Fire", EKeys::LeftMouseButton));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Fire", EKeys::SpaceBar));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Fire", EKeys::Gamepad_FaceButton_Bottom));

}







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

	//auto Path = TEXT("StaticMesh'/Game/StarSparrow/Meshes/Examples/SM_StarSparrow01.SM_StarSparrow01'"); // funziona ugualmente ma non più necessario
	auto Path = TEXT("/Game/StarSparrow/Meshes/Examples/SM_StarSparrow01");
	//auto Path = TEXT("SM_StarSparrow01");
	// 
	// per sicurezza (ad esempio aver dato un path sbagliato) controllo con un cast
	// che il puntatore restituito sia effettivamente a StaticMesh


	auto MyMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),  // tipo dell'oggetto da trovare
		nullptr,                                  // riferimeto ad oggetto se serve
		Path));

	                               // path dell'asset
	if (MyMesh)
	{
		Collision->SetCapsuleSize(64,32);
		Collision->SetHiddenInGame(true); 
		Body->SetStaticMesh(MyMesh);
		Body->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
		Body->CastShadow = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" no mesh "));
	}

}

// Called when the game starts or when spawned
void APShip::BeginPlay()
{
	Super::BeginPlay();
	
	// inizializzo il mio array "pool"
	// creao 15 proiettili e li inserisco tra quelli disponibili per essere sparati
	for (int i = 0; i < 15; i++)
	{
		// creo istanza
		auto bull{ GetWorld()->SpawnActor<APBullet>(APBullet::StaticClass()) };
		bull->SetOwner(this); // come creo il proiettile
		// gli lascio un riferimento all'oggetto che lo ha creato
		bull->DeActivate(); // disattivo istanza
	
		//Available.AddUnique(bull); // inserisco in array dei disponibili.
	}
	
	// il fire rate messo come vaolre iniziale si intende in 
	// proiettil ial secondo. qui lo ricaloclolo come 1/ numero di proiettili
	FireRate = 1 / FireRate;

	// Arg1: inizializzo un array di puntatori con lista di oggetti del tipo desiderato 
	// Arg2: il nome dell'array, diventa falso quando l'array è finito
	// Arg3: incremento passo all'elemento successivo dell'array
	for (TActorIterator<APCamera> CamList(GetWorld()); CamList; ++CamList)
	{
		MyCamera = *CamList;

		if (MyCamera) // il primo oggetto pcam della lista, va bene dato che ne metto solo uno
		{
			// devo passare la visuale dal giocatore a questo oggetto
			auto MyController{ Cast<APlayerController>(GetController()) };
			MyController->SetViewTargetWithBlend(MyCamera, 1.f);
			// primo argomento, oggetto a cui voglio passare la visuale
			// secondo argomento, in quanto tempo


			// memorizzo la distanza iniziale in z tra spawn point della nave
			// e oggetto camera, questa distanza (salvo necessita di gameplay)
			// dovrà essere mantenuta
			CamOffset.Z = GetActorLocation().Z - MyCamera->GetActorLocation().Z;


			return;
		}
	}
	
}

// Called every frame
void APShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ManageMove(DeltaTime);
	
	ManageFire(DeltaTime);

}

void APShip::ManageFire(float DeltaTime)
{
	if (bFireOn)
	{
		// FireTime parte da 0 e quando raggiunge il valore di fire rat, rilascio un proiettile e lo azzero nuovamente
		if (FireTime < FireRate) FireTime += DeltaTime;
		else
		{
			FireTime = 0;
			SpawnBullet();
		}

	}
}

void APShip::SpawnBullet()
{
	// dovendo far spawn di una classe che ha già in CPP grafica
	// e tutto il necessario, non serve avere un riferimento da BP
	// in questo caso UClass si crea con Nomeclass::StaticClass()
	FVector SpawnLocation{ GetActorLocation() + GetActorForwardVector() * 100 }; // trovo posizione spawn
	//classe  // riferimento uclass    // locazione   // rotazione di spawn
	//GetWorld()->SpawnActor<APBullet>(APBullet::StaticClass(), SpawnLocation, GetActorRotation());

	// per eseguire spawn dalla pool controllo che ci siano elementi disponibili e 
	// nel caso li attivo posizionandoli nella locazione giusta

	if (Available.Num() > 0) // se c'è almeno un elemento
	{
		auto NewBull{ Available.Pop() }; // tramite pop estraggo elemento dall'array
		NewBull->Activate(SpawnLocation, GetActorRotation()); // attivo oggetto posizionandolo
		// nella locazione desiderata
		InUse.AddUnique(NewBull); // memorizzo l'indirizzo dell'istanza nella lista "in uso"
	
	
	}

}


/*
void APShip::ManageMove(float DeltaTime)
{
	FVector Position{ GetActorLocation() }; // salvo posizione attuale
	// creo vettore temporaneo utilizzando i valori 
	// catturati in input
	FVector Direction{ front,left,0 };
	Direction.Normalize(); // normalizzo il vettore portandol oa dimensione 1

	// aggiorno la posizione, seguendo il vettore combinato 
	SetActorLocation(Position + (Direction * Vel * DeltaTime), true);
}*/


void APShip::ManageMove(float DeltaTime)
{
	FVector Direction{ front,left,0 };
	Direction.Normalize(); // normalizzo il vettore portandol oa dimensione 1

	// non baso più la posizione della nave su la propria locazione 
	//+  (Direction * Vel * DeltaTime), ma utilizzo questi valori
	// per modificare lo scarto tra ca,era e nave
	
	CamOffset += (Direction * Vel * DeltaTime);
	
	// sequenza di CLAMP per impedire all'offset (ovvero la distanza tra nave e camera)
	// di superare certi range pre impostati 
	if (CamOffset.X >  YLimit) CamOffset.X =  YLimit;
	if (CamOffset.X < -YLimit) CamOffset.X = -YLimit;
	if (CamOffset.Y >  XLimit) CamOffset.Y =  XLimit;
	if (CamOffset.Y < -XLimit) CamOffset.Y = -XLimit;
	
	// la nuova locazione sarà uguale alla posizione della camera + lo
	// scarto attuale
	SetActorLocation(MyCamera->GetActorLocation() + CamOffset,true);

	//FVector Position{ GetActorLocation() }; // salvo posizione attuale
	// creo vettore temporaneo utilizzando i valori 
	// catturati in input
	

	// aggiorno la posizione, seguendo il vettore combinato 
	//SetActorLocation(Position + (Direction * Vel * DeltaTime), true);
}


// Called to bind functionality to input
void APShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InitDefaultKeys(); 
	// bind di evento pressione con funzioni corrispondenti
	InputComponent->BindAxis("MoveFW", this, &APShip::MoveForward);
	InputComponent->BindAxis("MoveRT", this, &APShip::MoveRight);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APShip::FireBullet);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &APShip::FireBulletRelease);
	//GetWorld()->SpawnActor<APBullet>(APBullet::StaticClass(),,)

}

// nelle funzioni input semplicemente memorizzo l'ultimo valore
void  APShip::MoveForward(float Input)
{
	front = Input;
}
	

void  APShip::MoveRight(float Input)
{ 
	left = Input; 	
}

