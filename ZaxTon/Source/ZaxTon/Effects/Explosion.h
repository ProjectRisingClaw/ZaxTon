// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ZAXTON_API AExplosion : public AActor
{
	GENERATED_BODY()
	
    // puntatore del componente
	UNiagaraComponent* Comp{ nullptr };

public:	
	// Sets default values for this actor's properties
	AExplosion();

	void Activate(FVector SpawnLocation, FRotator SpawnRotation, UNiagaraSystem* ToSpawn);

	void DeActivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SystemEnd(UNiagaraComponent* FinishedComponent);

/*public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;*/

};
