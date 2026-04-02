// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EBullet.generated.h"

class USphereComponent;
class AZaxMode;

UCLASS()
class ZAXTON_API AEBullet : public AActor
{
	GENERATED_BODY()
	
	USphereComponent* Collision{ nullptr };

	UStaticMeshComponent* Body{ nullptr };

	float Vel{ 3500.f };

	UDataTable* MyDT{ nullptr }; // Data Table da cui prendere i dati del proiettile

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
