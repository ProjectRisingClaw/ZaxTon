#pragma once


UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	EET_NemicoA UMETA(DisplayName = "Nemico A"),
	EET_NemicoB UMETA(DisplayName = "Nemico B"),
	EET_NemicoC UMETA(DisplayName = "Nemico C"),
	EET_NemicoD UMETA(DisplayName = "Nemico D"),
	EET_NemicoE UMETA(DisplayName = "Nemico E"),
	EET_MAX     UMETA(DisplayName = "MAX")

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
	EBK_MAX      UMETA(DisplayName = "MAX")

};
