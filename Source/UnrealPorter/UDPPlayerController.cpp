// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealPorter.h"
#include "UDPPlayerController.h"


AUDPPlayerController::AUDPPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ListenSocket = NULL;
};


void AUDPPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	StartUDPReceiver("UDPSocketListener", "192.168.150.104", 8890);
}



void AUDPPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//~~~~~~~~~~~~~~~~

	UDPReceiver->Stop();
	delete UDPReceiver;
	UDPReceiver = nullptr;

	//Clear all sockets!
	//		makes sure repeat plays in Editor dont hold on to old sockets!
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Rama's Start TCP Receiver
bool AUDPPlayerController::StartUDPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {

	//ScreenMsg("RECEIVER INIT");

	//~~~

	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	//Create Socket
	FIPv4Endpoint Endpoint(Addr, ThePort);

	//BUFFER SIZE
	int32 BufferSize = 2 * 1024 * 1024;

	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(BufferSize);
	;

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UDPReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UDPReceiver->OnDataReceived().BindUObject(this, &AUDPPlayerController::Recv);
	UDPReceiver->Start();

	return true;
}

void AUDPPlayerController::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	int size = ArrayReaderPtr->Num();

	TArray<uint8> array;
	uint8* simpleArray = ArrayReaderPtr->GetData();
	array.Append(simpleArray, size);

	FString JsonRaw = StringFromBinaryArray(array);
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		if (JsonParsed->HasField("x")) {
			FString xVal = JsonParsed->GetStringField("x");
			int intVal = FCString::Atoi(*xVal);
			if (intVal > 50 || intVal < -50) {
				float val = (intVal - 50) / 300.f;
				//ScreenMsg("Received", val);

				MoveRight(val);
			}
			else
			{
				MoveRight(0);
			}
		}
		
		if (JsonParsed->HasField("jump")) {
			Jump();
		}
	}
}

FString AUDPPlayerController::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	//FString can take in the c_str() of a std::string
	return FString(cstr.c_str());
}
