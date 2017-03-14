// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/IPAddress.h"
#include "SocketComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOCKETS1_API USocketComponent : public UActorComponent
{
	GENERATED_BODY()

	FSocket* Listener;
	FSocket* Socket;

public:
	// Sets default values for this component's properties
	USocketComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "Socket")
		int Listen();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void Connect(FString Address, int Port);
	
};
