// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"


UENUM()
enum class ECannonType
{
	Projectile UMETA(DisplayName = "Projectile"),
	Trace	   UMETA(DisplayName = "Trace")
};

UCLASS()
class MYSECOND_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USceneComponent* EmptyRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UArrowComponent* SpawnPoint;
	

	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	ECannonType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	float FireRate = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	float SerialRate = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	int SerialShootSize = 3;
//в привате переменная TempSize
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	int Ammunition = 5;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	float FireSpecialRate = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon")
	TSubclassOf<AProjectile> ProjectileType;


	void Fire();
	void FireSpecial();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnReload();
	void OnReloadTwo();
	bool bReadyToFire = true;
	bool bReadyToFireSpecial = true;
	int TempSize = SerialShootSize;
	FTimerHandle ReloadHandle;
	FTimerHandle SerialHandle;
	FTimerHandle ReloadHandleTwo;
};
