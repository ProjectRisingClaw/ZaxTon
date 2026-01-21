// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PShip.generated.h"

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

public:
	// Sets default values for this pawn's properties
	APShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
