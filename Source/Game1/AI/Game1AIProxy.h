// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Game1AIProxy.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillBPEvent, FName, Key, FName, Value);


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UGameStepProxy : public UObject
{
	GENERATED_BODY()
public:
	
	int Key;

	int NextMove{-1};

	bool isAi{false};
	
	virtual int PlanAction();
	void CheckAction();
	
	//
	UPROPERTY()
		TWeakObjectPtr<class UGameStepProxy> TargetPlayer;

	UPROPERTY()
		TWeakObjectPtr<class AGame1GameMode> GameMode;

};



UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UGameStepAIProxy : public UGameStepProxy
{
	GENERATED_BODY()
public:
	UGameStepAIProxy();

	virtual int PlanAction();


};


