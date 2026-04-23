// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZaxTon/Headers/Enumerators.h"
#include "EBullet.generated.h"

class USphereComponent;
class AZaxMode;
class UNiagaraSystem;
class UNiagaraComponent;
class APCamera;

UCLASS()
class ZAXTON_API AEBullet : public AActor
{
	GENERATED_BODY()
	
	USphereComponent* Collision{ nullptr };

	UStaticMeshComponent* Body{ nullptr };

	float Vel{ 3500.f };

	UDataTable* MyDT{ nullptr }; // Data Table da cui prendere i dati del proiettile

	EBulletKind BulletKind{ EBulletKind::EBK_Normal } ;

	float Degree;

	uint8 Number;

	UNiagaraComponent* VfxComp{ nullptr };

	float Wait{ 5.f }; // quando un proiettile deve stare fermo per un pò in una certa posizione

	float Distance{ 500.f }; // quando devo misurare la distanza tra il proiettile e un punto di origine
	
	uint8 substate{ 0 }; // eventuale sotto stato per proiettili che richiedono più fasi

	APCamera* MyCamera;

public:
	// Sets default values for this actor's properties
	AEBullet();
	//~APBullet();
	// tempo prima di disattivarsi
	float Durata{ 1.5f };

	// attiva grafica e fisica del proiettile
	// da richiamare qunado estratto dalla pool
	void Activate(FVector SpawnLocation, FRotator SpawnRotation, FName AttackType = "NormalBullet");

	// disattiva grafica e fisica del proiettile
	// quando ri messo da parte nella pool
	void DeActivate();

	AZaxMode* MyGM{ nullptr };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame


	void UpdateLoc(float DeltaTime);

};
