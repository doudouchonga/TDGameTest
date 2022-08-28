// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game1GameMode.h"
#include "Game1PlayerController.h"
#include "Game1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

AGame1GameMode::AGame1GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGame1PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGame1GameMode::TickGameRole(float DeltaSeconds)
{
	if (bGameOver)
	{
		return;
	}

	if (CurrentPlayer)
	{
		CurrentPlayer->NextMove = CurrentPlayer->PlanAction();
		CurrentPlayer->CheckAction();
	}


}

void AGame1GameMode::SetupPlayer()
{
	// 初始化玩家
	PlayerA = NewObject<UGameStepProxy>(this, "A");
	PlayerA->GameMode = this;
	PlayerA->Key = 0;

	PlayerB = NewObject<UGameStepAIProxy>(this, "B");
	PlayerB->GameMode = this;
	PlayerB->TargetPlayer = PlayerA;
	PlayerB->Key = 1;
	SwitchPlayer();
	// 初始化胜利条件
	SetupWinnerData();
}

void AGame1GameMode::ResetPlayerData()
{
	// 重玩重置
	bGameOver = false;
	PlayerA->NextMove = -1;
	PlayerB->NextMove = -1;
	CurrentPlayer = PlayerA;
	PlayerData.Empty();
}

void AGame1GameMode::SetupWinnerData()
{
	for (int i =0; i< MaxCount; ++i)
	{
		{
			TArray<int> r = GetWinerData(i, 1, 0);
			if (r.Num() == WinNUM)
			{
				WinData.Add(r);
			}
		}
		{
			TArray<int> r = GetWinerData(i, 0, 1);
			if (r.Num() == WinNUM)
			{
				WinData.Add(r);
			}
		}
		
		{
			TArray<int> r = GetWinerData(i, 1, 1);
			if (r.Num() == WinNUM)
			{
				WinData.Add(r);
			}
		}
	}

}

TArray<int> AGame1GameMode::GetWinerData(int pos, int row, int col)
{
	// 模拟所有方向的情况
	// 
	TArray<int> Rets;
	int MaxRow = MaxCount / WinNUM;
	int MaxCol = MaxCount / WinNUM;
	int CurRow = pos / MaxRow;
	int CurCol = pos % MaxCol;

	int Row = CurRow;
	int Col = CurCol;

	while (Row >= 0 && Col >= 0 && Row <= MaxRow - 1 && Col <= MaxCol - 1)
	{
		int pos = WinNUM * Row + Col;
		Rets.AddUnique(pos);
		Row += row;
		Col += col;
	}

	Row = CurRow;
	Col = CurCol;
	while (Row >= 0 && Col >= 0 && Row <= MaxRow - 1 && Col <= MaxCol - 1)
	{
		int pos = WinNUM * Row + Col;
		Rets.AddUnique(pos);
		Row -= row;
		Col -= col;
	}

	/*Row = CurRow;
	Col = CurCol;
	while (Row >= 0 && Col >= 0 && Row <= MaxRow - 1 && Col <= MaxCol - 1)
	{
		int pos = WinNUM * Row + Col;
		Rets.AddUnique(pos);
		Row += row;
		Col -= col;
	}

	Row = CurRow;
	Col = CurCol;
	while (Row >= 0 && Col >= 0 && Row <= MaxRow - 1 && Col <= MaxCol - 1)
	{
		int pos = WinNUM * Row + Col;
		Rets.AddUnique(pos);
		Row -= row;
		Col += col;
	}*/

	return Rets;
}

void AGame1GameMode::SwitchPlayer()
{
	if (CurrentPlayer == nullptr || CurrentPlayer == PlayerB)
	{
		CurrentPlayer = PlayerA;
		return;
	}

	CurrentPlayer = PlayerB;
}

AGame1Character* AGame1GameMode::GetLocalCharacter()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return nullptr;

	AController* LocalCtrl = World->GetFirstPlayerController();
	if (!LocalCtrl)
	{
		return nullptr;
	}
	ACharacter* Ch = Cast<ACharacter>(LocalCtrl->GetCharacter());
	if (!Ch)
	{
		return nullptr;
	}

	return Cast<AGame1Character>(Ch);
}

void AGame1GameMode::SetPlayerPos(int key, int pos, bool set)
{
	// 
	FPlayerData& data = PlayerData.FindOrAdd(key);
	if (set)
	{
		data.Counts.AddUnique(pos);
	}
	else
	{
		if (data.Counts.Contains(pos))
		{
			data.Counts.Remove(pos);
		}
	}

	if (PlayerA && PlayerA->Key == key)
	{
		PlayerA->NextMove = pos;
	}

	if (PlayerB && PlayerB->Key == key)
	{
		PlayerB->NextMove = pos;
	}
}

bool AGame1GameMode::GetPlayerPos(int key, int pos)
{
	if (!PlayerData.Find(key))
	{
		return false;
	}

	FPlayerData& data = PlayerData.FindOrAdd(key);
	
	return data.Counts.Contains(pos);

}

bool AGame1GameMode::CheckGameWinRole(int key)
{
	return CheckGameWinRoleWithData(PlayerData, key);
}

bool AGame1GameMode::CheckGameWinRoleWithData(TMap<int, FPlayerData>& CheckPlayerData, int key)
{
	// 只要胜利条件的情况出现一次，就算胜利
	if (!CheckPlayerData.Find(key))
	{
		return false;
	}
	//
	FPlayerData& elm = CheckPlayerData[key];
	
	for (FWinnerData& WinnerData : WinData)
	{
		bool Win = true;
		for (int i : WinnerData.Counts)
		{
			if (!elm.Counts.Contains(i))
			{
				Win = false;
				continue;
			}

		}

		if (Win)
		{
			bGameOver = true;
			return true;
		}
	}

	return false;
}

int AGame1GameMode::MakeMove(int Key, int selfKey, int& dis /*= 1*/)
{
	// 按照胜利条件的情况来下棋
	TArray<int> CanMove;
	int LastMoveNum = 100;
	for (FWinnerData& WinnerData : WinData)
	{
		TArray<int> steps;
		int MoveNum = 0;
		for (int i : WinnerData.Counts)
		{
			// 如果对手已经下了这个格子，则这个格子对应的所有胜利分支都失败
			if (IsMoveExist(-1, selfKey, i))
			{
				MoveNum = 100;
				steps.Empty();
				break;
			}
			// 自己还没下 则继续下
			if (!IsMoveExist(Key, -1, i))
			{
				steps.Add(i);
				MoveNum++;
			}

		}

		// 步数最少的胜利条件
		if (steps.Num() > 0)
		{
			if (MoveNum < LastMoveNum)
			{
				LastMoveNum = MoveNum;
				CanMove = steps;
			}
		}
		
	}

	if (CanMove.Num() == 0)
	{
		return -1;
	}

	dis = LastMoveNum;
	return CanMove[0];
	
}

bool AGame1GameMode::IsMoveExist(int Key, int selfKey, int pos)
{
	if (PlayerData.Find(Key))
	{
		FPlayerData& data = PlayerData[Key];
		
		if (data.Counts.Contains(pos))
		{
			return true;
		}
	}
	
	if (PlayerData.Find(selfKey))
	{
		FPlayerData& Selfdata = PlayerData[selfKey];
		if (Selfdata.Counts.Contains(pos))
		{
			return true;
		}
	}

	return false;
	
}

bool FPlayerData::CheckDis(int pos, int dis)
{
	for (int i : Counts)
	{
		if (FMath::Abs(i - pos) == dis)
		{
			return true;
		}
	}

	return false;
}
