#include "MyComponent1.h"

DECLARE_LOG_CATEGORY_EXTERN(MyComponent1, Log, All);
DEFINE_LOG_CATEGORY(MyComponent1);



UMyComponent1::UMyComponent1()
{
	UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::UMyComponent1 %p"), this);
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

UMyComponent1::~UMyComponent1()
{
	UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::~UMyComponent1 %p, Name=%s"), this, *GetFullName());
}

void UMyComponent1::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::BeginPlay %p, Name=%s"), this, *GetFullName());
}

void UMyComponent1::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::EndPlay %p, Name=%s"), this, *GetFullName());
}

void UMyComponent1::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::BeginDestroy %p, Name=%s"), this, *GetFullName());
}

void UMyComponent1::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(false);
	UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::DestroyComponent %p, Name=%s"), this, *GetFullName());
}

void UMyComponent1::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// UE_LOG(MyComponent1, Log, TEXT("UMyComponent1::TickComponent %p, DeltaTime %f, Name=%s"), this, DeltaTime, *GetFullName());
}
