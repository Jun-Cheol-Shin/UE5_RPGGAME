// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGCharacter.h"

#include "Blueprint/UserWidget.h"
#include "RPGUserWidget.h"

#include "GridActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PathRequestManager.h"
#include "RPGCharacterAnimInstance.h"


// Sets default values
ARPGCharacter::ARPGCharacter()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArm->SetupAttachment(RootComponent);
	springArm->bDoCollisionTest = false;

	springArm->TargetArmLength = 800.f;
	springArm->bInheritPitch = false;
	springArm->bInheritRoll = false;
	springArm->bInheritYaw = false;
	
	quarterViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("QuarterViewCamera"));
	quarterViewCamera->SetupAttachment(springArm);
	
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerUMG = nullptr;
	PlayerUMGClass = nullptr;

	//ClimberComponent = CreateDefaultSubobject<UClimberComponent>(TEXT("ClimberComponent"));
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Mouse Cursor Setting...
	APlayerController* control = Cast<APlayerController>(GetController());
	if(control)
	{
		control->bShowMouseCursor = true;
		control->bEnableClickEvents = true;
		control->bEnableMouseOverEvents = true;
		control->GetPawn()->bUseControllerRotationYaw = false;
	}


	if(bIsQuarterView)
	{
		//quarterViewCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
		//quarterViewCamera->SetOrthoWidth(2500.f);
		springArm->SetRelativeRotation(FRotator(-52.5f, -45, 0));
		springArm->TargetArmLength = 900.f;
	}

	else
	{
		quarterViewCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
		quarterViewCamera->SetOrthoWidth(2500.f);
		springArm->SetRelativeRotation(FRotator(-90, 0, 0));
	}

	// Animation Tree Setting...
	UAnimInstance* Instance = GetMesh()->GetAnimInstance();

	if (Instance)
	{
		animInstance = Cast<URPGCharacterAnimInstance>(Instance);

		if (animInstance)
		{
			animInstance->GetPlayer(this);
			//UE_LOG(LogTemp, Warning, TEXT("Get Player!"));
		}
	}


	// A* Grid Setting..
	SetGridActor();


	// UI Setting..
	if(IsLocallyControlled() && PlayerUMGClass)
	{
		APlayerController* APC = GetController<APlayerController>();
		check(APC);
		PlayerUMG = CreateWidget<URPGUserWidget>(APC, PlayerUMGClass);
		check(PlayerUMG);
		PlayerUMG->AddToPlayerScreen();
	}
}

void ARPGCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red,
	FString::Printf(TEXT("Grid Actor : %s"), gridActor ? *gridActor->GetName() : TEXT("None")));
	/*GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red,
		FString::Printf(TEXT("Climb ? : %s"), ladderActor ? *ladderActor->GetName() : TEXT("None")));*/
	
	CharacterMove();
	FollowPath(DeltaTime);
	//ClimbingLadder(DeltaTime);
}

void ARPGCharacter::CharacterMove()
{
	if(bClickMouse && gridActor && gridActor->GetRequestManager())
	{
		if(GetDestinationToMove() && gridActor->NodeFromWorldPoint(currentWaypoint) != gridActor->NodeFromWorldPoint(DestLocation))
		{
			FCall_Back_Two_Params Fcallback;
			Fcallback.BindUFunction(this, FName("OnPathFound"));
			if(Fcallback.IsBound())
			{
				//UE_LOG(LogTemp, Error, TEXT("SUCCESS BOUND!!"));
				gridActor->GetRequestManager()->RequestPath(GetActorLocation(), DestLocation, Fcallback);
			}
		}
	}
}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("Move", IE_Pressed, this, &ARPGCharacter::InputClickPressed);
	InputComponent->BindAction("Move", IE_Released, this, &ARPGCharacter::InputClickReleased);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARPGCharacter::ZoomInCamera);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARPGCharacter::ZoomOutCamera);
	//InputComponent->BindAction("Interaction", IE_Pressed, this, &ARPGCharacter::InteractionLadder);
}

