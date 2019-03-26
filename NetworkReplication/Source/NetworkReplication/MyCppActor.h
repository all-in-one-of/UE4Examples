#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MyCppActor.generated.h"

USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()

	UPROPERTY()
	int IntValue;

	UPROPERTY()
	float FloatValue;
};

UCLASS()
class UMyObject : public UObject
{
	GENERATED_BODY()

private:

	void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;

public:

	UPROPERTY(Transient, Replicated)
	int IntValue = 1;

	UMyObject();
};

UCLASS(BlueprintType)
class AMyOtherActor : public AActor
{
	GENERATED_BODY()

private:

	void GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const override;

public:

	UPROPERTY(Replicated, Transient)
	int IntValue;

	AMyOtherActor();
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

	UPROPERTY(Transient, Replicated)
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

	UPROPERTY(Transient, Replicated)
	UMyObject* MyObject;

	UPROPERTY(Transient, Replicated)
	AMyOtherActor* OtherActor;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ChangeValues();

	AMyCppActor();
};
