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
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float SinusAmp{ 100 };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float Frequenza{ 100 };

	// distanza dalla camera a cui fermarsi in modalità Wait
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float CamDistance{ 300 };

	// variabile da sfruttare per definire un tempo limitato
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float Counter{ 1.f };
};