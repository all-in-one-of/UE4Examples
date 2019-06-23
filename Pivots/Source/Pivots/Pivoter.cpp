#include "Pivoter.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

APivoter::APivoter()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	EulerAngles = FVector::ZeroVector;
}

void APivoter::OnConstruction(FTransform const& Transform)
{
	Super::OnConstruction(Transform);

	StaticMeshComponent->SetRelativeTransform(FTransform(FQuat::MakeFromEuler(EulerAngles)));
}

