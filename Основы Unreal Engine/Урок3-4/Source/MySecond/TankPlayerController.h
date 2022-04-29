// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

UCLASS()
class MYSECOND_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	FVector GetMousePosition() const
	{
		return MousePosition;
	}
private: 
	void OnMoveForward(float Scale);
	void OnMoveLateral(float Scale);
	void OnRotateRight(float Scale);
	void OnFire();
	void OnFireSpecial();
	
	UPROPERTY() //прописывать не надо, так как он приватный, а ещё не будет удаляться движком, потомушо макрос
	ATankPawn* TankPawn;

	FVector MousePosition; 
	
};
