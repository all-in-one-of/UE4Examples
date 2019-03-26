#include "MyCppActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/ActorChannel.h"

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

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	LastIntValue = IntValue;
	MyObject = CreateDefaultSubobject<UMyObject>(TEXT("MyObject"));
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

	if (MyObject2)
	{
		MyObject2->IntValue += 10;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SHOULD NEVER BE HERE"));

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
	DOREPLIFETIME(AMyCppActor, MyObject);
	DOREPLIFETIME(AMyCppActor, MyObject2);
	DOREPLIFETIME(AMyCppActor, OtherActor);
}

void AMyCppActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		MyObject2 = NewObject<UMyObject2>(this);
		MyObject2->IntValue = 333;

		OtherActor = GetWorld()->SpawnActor<AMyOtherActor>();
		OtherActor->IntValue = 8585;
	}
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

void AMyCppActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// UE_LOG(LogTemp, Log, TEXT("Tick called. HasAuthrity? %d"), HasAuthority());


	if (!HasAuthority())
	{
		if (LastIntValue != IntValue)
		{
			LastIntValue = IntValue;
			UE_LOG(LogTemp, Log, TEXT("CLIENT OBSERVED CHANGE IntValue=%d FloatValue=%f VectorValue=%s BoolValue=%d FName=%s FString=%s FText=%s MyStruct={%d,%f} MyObject=%p={%d} MyObject2=%p={%d} OtherActor=%p={%d}"),
				IntValue, FloatValue, *VectorValue.ToString(), BoolValue,
				*FNameValue.ToString(), *FStringValue, *FTextValue.ToString(),
				MyStructValue.IntValue, MyStructValue.FloatValue,
				MyObject, MyObject ? MyObject->IntValue : -1,
				MyObject2, MyObject2 ? MyObject2->IntValue : -1,
				OtherActor, OtherActor ? OtherActor->IntValue : -1
				);
		}
		else
		{
			// UE_LOG(LogTemp, Log, TEXT("SAME: %d. FloatValue=%f"), IntValue, FloatValue);
		}
	}
}