void ARPGCharacter::ZoomInCamera()
{
	if(springArm)
	{
		springArm->TargetArmLength = std::clamp(springArm->TargetArmLength -= 50.f, 300.f, 1200.f);
	}
}
void ARPGCharacter::ZoomOutCamera()
{
	if(springArm)
	{
		springArm->TargetArmLength = std::clamp(springArm->TargetArmLength += 50.f, 300.f, 1200.f);
	}
}
void ARPGCharacter::InputClickPressed()
{
	bClickMouse = true;
	if(!gridActor) SetGridActor();
}
void ARPGCharacter::InputClickReleased()
{
	bClickMouse = false;
}

// A* Algorithm Move
bool ARPGCharacter::GetDestinationToMove()
{
	APlayerController* Control = Cast<APlayerController>(GetController());
	if(Control)
	{
		FHitResult hit;
		Control->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, hit);
		if(hit.GetActor() && Cast<AGridActor>(hit.GetActor()) == gridActor)
		{
			DestLocation = hit.Location;
			return true;
		}
	}
	return false;
}
void ARPGCharacter::SetGridActor()
{
	FHitResult Hit;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(),
		GetActorLocation() - GetActorUpVector() * 100.f, ECollisionChannel::ECC_WorldDynamic, CollisionParams);

	if(Hit.GetActor())
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *Hit.GetActor()->GetName());
		AGridActor* actor = Cast<AGridActor>(Hit.GetActor());
		if(actor) gridActor = actor;
	}
}
void ARPGCharacter::OnPathFound(const TArray<FVector>& newPath, bool pathSuccessful)
{
	if(pathSuccessful)
	{
		path = newPath;

		/*for(auto iter : path)
		{
			UE_LOG(LogTemp, Error, TEXT("%f, %f, %f"), iter.X, iter.Y, iter.Z);
		}*/
		
		if(path.IsValidIndex(0))
		{
			targetIndex = 0;
			currentWaypoint = path[0];
			SetActorRotation(FRotator(0,UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),DestLocation).Yaw, 0));
			bIsStartMove = true;
		}
		/*etWorld()->GetTimerManager().ClearTimer(moveTimer);
		GetWorld()->GetTimerManager().SetTimer(moveTimer, [this]()
		{
			FollowPath();
		}
		,0 , false);
		*/
	}
}
void ARPGCharacter::FollowPath(float DeltaTime)
{
	if(bIsStartMove)
	{
		if(path.Num() > 0)
		{
			if(targetIndex == path.Num() - 1)
			{
				FVector2D actorLoc = FVector2D(GetActorLocation().X, GetActorLocation().Y);
				FVector2D current = FVector2D(currentWaypoint.X, currentWaypoint.Y);

				if(actorLoc.Equals(current, 15.f))
				{
					//currentWaypoint = GetActorLocation();
					path.Reset();
					bIsStartMove = false;
					return;
				}
			}
		
			else
			{
				if(gridActor && gridActor->NodeFromWorldPoint(GetActorLocation()) == gridActor->NodeFromWorldPoint(currentWaypoint))
				{
					targetIndex++;
					if(targetIndex >= path.Num() || !path.IsValidIndex(targetIndex))
					{
						targetIndex = path.Num() - 1;
						currentWaypoint = path.Last();
						return;
					}
			
					currentWaypoint = path[targetIndex];
				}
			}

		
			FRotator currentRotation = FRotator(0,UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),currentWaypoint).Yaw, 0);
			// 부드럽게 회전하도록 변경해야된다....
			SetActorRotation(FMath::Lerp(GetActorRotation(),currentRotation, DeltaTime * 3.f));
			FVector dir = FVector(currentWaypoint.X - GetActorLocation().X,
				currentWaypoint.Y - GetActorLocation().Y, 0).GetSafeNormal();
			AddMovementInput(dir);
		}
	}
}


