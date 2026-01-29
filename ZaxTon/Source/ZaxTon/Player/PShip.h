// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PShip.generated.h"


class APBullet; // forward declaration per bullet, dato che lo includo solo nel cpp

UCLASS()
class ZAXTON_API APShip : public APawn
{
	GENERATED_BODY()

	// puntatori vuoti per i componenti da aggiungere
	// un solo putatore creato non vuol dire
	// avere il componente, è solo il contenitore
	// componente static mesh
	UStaticMeshComponent*    Body{ nullptr };
	//UStaticMeshComponent* Body = nullptr;

	class UCapsuleComponent* Collision{ nullptr };

	// funzioni gestione movimento
	void  MoveForward(float Input);

	void  MoveRight(float Input);

	FORCEINLINE void  FireBullet()        { bFireOn = true; };  // inizio fuoco
	FORCEINLINE void  FireBulletRelease() { bFireOn = false; }; // fine fuoco

	// variabili in cui memorizzare gli input
	// e renderli visibili in altre funzioni
	float left { 0 };
	float front{ 0 };

	float Vel{ 900.f }; // velocità della nave

	// vera se il fuco è attivo
	bool  bFireOn{ false };
	float FireRate{8.f}; // fire rte ovver oil numero di colpi al secondo 

	float FireTime{ 0.f }; // variabile counter , in sostituzione di un eventuale timer di unreal

	//FTimerHandle FireHandle; // nel caso in cui io desideri usare un timer per il fire rate
	// mi serve un handle per poter spegnere il timer al momento del rilascio


	// gestione di pool per spawn proiettili
	TArray<APBullet*> Available; // proiettili disponibili
	TArray<APBullet*> InUse;     //proiettili correntemente in uso


public:
	// Sets default values for this pawn's properties
	APShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ManageFire(float DeltaTime);

	void SpawnBullet();

	void ManageMove(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
