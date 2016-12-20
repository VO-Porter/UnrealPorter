// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPorter.h"
#include "UnrealPorterGameModeBase.h"
#include "UnrealPorterPawn.h"
#include "UDPPlayerController.h"


AUnrealPorterGameModeBase::AUnrealPorterGameModeBase()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AUnrealPorterPawn::StaticClass();
	PlayerControllerClass = AUDPPlayerController::StaticClass();
}


