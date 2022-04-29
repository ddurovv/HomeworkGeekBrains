// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "Windows/AllowWindowsPlatformTypes.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void ACannon::Fire()
{
	if (Ammunition < 1)
		return GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, FString::Printf(TEXT("No ammunition")));
	if (!bReadyToFire)
		return GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, FString::Printf(TEXT("Not ready")));
	

	if (SerialShootSize != 0)
	{
		SerialShootSize--;
		switch (Type)
		{
		case ECannonType::Projectile:
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, FString::Printf(TEXT("Projectile")));
			break;
		case ECannonType::Trace:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, FString::Printf(TEXT("Trace")));
			break;
		}


		GetWorld()->GetTimerManager().SetTimer(SerialHandle, this, &ACannon::Fire, SerialRate, false);
	}
	else {
		bReadyToFire = false;
		Ammunition -= 1;
		if (Ammunition > 0)
			GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &ACannon::OnReload, FireRate, false);
	}
}

void ACannon::FireSpecial()
{
	if (!bReadyToFireSpecial)
		return;
	switch (Type)
	{
	case ECannonType::Projectile:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("PROJECTILE")));
		break;
	case ECannonType::Trace:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("TRACE")));
		break;
	}
	bReadyToFireSpecial = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandleTwo, this, &ACannon::OnReloadTwo, FireSpecialRate, false);
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(ReloadHandle);
	GEngine->AddOnScreenDebugMessage(123,-1, FColor::Cyan,FString::Printf(TEXT("Until reload main fire %f"), Remaining));

	auto RemainingTwo = GetWorld()->GetTimerManager().GetTimerRemaining(ReloadHandleTwo);
	GEngine->AddOnScreenDebugMessage(113,-1, FColor::Cyan,FString::Printf(TEXT("Until reload special %f"), RemainingTwo));

	GEngine->AddOnScreenDebugMessage(123, -1, FColor::Green, FString::Printf(TEXT("Ammunition: %i"), Ammunition));
}

void ACannon::OnReload()
{
	bReadyToFire = true;
	SerialShootSize = TempSize;
}

void ACannon::OnReloadTwo()
{
	bReadyToFireSpecial = true;
}

