#pragma once

//FName TipiNemici[10]{ "Striker","Eagle","Bomber","Ambush","Flip","Killer","Destroyer","XX","YY","ZZ" };

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	EET_Striker   UMETA(DisplayName = "Striker"),
	EET_Eagle     UMETA(DisplayName = "Eagle"),
	EET_Bomber    UMETA(DisplayName = "Bomber"),
	EET_Ambush    UMETA(DisplayName = "Ambush"),
	EET_Flip      UMETA(DisplayName = "Flip"),
	EET_Killer    UMETA(DisplayName = "Killer"),
	EET_Destroyer UMETA(DisplayName = "Destroyer"),
	EET_MAX       UMETA(DisplayName = "MAX")

};

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	EET_PlayerA UMETA(DisplayName = "Player A"),
	EET_PlayerB UMETA(DisplayName = "Player B"),
	EET_PlayerC UMETA(DisplayName = "Player C"),
	EET_PlayerD UMETA(DisplayName = "Player D"),
	EET_MAX     UMETA(DisplayName = "MAX")

};


UENUM(BlueprintType)
enum class EWaveMode : uint8
{
	EWM_Straight UMETA(DisplayName = "Straight"),
	EWM_Sinus    UMETA(DisplayName = "Sinus"),
	EWM_Wait     UMETA(DisplayName = "Wait"),
	EWM_Back     UMETA(DisplayName = "Back"),
	EWM_Spin     UMETA(DisplayName = "Spin"),
	EWM_MAX      UMETA(DisplayName = "MAX")

};


UENUM(BlueprintType)
enum class EOrientVector : uint8
{
	EOV_Forward UMETA(DisplayName = "Turn Around Forward (Roll)"),
	EOV_Right   UMETA(DisplayName = "Turn Around Right (Pitch)"),
	EOV_Up      UMETA(DisplayName = "Turn Around Up (Yaw)"),
	EOV_MAX     UMETA(DisplayName = "MAX")

};


UENUM(BlueprintType)
enum class EBulletKind : uint8
{

	EBK_Normal   UMETA(DisplayName = "Normal"), // un proiettile semplice che va dritto
	EBK_Follow   UMETA(DisplayName = "Follow"), // un proiettile che insegue il nemico più vicino
	EBK_Spiral   UMETA(DisplayName = "Spiral"), // un proiettile lento con traiettoria a spirale
	EBK_Laser    UMETA(DisplayName = "Laser"), // un fascio di tipo line trace, che colpisce per tutta un linea
	EBK_Spread   UMETA(DisplayName = "Spread"), // proiettili che si dirigono in varie direzioni
	EBK_Speed    UMETA(DisplayName = "Speed"), // un proiettile normale di base più rapido
	EBK_MAX      UMETA(DisplayName = "MAX")

};
