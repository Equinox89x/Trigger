// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Menu.h"

AGS_Menu::AGS_Menu()
{
	//required for replication
	//bReplicates = true;
	//bNetLoadOnClient = true;
}

void AGS_Menu::NotifyBeginPlay()
{}

//required for replication
//void AGS_Menu::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
//{
//	//DOREPLIFETIME(AGS_Menu, Seconds);
//	DOREPLIFETIME(AGS_Menu, Owner);
//}
