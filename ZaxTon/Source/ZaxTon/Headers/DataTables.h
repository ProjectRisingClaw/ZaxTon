#pragma once
#include "Engine/DataTable.h"
#include "Enumerators.h"
#include "NiagaraSystem.h"
#include "DataTables.generated.h"

USTRUCT(BlueprintType)
struct FEnemyTableRaw : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UStaticMesh*    Mesh{ nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UNiagaraSystem* ExplosionFX{ nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	EWaveMode  WaveMode{ EWaveMode::EWM_Straight };
	

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Sinus", EditConditionHides) )
	float SinusAmp{ 100 };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Sinus", EditConditionHides))
	float Frequenza{ 100 };

	// distanza dalla camera a cui fermarsi in modalità Wait
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait || WaveMode ==  EWaveMode::EWM_Back", EditConditionHides) )
	float CamDistance{ 300 };

	// variabile da sfruttare per definire un tempo limitato
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait", EditConditionHides))
	float Counter{ 1.f };

	/* Valore utilizzato per identificar eun punto ad un certa distanza dalla camera (di solito asse X) */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Back", EditConditionHides))
	float CameraOffset{ -400.f };


	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Back", EditConditionHides))
	EOrientVector OrientVector{ EOrientVector::EOV_Right };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Back", EditConditionHides))
	bool bMoveWhileRotating{ false };

	/* di quanti gradi deve ruotare durante l'inversione*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Back", EditConditionHides))
	float AngleAmp{ 180.f };

	/* 360 significa che si compie una rivoluzione completa in 1 secondo*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Back", EditConditionHides))
	float RotationSpeed{ 360.f };

};