#pragma once


UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	EET_NemicoA UMETA(DisplayName = "Nemico A"),
	EET_NemicoB UMETA(DisplayName = "Nemico B"),
	EET_NemicoC UMETA(DisplayName = "Nemico C"),
	EET_NemicoD UMETA(DisplayName = "Nemico D"),
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