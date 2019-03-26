#include "MyCppActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

UMyObject::UMyObject()
{
}

void UMyObject::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyObject, IntValue);
}

UMyObject2::UMyObject2()
{
}

void UMyObject2::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyObject2, IntValue);
}


AMyOtherActor::AMyOtherActor()
{
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	UStaticMeshComponent* SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SM->SetupAttachment(RootComponent);

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(this, TEXT("StaticMesh'/Engine/ArtTools/RenderToTexture/Meshes/ShadowTestMesh.ShadowTestMesh'"));
	UE_LOG(LogTemp, Log, TEXT("Mesh: %p"), Mesh);

	SM->SetStaticMesh(Mesh);
}

void AMyOtherActor::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyOtherActor, IntValue);
}

void AMyOtherActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("BeginPlay %p"), this);
}

AMyCppActor::AMyCppActor()
{
	bReplicates = true;

	LastIntValue = IntValue;
	MyObject = CreateDefaultSubobject<UMyObject>(TEXT("MyObject"));
	MyObject->IntValue = 3;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
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
	DOREPLIFETIME(AMyCppActor, MyObject);
	DOREPLIFETIME(AMyCppActor, MyObject2);
	DOREPLIFETIME(AMyCppActor, OtherActor);
}

void AMyCppActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if 0
	if (HasAuthority())
	{
		MyObject2 = NewObject<UMyObject2>(this);
		MyObject2->IntValue = 333;

		OtherActor = GetWorld()->SpawnActor<AMyOtherActor>();
		OtherActor->IntValue = 8585;
	}
#endif
}

void AMyCppActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, TEXT("HEWRE"));

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyCppActor::ServerChangeValues, 2.0, true);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyCppActor::ClientCheckForChangedValues, 1.0, true);
	}

#if 0
	if (HasAuthority())
	{
		MyObject2 = NewObject<UMyObject2>(this);
		MyObject2->IntValue = 3330;

		OtherActor = GetWorld()->SpawnActor<AMyOtherActor>();
		OtherActor->IntValue = 85850;
	}
#endif
}

bool AMyCppActor::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	// UE_LOG(LogTemp, Log, TEXT("ReplicateSubs"));

	if (MyObject2)
	{
		bWroteSomething |= Channel->ReplicateSubobject(MyObject2, *Bunch, *RepFlags);
	}

	return bWroteSomething;

}

void AMyCppActor::ServerChangeValues()
{
	check(HasAuthority());

	static FName NameOne = FName("FName1");
	static FName NameTwo = FName("FName2");

	IntValue++;
	FloatValue++;
	VectorValue += FVector(1, 1, 1);
	BoolValue = !BoolValue;
	FNameValue = FNameValue == NameOne ? NameTwo : NameOne;
	FStringValue = FString::Printf(TEXT("String%d"), IntValue);
	FTextValue = FText::FromString(FString::Printf(TEXT("Text%d"), IntValue));
	MyStructValue = { MyStructValue.IntValue * 2, MyStructValue.FloatValue * 2.5f };
	MyObject->IntValue += 7;

	if (MyObject2)
	{
		MyObject2->IntValue += 10;
	}
	else
	{
		MyObject2 = NewObject<UMyObject2>(this);
		MyObject2->IntValue = 431;
	}

	if (OtherActor)
	{
		OtherActor->IntValue += 20;
	}

#if 0
	if (OtherActor)
	{
		if (OtherActor->IntValue >= 4)
		{
			Destroy(OtherActor);
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
		//p.Owner = this;
		p.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		p.Name = FName(TEXT("MyOtherActor"));
		AMyOtherActor* o = GetWorld()->SpawnActor<AMyOtherActor>(FVector::ZeroVector, FRotator::ZeroRotator, p);
		UE_LOG(LogTemp, Log, TEXT("OtherActor Spawned: %p"), o);
		o->IntValue = IntValue * 10;
		OtherActor = o;
	}
#endif

	UE_LOG(LogTemp, Log, TEXT("Server: Change Values. IntValue=%d"), IntValue);
}

void AMyCppActor::ClientCheckForChangedValues()
{
	if (LastIntValue != IntValue)
	{
		LastIntValue = IntValue;
		UE_LOG(LogTemp, Log, TEXT("Client: Changed IntValue=%d FloatValue=%.2f VectorValue=(%.2f,%.2f,%.2f) BoolValue=%d FName=%s FString=%s FText=%s MyStruct={%d,%.2f} MyObject=%p={%d} MyObject2=%p={%d} OtherActor=%p={%d}"),
			IntValue, FloatValue,
			VectorValue.X, VectorValue.Y, VectorValue.Z,
			BoolValue,
			*FNameValue.ToString(), *FStringValue, *FTextValue.ToString(),
			MyStructValue.IntValue, MyStructValue.FloatValue,
			MyObject, MyObject ? MyObject->IntValue : -1,
			MyObject2, MyObject2 ? MyObject2->IntValue : -1,
			OtherActor, OtherActor ? OtherActor->IntValue : -1
		);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Client: No change."));
	}
}
