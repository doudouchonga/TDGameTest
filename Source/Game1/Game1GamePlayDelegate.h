// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "Game1GamePlayDelegate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBPEvent, FName, Key, FName, Value);

UCLASS()
class UGame1GamePlayDelegates : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UGame1GamePlayDelegates();
	virtual ~UGame1GamePlayDelegates();
	UFUNCTION(BlueprintCallable)
		static UGame1GamePlayDelegates* Get(UObject* WorldContextObject)
	{
		UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
		if (World)
		{
			return Cast<UGame1GamePlayDelegates>(World->GetSubsystemBase(UGame1GamePlayDelegates::StaticClass()));
		}
		return nullptr;
	}

	virtual void Deinitialize() override;

public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnBPEvent OnBPEvent;

	UFUNCTION(BlueprintCallable)
		void BroadcastBPEvent(FName Key, FName Value);
};