#include "MyActor.h"

#include "Engine/Font.h"
#include "Engine/Texture.h"
#include "UObject/ConstructorHelpers.h"

AMyActor::AMyActor()
{
	Font = ConstructorHelpers::FObjectFinder<UFont>(TEXT("Font'/Game/Roboto-Regular_Font.Roboto-Regular_Font'")).Object;
	Texture = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("Texture2D'/Game/Circle.Circle'")).Object;

	UE_LOG(LogTemp, Display, TEXT("Font is %p, Texture is %p"), Font, Texture);

}
