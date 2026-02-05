// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBullet.generated.h"

UCLASS()
class ZAXTON_API APBullet : public AActor
{
	GENERATED_BODY()
	

	UStaticMeshComponent* Body{ nullptr };
	float Vel{ 3500.f };

public:	
	// Sets default values for this actor's properties
	APBullet();
	//~APBullet();
	// tempo prima di disattivarsi
	float Durata{ 1.5f };

	// attiva grafica e fisica del proiettile
	// da richiamare qunado estratto dalla pool
	void Activate(FVector SpawnLocation,FRotator SpawnRotation);

	// disattiva grafica e fisica del proiettile
	// quando ri messo da parte nella pool
	void DeActivate();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
