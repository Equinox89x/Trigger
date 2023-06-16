// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Menu.h"

APS_Menu::APS_Menu()
{
}

void APS_Menu::NotifyBeginPlay()
{
}

void APS_Menu::SetScoreDetails(bool fullCombo, float drawSpeed, float averageShootSpeed, float averageAccuracy, float score, float speedMultiplier)
{
	FullCombo = fullCombo;
	DrawSpeed = drawSpeed;
	AverageShootSpeed = averageShootSpeed;
	AverageAccuracy = averageAccuracy;
	Combined = score + (fullCombo ? 200 : 0);
	SpeedMultiplier = speedMultiplier;
	Total = Combined * speedMultiplier;

	IsGameEnded = true;
}