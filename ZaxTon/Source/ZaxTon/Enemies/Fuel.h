// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fuel.generated.h"

class AZaxMode;

UCLASS()
class ZAXTON_API AFuel : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent* Body{ nullptr };

	
public:	
	// Sets default values for this actor's properties
	AFuel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AZaxMode* MyGM{ nullptr };

	virtual void Activate(FVector Posizione);
	virtual void DeActivate();

};
