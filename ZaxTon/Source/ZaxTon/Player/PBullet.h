// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBullet.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class AZaxMode;
class ABaseFoe;

UCLASS()
class ZAXTON_API APBullet : public AActor
{
	GENERATED_BODY()
	

	// collisione effettiva per il proiettile (root)
	USphereComponent*     Collision{ nullptr };

	// eventuale mesh statica per grafica del proiettile
	UStaticMeshComponent* Body{ nullptr };

	// effetto persistente del proiettile
	UNiagaraComponent*    VfxComp{ nullptr };


	UNiagaraSystem*       NSImpact{ nullptr };

	float Vel{ 3500.f };

	UDataTable* MyDT{ nullptr }; // Data Table da cui prendere i dati del proiettile

	// se vero attivo opzione di inseguimento avversario
	bool      bFollow{ false };
	uint8     substate{ 0 }; // per scandire le varie fasi dell'attacco a inseguimento
	
	//ABaseFoe* Target{ nullptr }; // l'eventuale obbiettivo da seguire
	TObjectPtr<ABaseFoe> Target;
	//FName TipiNemici[10]{ "NemicoA","NemicoB","NemicoC","NemicoD","NemicoE","","","","","" };

public:	
	// Sets default values for this actor's properties
	APBullet();
	//~APBullet();
	// tempo prima di disattivarsi
	float Durata{ 1.5f };

	float Timer{ 0.3f }; // timer generico multi purpose

	// attiva grafica e fisica del proiettile
	// da richiamare qunado estratto dalla pool
	void Activate(FVector SpawnLocation,FRotator SpawnRotation, FName AttackType = "NormalBullet");

	

	// disattiva grafica e fisica del proiettile
	// quando ri messo da parte nella pool
	void DeActivate();


	AZaxMode* MyGM{ nullptr };// riferimento al game mode

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() // forzato per ogni funzione di cui fare bind
	void HitEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	ABaseFoe* GetRandomActiveEnemy();

	ABaseFoe* GetCloseActiveEnemy();

public:	
	// Called every frame


	void UpdateLoc(float DeltaTime);

};
