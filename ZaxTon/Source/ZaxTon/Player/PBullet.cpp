// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"

// Sets default values
APBullet::APBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);

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


}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(1.5f); // dopo 1.5 secondi viene rimosso
}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// prosegue diritto di fronte a se
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * Vel);

}

