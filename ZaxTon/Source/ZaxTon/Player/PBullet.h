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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
