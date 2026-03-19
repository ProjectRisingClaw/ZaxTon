// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZaxTon/Headers/Enumerators.h"
#include "FoeSpawner.generated.h"

UCLASS()
class ZAXTON_API AFoeSpawner : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnyWhere)
	class USphereComponent* SpawnPoint{ nullptr };
	
	UPROPERTY(VisibleAnyWhere)
	class UArrowComponent* Direction{ nullptr };

public:	
	// Sets default values for this actor's properties
	AFoeSpawner();

	/* Numero Di Nemici da creare  */
	UPROPERTY(EditAnyWhere, Category = "Spawn")
	int   NumFoe{ 1 };

	/* Velcoita di base di movimento */
	UPROPERTY(EditAnyWhere, Category = "Spawn")
	float Vel{ 300.f };

	/* Distanza tra un nemico e l'altro */
	UPROPERTY(EditAnyWhere, Category = "Spawn")
	float Gap{ 200.f };

	/* Eventuale sfasamento tra i nemici (in caso di sinusoidi o similari */
	UPROPERTY(EditAnyWhere, Category = "Spawn")
	float Phase{ 30.f };


	UPROPERTY(EditAnyWhere, Category = "Spawn")
	EEnemyType EnemyType{ EEnemyType::EET_NemicoA };


	FName TipiNemici[10]{ "NemicoA","NemicoB","NemicoC","NemicoD","NemicoE","","","","","" };


	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AZaxMode* MyGameMode;

// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
