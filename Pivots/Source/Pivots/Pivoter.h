#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pivoter.generated.h"

UCLASS()
class PIVOTS_API APivoter : public AActor
{
	GENERATED_BODY()
	
public:	
	APivoter();

private:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	FVector EulerAngles;

	void OnConstruction(FTransform const& Transform) override;
};
