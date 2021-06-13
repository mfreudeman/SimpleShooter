// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);


}

void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("Pull Trigger"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AController* OwnerController = OwnerPawn->GetController())
		{
			FVector OwnerLocation;
			FRotator OwnerRotation;
			OwnerController->GetPlayerViewPoint(OwnerLocation, OwnerRotation);
			
			FVector End = OwnerLocation + OwnerRotation.Vector() * MaxRange;

			FHitResult Hit;
			bool bDidHit = GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, End, ECC_GameTraceChannel1);
			if (bDidHit)
			{
				DrawDebugPoint(GetWorld(), OwnerLocation, 20.f, FColor::Green, true);

				FVector ShotDirection = -OwnerRotation.Vector();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, ShotDirection.Rotation());

				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				if (AActor* HitActor = Hit.GetActor())
				{
					HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
					DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
					DrawDebugLine(GetWorld(), OwnerLocation, Hit.Location, FColor::Purple, true);
				}
				else
				{
					DrawDebugLine(GetWorld(), OwnerLocation, Hit.Location, FColor::Black, true);
				}
			}
			else
			{
				DrawDebugPoint(GetWorld(), OwnerLocation, 20.f, FColor::Green, true);
				DrawDebugLine(GetWorld(), OwnerLocation, End, FColor::Blue, true);
			}
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

