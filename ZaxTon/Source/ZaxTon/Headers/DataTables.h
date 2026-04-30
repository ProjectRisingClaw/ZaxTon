#pragma once
#include "Engine/DataTable.h"
#include "Enumerators.h"
#include "NiagaraSystem.h"
#include "DataTables.generated.h"

#define ECC_PLAYER_TRACE ECC_GameTraceChannel1
#define ECC_PLAYER_OBJ   ECC_GameTraceChannel2
#define ECC_ENEMY_OBJ    ECC_GameTraceChannel3
#define ECC_ENEMY_TRACE  ECC_GameTraceChannel4


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
		meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait || WaveMode ==  EWaveMode::EWM_Back || WaveMode ==  EWaveMode::EWM_Spin", EditConditionHides) )
	float CamDistance{ 300 };

	// variabile da sfruttare per definire un tempo limitato
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait  || WaveMode ==  EWaveMode::EWM_Spin", EditConditionHides))
	float EnemyDelay{ 1.f };

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
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Back  || WaveMode ==  EWaveMode::EWM_Spin", EditConditionHides))
	float RotationSpeed{ 360.f };

	// numero di proiettili, con zero non spara
	// a seconda del tipo di ondata i proiettili potrebbero essere 
	//sparati in momenti differenti
	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait", EditConditionHides))
	uint8 BulletNumber{ 0 };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait || WaveMode ==  EWaveMode::EWM_Spin", EditConditionHides))
	float BulletDelay{ 0.2f }; // tempo che intercorre tra un proiettile e l'altro

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
	meta = (EditCondition = "WaveMode ==  EWaveMode::EWM_Wait || WaveMode ==  EWaveMode::EWM_Spin", EditConditionHides))
	EBulletKind BulletKind{ EBulletKind::EBK_Normal };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	uint8 HitPoint{ 1 };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UMaterial* HitMaterial{ nullptr };

};

USTRUCT(BlueprintType)
struct FPlayerTableRaw: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UStaticMesh* Mesh{ nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UNiagaraSystem* ExplosionFX{ nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float Vel{ 1200.f };

};

USTRUCT(BlueprintType)
struct FBulletTableRaw : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,meta = (EditCondition = "BulletKind !=  EBulletKind::EBK_Spread", EditConditionHides))
	UStaticMesh* Mesh{ nullptr };

	// effetto particellare per impatto del proiettile
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (EditCondition = "BulletKind !=  EBulletKind::EBK_Spread", EditConditionHides))
	UNiagaraSystem* ExplosionFX{ nullptr };

	// effetto particellare per scia del proiettile
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (EditCondition = "BulletKind !=  EBulletKind::EBK_Spread", EditConditionHides))
	UNiagaraSystem* MoveFX{ nullptr };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (EditCondition = "BulletKind !=  EBulletKind::EBK_Spread", EditConditionHides))
	float Vel{ 1200.f };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	EBulletKind BulletKind{ EBulletKind::EBK_Normal };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (EditCondition = "BulletKind !=  EBulletKind::EBK_Spread", EditConditionHides))
	float Scale{ 1.f };


	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "BulletKind ==  EBulletKind::EBK_Spread", EditConditionHides))
	float Degree{ 20.f };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "BulletKind ==  EBulletKind::EBK_Spread", EditConditionHides))
	uint8 SpreadNumber{ 3 };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "BulletKind ==  EBulletKind::EBK_Laser", EditConditionHides))
	float Wait{ 3.f };

	UPROPERTY(EditAnyWhere, BlueprintReadOnly,
		meta = (EditCondition = "BulletKind ==  EBulletKind::EBK_Laser", EditConditionHides))
	float Distance{ 500.f };



};