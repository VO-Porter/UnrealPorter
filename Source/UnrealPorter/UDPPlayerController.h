// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "Json.h"
#include <string>
#include "GameFramework/Actor.h"
#include "UDPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTER_API AUDPPlayerController : public AActor
{
	GENERATED_BODY()
	
public:
	AUDPPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void Jump();

	UFUNCTION(BlueprintImplementableEvent)
	void MoveRight(float Value);



public:
	FSocket* ListenSocket;

	FUdpSocketReceiver* UDPReceiver = nullptr;
	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	bool StartUDPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	//ScreenMsg
	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const float Value)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Value));
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
	}

public:

	/** Called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void BeginPlay() override;


public:
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);
	
	
};
