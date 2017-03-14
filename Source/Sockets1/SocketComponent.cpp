// Fill out your copyright notice in the Description page of Project Settings.

#include "Sockets1.h"
#include "SocketComponent.h"
#include "Runtime/Networking/Public/Networking.h"


// Sets default values for this component's properties
USocketComponent::USocketComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USocketComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void USocketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (Listener && !Socket) {
		bool Pending;
		if (Listener->HasPendingConnection(Pending) && Pending) {
			TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
			FSocket* Sock = Listener->Accept(*RemoteAddress, TEXT("SocketComponent Client"));

			if (Sock) {
				Socket = Sock;
				UE_LOG(LogTemp, Log, TEXT("Connection from %s"), *RemoteAddress->ToString(true));
			}
		}
	}

	if (Socket) {
		// TODO: if not locally controlled && has pending data
		//           append to waveform
		uint32 PendingDataSize;
		if (Socket->HasPendingData(PendingDataSize)) {
			uint8* Data = new uint8[PendingDataSize];
			int32 BytesRead;
			Socket->Recv(Data, PendingDataSize, BytesRead, ESocketReceiveFlags::Type::None);
			UE_LOG(LogTemp, Log, TEXT("Received %d bytes"), BytesRead);
			
			delete Data;
		}

		// TODO: If locally controlled & voice data available
		//           Send data
	}
}

int USocketComponent::Listen() {
	// TODO: Assert called once
	Listener = FTcpSocketBuilder(TEXT("SocketComponent Server"))
		.AsReusable()
		.BoundToEndpoint(FIPv4Endpoint::Any)
		.Listening(8);
	TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Listener->GetAddress(*Address);
	UE_LOG(LogTemp, Log, TEXT("Listening on %s"), *Address->ToString(true));
	return Address->GetPort();
}

void USocketComponent::Connect(FString Address, int Port) {
	// TODO: Assert called once
	FIPv4Address IPv4Address;
	FIPv4Address::Parse(Address, IPv4Address);
	FIPv4Endpoint Endpoint(IPv4Address, Port);
	TSharedRef<FInternetAddr> Addr = Endpoint.ToInternetAddr();
	FSocket* Sock = FTcpSocketBuilder(TEXT("SocketComponent Client"));
	if (!Sock->Connect(*Addr)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to %s"), *Endpoint.ToString());
		return;
	}

	Socket = Sock;
	UE_LOG(LogTemp, Log, TEXT("Connected to %s"), *Endpoint.ToString());
}