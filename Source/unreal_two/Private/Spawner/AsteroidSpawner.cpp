// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/AsteroidSpawner.h"

#include "PickUpBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"


// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	
	MinSpawnDelay = 0.5f;
	MaxSpawnDelay = 2.5f;
}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	StartSpawnTimer();
}

FVector AAsteroidSpawner::GetRandomSpawnPoint()
{
	const FVector SpawnOrigin = SpawnArea->Bounds.Origin;
	const FVector SpawnLimits = SpawnArea->Bounds.BoxExtent;
	
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnLimits);
}

// Called every frame
void AAsteroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAsteroidSpawner::SpawnActors()
{
	if (!ActorToSpawn || !GetWorld()) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	APickUpBase* SpawnedActor = GetWorld()->SpawnActor<APickUpBase>(ActorToSpawn, GetRandomSpawnPoint(),
		UKismetMathLibrary::RandomRotator(), SpawnParams);
	
	StartSpawnTimer();
}

void AAsteroidSpawner::StartSpawnTimer()
{
	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this,
		&AAsteroidSpawner::SpawnActors, RandomSpawnDelay, false);
}

