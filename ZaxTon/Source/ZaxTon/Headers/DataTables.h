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

};