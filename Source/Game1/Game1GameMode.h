// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AI/Game1AIProxy.h"
#include "Game1GameMode.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillBPEvent, FName, Key, FName, Value);


USTRUCT(BlueprintType)
struct FCountRoleData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int Num;

	UPROPERTY()
	TArray<int> Counts;
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<int> Counts;
	// 
	
	bool CheckDis(int pos, int dis);
};

USTRUCT(BlueprintType)
struct FWinnerData
{
	GENERATED_USTRUCT_BODY()

	FWinnerData(){}

	FWinnerData(TArray<int> data): Counts(data){}

	UPROPERTY()
	TArray<int> Counts;
};


UCLASS(minimalapi)
class AGame1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGame1GameMode();

	UFUNCTION(BlueprintCallable)
	void TickGameRole( float DeltaSeconds );

	UFUNCTION(BlueprintCallable)
	void SetupPlayer();

	UFUNCTION(BlueprintCallable)
	void ResetPlayerData();

	void SetupWinnerData();

	TArray<int> GetWinerData(int pos, int row, int col);

	UPROPERTY()
		UGameStepProxy* PlayerA;
	UPROPERTY()
		UGameStepProxy* PlayerB;

	UPROPERTY()
		UGameStepProxy* CurrentPlayer;

	void SwitchPlayer();

	class AGame1Character* GetLocalCharacter();
public:
	UPROPERTY()
	TArray<FWinnerData> WinData;

	UPROPERTY()
	TMap<int, FPlayerData> PlayerData;

	UFUNCTION(BlueprintCallable)
	void SetPlayerPos(int key, int pos, bool set);
	UFUNCTION(BlueprintPure)
	bool GetPlayerPos(int key, int pos);

	UFUNCTION(BlueprintCallable)
	bool CheckGameWinRole(int key);
	
	bool CheckGameWinRoleWithData(TMap<int, FPlayerData>& CheckPlayerData, int key);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int WinNUM{3};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxCount{9};

	int MakeMove(int key, int selfKey, int& dis);

	bool IsMoveExist(int key, int selfKey, int pos);
	
	bool bGameOver{false};
};



