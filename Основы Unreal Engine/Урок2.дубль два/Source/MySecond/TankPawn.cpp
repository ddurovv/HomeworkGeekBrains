// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "MySecond.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tank body");
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("Turret body");
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>("CannonSpawnPoint");
	CannonSpawnPoint->SetupAttachment(TurretMesh);
}

void ATankPawn::MoveForward(float Scale)
{
	ForwardScaleMax = Scale;
}

void ATankPawn::MoveLateral(float Scale)
{
	LateralScale = Scale;
}

void ATankPawn::RotateRight(float Scale)
{
	RotateScaleMax = Scale;
}

void ATankPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	TankController = Cast<ATankPlayerController>(NewController);
}

void ATankPawn::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
		Cannon->FireSpecial();
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	if (CannonClass){
		auto Transform = CannonSpawnPoint->GetComponentTransform();
		Cannon = Cast<ACannon>(GetWorld()->SpawnActor(CannonClass, &Transform));
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ATankPawn::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
		Cannon->Destroy();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForwardScaleCurrent = FMath::Lerp(ForwardScaleCurrent, ForwardScaleMax, MovementAcceleration);
	//UE_LOG(LogTanks, Warning, TEXT("Forward scale = %f"), ForwardScaleCurrent)
	
	auto Location = GetActorLocation();
	auto ForwardVector = GetActorForwardVector();
	auto LateralVector = GetActorRightVector();
	SetActorLocation(Location + ForwardVector * ForwardScaleCurrent * MovementSpeed * DeltaTime + LateralVector * LateralScale * MovementSpeed * DeltaTime, true);

	RotateScaleCurrent = FMath::Lerp(RotateScaleCurrent, RotateScaleMax, RotationAcceleration);
	
	auto Rotation = GetActorRotation();
	Rotation.Yaw = Rotation.Yaw + RotationSpeed * RotateScaleCurrent * DeltaTime;
	SetActorRotation(Rotation);
	GEngine->AddOnScreenDebugMessage(234, 0.1f, FColor::Green, FString::Printf(TEXT("Forward scale = %f"), ForwardScaleCurrent), true);//-1 useless information

	if (TankController)
	{
		auto MousePosition = TankController->GetMousePosition();
		auto TurretRotation = TurretMesh->GetComponentRotation();
		FRotator MouseRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), MousePosition);
		MouseRotation.Pitch = 0;
		MouseRotation.Roll = 0; 
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, MouseRotation, TurretAcceleration));
	}
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

