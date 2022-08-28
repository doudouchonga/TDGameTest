// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game1GamePlayDelegate.h"
#include "Modules/ModuleManager.h"

UGame1GamePlayDelegates::UGame1GamePlayDelegates()
{

}

UGame1GamePlayDelegates::~UGame1GamePlayDelegates()
{

}

void UGame1GamePlayDelegates::Deinitialize()
{

}

void UGame1GamePlayDelegates::BroadcastBPEvent(FName Key, FName Value)
{
	OnBPEvent.Broadcast(Key, Value);
}
