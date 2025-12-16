// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFPSPlayerState.h"
#include "FPS.h"

AMyFPSPlayerState::AMyFPSPlayerState()
{
	// 初始化分数
	CurrentScore = 0.0f;
}

void AMyFPSPlayerState::AddScore(float Amount)
{
	CurrentScore += Amount;
	
	UE_LOG(LogFPS, Log, TEXT("Player %s Score Updated: New Score = %f (Added %f)"), *GetPlayerName(), CurrentScore, Amount);
}
