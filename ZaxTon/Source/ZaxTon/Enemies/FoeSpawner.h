// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditAnyWhere, Category = "Spawn")
	int   NumFoe{ 1 };

	UPROPERTY(EditAnyWhere, Category = "Spawn")
	float Vel{ 300.f };

	UPROPERTY(EditAnyWhere, Category = "Spawn")
	float Gap{ 200.f };



	UFUNCTION()
	void Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AZaxMode* MyGameMode;

// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
