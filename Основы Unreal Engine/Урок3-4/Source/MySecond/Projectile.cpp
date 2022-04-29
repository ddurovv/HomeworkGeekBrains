// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include <concrt.h>

#include "AnimGraphRuntime/Public/AnimNodes/AnimNode_SequenceEvaluator.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &AProjectile::Move, MovementRate, true);
	StartPosition = GetActorLocation();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AProjectile::Move()
{
	auto Location = GetActorLocation() + GetActorForwardVector() * MovementRate * MovementSpeed;
	SetActorLocation(Location,true);
	if (FVector::Distance(StartPosition, GetActorLocation()) > MaxDistance)
		Destroy();
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other != GetInstigator() && Other->GetInstigator() != GetInstigator())
	{
		Destroy();
		Other->Destroy();
	}
}


