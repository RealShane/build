// Fill out your copyright notice in the Description page of Project Settings.


#include "Person.h"

#include "Lib.h"

// Sets default values
APerson::APerson()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this -> Construct();
	this -> CreateModel();
	this -> CreateCamera();
}

void APerson::Construct()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent -> SetupAttachment(RootComponent);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
}

void APerson::CreateModel()
{
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> Model(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	// StaticMeshComponent -> SetStaticMesh(Model.Object);
	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
	SkeletalMeshComponent -> SetSkeletalMesh(SkeletalMesh);
	SkeletalMeshComponent -> SetupAttachment(RootComponent);
	SkeletalMeshComponent -> SetRelativeRotation(FRotator(0, -90, 0));
	SkeletalMeshComponent -> SetRelativeLocation(FVector(0, 0, 0));
	CapsuleComponent -> SetupAttachment(RootComponent);
	CapsuleComponent -> SetCapsuleHalfHeight(100);
	CapsuleComponent -> SetRelativeLocation(FVector(0, 0, 100));
	CapsuleComponent -> SetSimulatePhysics(true);
	CapsuleComponent -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent -> SetEnableGravity(true);
}

void APerson::CreateCamera()
{
	SpringArmComponent -> SetupAttachment(SkeletalMeshComponent);
	SpringArmComponent -> SetRelativeLocation(FVector(0, 0, 100));
	SpringArmComponent -> SetRelativeRotation(FRotator(-15, 90, 0));
	SpringArmComponent -> TargetArmLength = 600;
	SpringArmComponent -> bEnableCameraLag = true;
	SpringArmComponent -> CameraLagSpeed = 3;
	CameraComponent -> SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent -> SetRelativeRotation(FRotator(15, 0, 0));
}

void APerson::OnHit(UPrimitiveComponent * OverLapComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	GEngine -> AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, SweepResult.GetActor()->GetName());
}

// Called when the game starts or when spawned
void APerson::BeginPlay()
{
	Super::BeginPlay();
	// SetActorLocation(FVector(12000, 12000, 200));
}

// Called every frame
void APerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

