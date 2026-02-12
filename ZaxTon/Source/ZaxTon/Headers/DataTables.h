#pragma once
#include "Engine/DataTable.h"
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


};