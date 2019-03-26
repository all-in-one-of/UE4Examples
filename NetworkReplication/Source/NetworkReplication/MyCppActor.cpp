#include "MyCppActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"

UMyObject::UMyObject()
{
}

void UMyObject::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyObject, IntValue);
}

AMyOtherActor::AMyOtherActor()
{
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AMyOtherActor::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyOtherActor, IntValue);
}

AMyCppActor::AMyCppActor()
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	LastIntValue = IntValue;
	MyObject = CreateDefaultSubobject<UMyObject>(TEXT("MyObject"), true /* transient */);
	MyObject->IntValue = 3;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

static FName NameOne = FName("Set Name One");
static FName NameTwo = FName("Set Name Two");

void AMyCppActor::ChangeValues()
{
	check(HasAuthority());

	IntValue++;
	FloatValue++;
	VectorValue += FVector(1,1,1);
	BoolValue = !BoolValue;
	FNameValue = FNameValue == NameOne ? NameTwo : NameOne;
	FStringValue = FString::Printf(TEXT("Set String Value %d"), IntValue);
	FTextValue = FText::FromString(FString::Printf(TEXT("Set Text value %d"), IntValue));
	MyStructValue = { MyStructValue.IntValue * 2, MyStructValue.FloatValue * 2.5f };
	MyObject->IntValue += 7;

	if (OtherActor)
	{
		if (OtherActor->IntValue >= 4)
		{
			// Destroy(OtherActor);
			OtherActor = nullptr;
		}
		else
		{
			OtherActor->IntValue++;
		}
	}
	else
	{
		FActorSpawnParameters p;
		OtherActor = GetWorld()->SpawnActor<AMyOtherActor>(FVector::ZeroVector, FRotator::ZeroRotator, p);
		OtherActor->IntValue = 1;
	}

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
			UE_LOG(LogTemp, Log, TEXT("CLIENT OBSERVED CHANGE IntValue=%d FloatValue=%f VectorValue=%s BoolValue=%d FName=%s FString=%s FText=%s MyStruct={%d,%f} MyObject=%p={%d}, OtherActor=%p={%d}"),
				IntValue, FloatValue, *VectorValue.ToString(), BoolValue,
				*FNameValue.ToString(), *FStringValue, *FTextValue.ToString(),
				MyStructValue.IntValue, MyStructValue.FloatValue,
				MyObject, MyObject ? MyObject->IntValue : -1,
				OtherActor, OtherActor ? OtherActor->IntValue : -1
				);
		}
		else
		{
			// UE_LOG(LogTemp, Log, TEXT("SAME: %d. FloatValue=%f"), IntValue, FloatValue);
		}
	}
}
