#include "MyCppActor.h"
#include "Net/UnrealNetwork.h"

AMyCppActor::AMyCppActor()
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	LastIntValue = IntValue;
}

void AMyCppActor::ChangeValues()
{
	IntValue++;
	FloatValue++;
	VectorValue += FVector(1,1,1);
	BoolValue = !BoolValue;
	FNameValue = FName("My2ndFNameValue");
	FStringValue = FString("My 2nd FString Value");
	FTextValue = FText::FromString(TEXT("My 2nd FText value"));
	MyStructValue = { MyStructValue.IntValue * 2, MyStructValue.FloatValue * 2.5f };

	UE_LOG(LogTemp, Log, TEXT("Server (HasAuthority=%d) changed int to %d"), HasAuthority(), IntValue);
}


void AMyCppActor::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCppActor, IntValue);
	DOREPLIFETIME(AMyCppActor, FloatValue);
	DOREPLIFETIME(AMyCppActor, VectorValue);
	DOREPLIFETIME(AMyCppActor, BoolValue);
	DOREPLIFETIME(AMyCppActor, FNameValue);
	DOREPLIFETIME(AMyCppActor, FStringValue);
	DOREPLIFETIME(AMyCppActor, FTextValue);
	DOREPLIFETIME(AMyCppActor, MyStructValue);
}

void AMyCppActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// UE_LOG(LogTemp, Log, TEXT("Tick called. HasAuthrity? %d"), HasAuthority());


	if (!HasAuthority())
	{
		if (LastIntValue != IntValue)
		{
			LastIntValue = IntValue;
			UE_LOG(LogTemp, Log, TEXT("CLIENT OBSERVED CHANGE IntValue=%d FloatValue=%f VectorValue=%s BoolValue=%d FName=%s FString=%s FText=%s MyStruct={%d,%f}"),
				IntValue, FloatValue, *VectorValue.ToString(), BoolValue,
				*FNameValue.ToString(), *FStringValue, *FTextValue.ToString(),
				MyStructValue.IntValue, MyStructValue.FloatValue);
		}
		else
		{
			// UE_LOG(LogTemp, Log, TEXT("SAME: %d. FloatValue=%f"), IntValue, FloatValue);
		}
	}
}
