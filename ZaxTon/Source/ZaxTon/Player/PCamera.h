// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCamera.generated.h"

UCLASS()
class ZAXTON_API APCamera : public AActor
{
	GENERATED_BODY()
	

	UStaticMeshComponent* Body{ nullptr };

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox{ nullptr };

	UPROPERTY(EditAnyWhere)
	float Vel{ 100.f };

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	// Sets default values for this actor's properties
	APCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
