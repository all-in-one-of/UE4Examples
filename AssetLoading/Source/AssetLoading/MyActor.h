#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class AMyActor : public AActor
{
	GENERATED_BODY()

private:
	AMyActor();

public:
	UPROPERTY(VisibleAnywhere)
	class UFont* Font;

	UPROPERTY(VisibleAnywhere)
	class UTexture* Texture;
};
