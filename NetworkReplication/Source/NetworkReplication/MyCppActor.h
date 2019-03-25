#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MyCppActor.generated.h"

USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()

	int IntValue;
	float FloatValue;
};

UCLASS(BlueprintType)
class AMyCppActor : public AActor
{
	GENERATED_BODY()

private:

	void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;
	void Tick(float DeltaSeconds) override;

	// IntValue is used to determine when properties update.
	int LastIntValue;

public:

	UPROPERTY(Replicated)
	int IntValue = 1;

	UPROPERTY(Transient, Replicated)
	float FloatValue = 3.14f;

	UPROPERTY(Transient, Replicated)
	FVector VectorValue = FVector(1.1,2.2,3.3);

	UPROPERTY(Transient, Replicated)
	bool BoolValue = true;

	UPROPERTY(Transient, Replicated)
	FName FNameValue = FName("MyFName");

	UPROPERTY(Transient, Replicated)
	FString FStringValue = FString("MyFString");

	UPROPERTY(Transient, Replicated)
	FText FTextValue = FText::FromString(TEXT("My Text"));

	UPROPERTY(Transient, Replicated)
	FMyStruct MyStructValue = { 10, 20.2 };


	UFUNCTION(BlueprintCallable)
	void ChangeValues();

	AMyCppActor();
};
