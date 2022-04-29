// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;

UCLASS()
class MYSECOND_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* Collision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSpawnPoint;
	
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100;
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 500;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MovementAcceleration = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationAcceleration = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TurretAcceleration = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armory")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Armory");
	ACannon* Cannon;
	
	// Sets default values for this pawn's properties
	ATankPawn();

	void MoveForward(float Scale);
	void MoveLateral(float Scale);
	void RotateRight(float Scale);
	
	virtual void PossessedBy(AController* NewController) override;
	void Fire();
	void FireSpecial();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	float ForwardScaleMax = 0;
	float ForwardScaleCurrent = 0;
	float LateralScale = 0;
	float RotateScaleMax = 0;
	float RotateScaleCurrent = 0;

	class ATankPlayerController* TankController; 
};
