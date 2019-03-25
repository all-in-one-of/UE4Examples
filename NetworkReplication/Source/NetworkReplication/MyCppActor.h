#pragma once

#include "CoreMinimal.h"
#include "MyCppActor.generated.h"

struct FMyStruct
{
  int IntValue;
  float FloatValue;
}

UCLASS()
class MyCppActor : public AActor
{
public:

  UPROPERTY(Transient, Replicated)
  int IntValue = 1;

  UPROPERTY(Transient, Replicated)
  float FloatValue 3.14;

  UPROPERTY(Transient, Replicated)
  FVector VectorValue = FVector(1.1,2.2,3.3);

  UPROPERTY(Transient, Replicated)
  bool BoolValue = true;

  UPROPERTY(Transient, Replicated)
  FName FNameValue = FName("MyFName");

  UPROPERTY(Transient, Replicated)
  FString FStringValue = FString("MyFString");

  UPROPERTY(Transient, Replicated)
  FText FTextValue = TEXT("My Text");

  UPROPERTY(Transient, Replicated)
  FMyStruct MyStructValue = { 10, 20.2 };


  void ChangeValues();
};
