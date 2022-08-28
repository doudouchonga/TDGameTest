// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game1AIProxy.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Game1Character.h"
#include "../Game1GameMode.h"

int UGameStepProxy::PlanAction()
{
	return NextMove;
}

void UGameStepProxy::CheckAction()
{
	if (!GameMode.IsValid())
	{
		return ;
	}

	AGame1GameMode* GS = Cast<AGame1GameMode>(GameMode.Get());
	if (!GS)
	{
		return ;
	}

	if (NextMove != -1)
	{
		if (AGame1Character* Char = GS->GetLocalCharacter())
		{
			Char->PlayerMove(Key, NextMove);
		}

		NextMove = -1;
		GS->SwitchPlayer();
	}
}

UGameStepAIProxy::UGameStepAIProxy()
{
	isAi = true;
}

int UGameStepAIProxy::PlanAction()
{
	if (!TargetPlayer.IsValid())
	{
		return -1;
	}

	if (!GameMode.IsValid())
	{
		return -1;
	}

	AGame1GameMode* GS = Cast<AGame1GameMode>(GameMode.Get());
	if (!GS)
	{
		return -1;
	}
	// �����ֵ��ƶ�
	int MoveNum = 0;
	int NextStep = GS->MakeMove(TargetPlayer->Key, Key, MoveNum);

	// MAKE MOVE
	// �ٿ����Լ����ƶ�
	int MyMoveNum = 0;
	int MyNextStep = GS->MakeMove(Key, TargetPlayer->Key, MyMoveNum);

	// �Լ�һ��Ӯ�������ִ��
	if (MyMoveNum == 1 && MyNextStep >= 0)
	{
		return MyNextStep;
	}
	// �������һ��Ӯ���Ѷ��ֵĲ�����
	if (MoveNum == 1 && NextStep >= 0)
	{
		return NextStep;
	}
	//
	if (MyNextStep >= 0)
	{
		return MyNextStep;
	}

	if (NextStep >= 0)
	{
		return NextStep;
	}

	return -1;
	

}
