// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()-> InitCapsuleSize(42.0f, 96.0f);
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()-> RotationRate =FRotator(0.0f,540.0f,0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	bDead = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	FString MachineType = TEXT("UNKNOWN");
    
	if (GetWorld()->GetNetMode() == NM_ListenServer)
	{
		MachineType = TEXT("LISTEN SERVER (Host + Player)");
		UE_LOG(LogTemp,Warning,TEXT("SERVER+Client"))
		
	}
	else if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		MachineType = TEXT("DEDICATED SERVER");
	}
	else if (GetWorld()->GetNetMode() == NM_Client)
	{
		MachineType = TEXT("PURE CLIENT");
		UE_LOG(LogTemp,Warning,TEXT("CLIENT"))
	}
	else if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		MachineType = TEXT("STANDALONE (Offline)");
	}
    
	FString Authority = HasAuthority() ? TEXT("AUTHORITY") : TEXT("NO AUTHORITY");
    
	UE_LOG(LogTemp, Warning, TEXT("%s - %s - %s"), 
		*GetName(), 
		*MachineType,
		*Authority);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAxis("MoveFoward",this,&APlayerCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnCamera",this,&APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&APlayerCharacter::LookUp);
}

void APlayerCharacter::MoveFoward(float InputValue)
{
	FVector FowardDirection = GetActorForwardVector();
	AddMovementInput(FowardDirection,InputValue);
}
void APlayerCharacter::MoveRight(float InputValue)
{
	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection,InputValue);
}

void APlayerCharacter::LookUp(float InputValue)
{
	AddControllerPitchInput(InputValue);
	
}
void APlayerCharacter::Turn(float InputValue)
{
	AddControllerYawInput(InputValue);
}


