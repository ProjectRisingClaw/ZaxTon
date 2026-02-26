// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZaxTon/Headers/DataTables.h"
#include "BaseFoe.generated.h"

class USphereComponent;
class AZaxMode;

UCLASS()
class ZAXTON_API ABaseFoe : public AActor
{
	GENERATED_BODY()
	
	// puntatori vuoti per i componenti da aggiungere
	// un solo putatore creato non vuol dire
	// avere il componente, è solo il contenitore
	// componente static mesh
	UStaticMeshComponent* Body{ nullptr };
	//UStaticMeshComponent* Body = nullptr;

	USphereComponent* Collision{ nullptr };
	
	UNiagaraSystem* ExplosionEffect;

	float Vel{ 300.f };

	// puntatore alla Data Table da cui prelevare le informazioni
	UDataTable* MyDT{ nullptr };

	EWaveMode  WaveMode{ EWaveMode::EWM_Straight };

	FVector StartLocation{ 0,0,0 }; // locazione iniziale
	// set di variabili custom da utilizzare per diversi
	// scopi a seconda del tipo di ondata
	float Customf1{ 0.f }; // ampiezza curva	
	float Customf2{ 0.f }; // vel curva
	float Customf3{ 0.f }; // utilizzo per gradi attuali

	int8  Customi1{ 0   }; //-128 + 128

	uint8 SubState{ 0 }; // utilizzato per switch dei "sotto stati" di un singolo comportamento

public:	

	FORCEINLINE void SetCustom3(float NewVal) { Customf3 = NewVal; };

	AZaxMode* MyGM{ nullptr };

	// Sets default values for this actor's properties
	ABaseFoe();

	void DeActivate();

	void Activate(FVector SpawnLocation, FRotator SpawnRotation, FName NewType);
	
	FORCEINLINE void SetVel(float NewVel) { Vel = NewVel; };
	
	void SpawnDieEffect();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	

	void UpdateLoc(float DeltaTime);

};
