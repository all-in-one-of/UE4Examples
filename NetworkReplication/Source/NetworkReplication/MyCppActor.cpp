#include "MyCppActor.h"

MyCppActor::MyCppActor()
{
}


void MyCppActor::ChangeValues()
{
  IntValue++;
  FloatValue++;
  VectorValue += 1;
  BoolValue = !BoolValue;
  FNameValue = FName("My2ndFNameValue");
  FStringValue = FString("My 2nd FString Value");
  FTextValue = TEXT("My 2nd FText value");
  MyStructValue = { MyStructValue.IntValue * 2, MyStructValue.FloatValue * 2.5 };
}


