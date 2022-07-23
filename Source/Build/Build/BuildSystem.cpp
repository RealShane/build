#include "BuildSystem.h"

UBuildSystem::UBuildSystem()
{
	PrimaryComponentTick . bCanEverTick = true;
	BuildDistance = FStatic::FiveHundred;
	UI = CreateDefaultSubobject<UUIFacade>(*FStatic::UI);
}

void UBuildSystem::BeginPlay()
{
	Super::BeginPlay();
	Local = GetWorld() -> GetGameInstance() -> GetSubsystem<ULocal>();
	Global = GetWorld() -> GetGameState<AGlobal>();
	PopLayer = UI -> GetInstance<UPopLayer>(*FStatic::PopLayerBP);
	PopNotice = UI -> GetInstance<UPopNotice>(*FStatic::PopNoticeBP);
}

void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Player) {
		return;
	}
	SetServerPlayer(Player);
	if (IsServer()) {
		return;
	}
	this -> BlurAttach();
}

void UBuildSystem::SetPlayer(AMainCharacter* P)
{
	Player = P;
}

void UBuildSystem::SetServerPlayer_Implementation(AMainCharacter* P)
{
	Player = P;
}

void UBuildSystem::Building()
{
	if (BuildType == FStatic::Foundation) {
		this -> FoundationBuild(Local -> CID, Local -> UID);
	}
	if (BuildType == FStatic::Wall) {
		this -> WallBuild(Local -> CID, Local -> UID);
	}
	if (BuildType == FStatic::Floor) {
		this -> FloorBuild(Local -> CID, Local -> UID);
	}
}

/**
 * 显示取消虚影
 */
void UBuildSystem::SetBuild(const FString Type)
{
	if (Type == FStatic::Foundation) {
		BuildType = FStatic::Foundation;
		this -> Foundation();
	} else if (Type == FStatic::Wall) {
		BuildType = FStatic::Wall;
		this -> Wall();
	} else if (Type == FStatic::Floor) {
		BuildType = FStatic::Floor;
		this -> Floor();
	}
}

void UBuildSystem::UnSetBuild()
{
	if (BuildType == FStatic::Foundation) {
		GetWorld() -> DestroyActor(FoundationBase);
	}
	if (BuildType == FStatic::Wall) {
		GetWorld() -> DestroyActor(WallBase);
	}
	if (BuildType == FStatic::Floor) {
		GetWorld() -> DestroyActor(FloorBase);
	}
	FoundationBase = nullptr;
	WallBase = nullptr;
	FloorBase = nullptr;
	BuildType = nullptr;
}

/**
 * 虚影可移动附着
 */
void UBuildSystem::BlurAttach()
{
	if (BuildType == FStatic::Foundation) {
		this -> FoundationBlurAttach(Player -> RayHit, Local -> CID, Local -> UID);
	} else if (BuildType == FStatic::Wall) {
		this -> WallBlurAttach(Player -> RayHit, Local -> CID, Local -> UID);
	} else if (BuildType == FStatic::Floor) {
		this -> FloorBlurAttach(Player -> RayHit, Local -> CID, Local -> UID);
	}
}

bool UBuildSystem::IsBuildMode() const
{
	return !BuildType . IsEmpty();
}

bool UBuildSystem::IsServer() const
{
	if (GetNetMode() != NM_DedicatedServer) {
		return false;
	}
	return true;
}

bool UBuildSystem::IsClient() const
{
	if (Player -> GetLocalRole() != ROLE_AutonomousProxy || GetNetMode() == NM_DedicatedServer) {
		return false;
	}
	return true;
}

FString UBuildSystem::CreateBuildingKey(const FString Type) const
{
	return Local -> KeyPre +
		FStatic::Underline + Type +
		FStatic::Underline +
		FLib::Salt(FStatic::Ten) +
		FStatic::Underline +
		FString::SanitizeFloat(FDateTime::Now() . ToUnixTimestamp());
}

void UBuildSystem::Pop_Implementation(const FString& Message, const FString& Color)
{
	PopNotice -> Pop(Message, Color);
}

void UBuildSystem::Foundation_Implementation()
{
	if (BuildType != FStatic::Foundation) {
		this -> UnSetBuild();
	}
	if (FoundationBase == nullptr) {
		FActorSpawnParameters Parameters;
		Parameters . Owner = Player;
		Parameters . bNoFail = true;
		FoundationBase = GetWorld() -> SpawnActor<AFoundation>(
			FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
			FRotator(FStatic::Zero),
			Parameters
		);
		BuildType = FStatic::Foundation;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FoundationBlurAttach_Implementation(FHitResult RayHit, int CID, int UID)
{
	if (FoundationBase != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		const FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		const FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation . Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		float Angle = FStatic::Ninety - (FStatic::ThreeHundredAndSixty - ViewRotation . Pitch);
		if (Angle <= FStatic::Zero) {
			Angle = FStatic::EightyNine;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * FStatic::Hundred;
		if (BuildDistance > FStatic::Thousand) {
			BuildDistance = FStatic::Thousand;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		const float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		const float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = FoundationBase -> LandZ;
		if (Z <= FStatic::Zero) {
			Z = Player -> GetActorLocation() . Z;
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Zero) + FVector(X, Y, Z);
		// 光线击中处附着
		if (RayHit . IsValidBlockingHit()) {
			const FString HitName = RayHit . GetActor() -> GetName();
			if (FStr::IsContain(HitName, FStatic::Foundation)) {
				const FString Index = RayHit . GetActor() -> Tags[FStatic::Zero] . ToString();
				const FBuildings BuildBlock = Global -> Buildings[Index];
				bool bIsAlly = false;
				if (BuildBlock . CID == CID && CID != FStatic::Zero) {
					bIsAlly = true;
				}
				if (BuildBlock . CID == FStatic::Zero && BuildBlock . UID == UID) {
					bIsAlly = true;
				}
				if (bIsAlly) {
					const FString AttachSide = RayHit . GetComponent() -> GetName();
					const float Side = FoundationBase -> HalfXY * FStatic::Two;
					if (AttachSide == FStatic::Right && !BuildBlock . Right) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Low && !BuildBlock . Low) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Left && !BuildBlock . Left) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Up && !BuildBlock . Up) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::Zero);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
				}
			}
		}
		FoundationBase -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		FoundationBase -> SetActorLocation(BuildLocation);
	}
}

void UBuildSystem::FoundationBuild_Implementation(int CID, int UID)
{
	if (FoundationBase == nullptr) {
		return;
	}
	if (FoundationBase -> IsBlock) {
		Pop(Lang . Get(FStatic::BuildBlockedText), FStatic::WarningFontMaterial);
		return;
	}
	if (FoundationBase -> EnemyRange) {
		Pop(Lang . Get(FStatic::BuildNearEnemyText), FStatic::WarningFontMaterial);
		return;
	}
	FString Key = CreateBuildingKey(FStatic::Foundation);
	FoundationBase -> Key = Key;
	FoundationBase -> IsSet = true;
	FBuildings Cache;
	Cache . UID = UID;
	Cache . CID = CID;
	Cache . InGameKey = Key;
	Cache . HealthPoints = FStatic::Hundred;
	Cache . Type . Foundation = true;
	Cache . Building = FoundationBase;
	Cache . Location = BuildLocation;
	Cache . Rotation = FoundationBase -> GetActorRotation();
	if (FoundationBase -> IsAttach) {
		for (auto& Item : FoundationBase -> BlockSideCache) {
			if (Item . Value . Right) {
				Global -> Buildings[Item . Key] . Right = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Global -> Buildings[Item . Key] . Low = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Global -> Buildings[Item . Key] . Left = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Global -> Buildings[Item . Key] . Up = Item . Value . Up;
			}
		}
		Cache . Right = FoundationBase -> Right;
		Cache . Low = FoundationBase -> Low;
		Cache . Left = FoundationBase -> Left;
		Cache . Up = FoundationBase -> Up;
		Cache . WallRight = FoundationBase -> WallRight;
		Cache . WallLow = FoundationBase -> WallLow;
		Cache . WallLeft = FoundationBase -> WallLeft;
		Cache . WallUp = FoundationBase -> WallUp;
	}
	Global -> Buildings . Emplace(Key, Cache);
	BuildType = nullptr;
	FoundationBase = nullptr;
}


void UBuildSystem::Wall_Implementation()
{
	if (BuildType != FStatic::Wall) {
		this -> UnSetBuild();
	}
	if (WallBase == nullptr) {
		FActorSpawnParameters Parameters;
		Parameters . Owner = Player;
		Parameters . bNoFail = true;
		WallBase = GetWorld() -> SpawnActor<AWall>(
			FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
			FRotator(FStatic::Zero),
			Parameters
		);
		BuildType = FStatic::Wall;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::WallBlurAttach_Implementation(FHitResult RayHit, int CID, int UID)
{
	if (WallBase != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		const FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		const FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation . Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		const float OriginAngle = FStatic::Ninety - (FStatic::ThreeHundredAndSixty - ViewRotation . Pitch);
		float Angle = OriginAngle;
		if (Angle <= FStatic::Zero) {
			Angle = FStatic::EightyNine;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * FStatic::Hundred;
		if (BuildDistance > FStatic::Thousand) {
			BuildDistance = FStatic::Thousand;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = WallBase -> LandZ;
		if (OriginAngle < -FStatic::HundredAndEighty && OriginAngle > -FStatic::TwoHundredAndSeventy) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + FStatic::TwoHundredAndSeventy)) *
				FStatic::ThousandAndTwoHundred;
			if (Z > FStatic::Thousand) {
				Z = FStatic::Thousand;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Hundred) + FVector(X, Y, Z);
		// 光线击中处附着
		if (RayHit . IsValidBlockingHit()) {
			const FString HitName = RayHit . GetActor() -> GetName();
			const bool IsFoundation = FStr::IsContain(HitName, FStatic::Foundation);
			const bool IsWall = FStr::IsContain(HitName, FStatic::Wall);
			const bool IsFloor = FStr::IsContain(HitName, FStatic::Floor);
			const float Side = WallBase -> HalfYZ;
			FString AttachSide = RayHit . GetComponent() -> GetName();
			if (IsFoundation || IsFloor) {
				const FString Index = RayHit . GetActor() -> Tags[FStatic::Zero] . ToString();
				const FBuildings BuildBlock = Global -> Buildings[Index];
				bool bIsAlly = false;
				if (BuildBlock . CID == CID && CID != FStatic::Zero) {
					bIsAlly = true;
				}
				if (BuildBlock . CID == FStatic::Zero && BuildBlock . UID == UID) {
					bIsAlly = true;
				}
				if (bIsAlly) {
					float CalZ = FStatic::Zero;
					if (IsFoundation) {
						CalZ = FStatic::Thirty;
					}
					if (IsFloor) {
						CalZ = FStatic::Four;
					}
					if (AttachSide == FStatic::Right && !BuildBlock . WallRight) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::Ninety;
					}
					if (AttachSide == FStatic::Low && !BuildBlock . WallLow) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty;
					}
					if (AttachSide == FStatic::Left && !BuildBlock . WallLeft) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy;
					}
					if (AttachSide == FStatic::Up && !BuildBlock . WallUp) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ + Side);
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty;
					}

					if (AttachSide == FStatic::DownWallRight && !BuildBlock . DownWallRight) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::Ninety;
					}
					if (AttachSide == FStatic::DownWallLow && !BuildBlock . DownWallLow) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty;
					}
					if (AttachSide == FStatic::DownWallLeft && !BuildBlock . DownWallLeft) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy;
					}
					if (AttachSide == FStatic::DownWallUp && !BuildBlock . DownWallUp) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, -(CalZ + Side));
						BuildRotation = BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty;
					}
				}
			}
			if (IsWall) {
				const FString Index = RayHit . GetActor() -> Tags[FStatic::Zero] . ToString();
				const FBuildings BuildBlock = Global -> Buildings[Index];
				bool bIsAlly = false;
				if (BuildBlock . CID == CID && CID != FStatic::Zero) {
					bIsAlly = true;
				}
				if (BuildBlock . CID == FStatic::Zero && BuildBlock . UID == UID) {
					bIsAlly = true;
				}
				if (bIsAlly) {
					if (AttachSide == FStatic::Low && !BuildBlock . Low) {
						BuildLocation = BuildBlock . Location +
							FVector(FStatic::Zero, FStatic::Zero, -Side * FStatic::Two);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Up && !BuildBlock . Up) {
						BuildLocation = BuildBlock . Location +
							FVector(FStatic::Zero, FStatic::Zero, Side * FStatic::Two);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
				}
			}
		}
		WallBase -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		WallBase -> SetActorLocation(BuildLocation);
	}
}

void UBuildSystem::WallBuild_Implementation(int CID, int UID)
{
	if (WallBase == nullptr) {
		return;
	}
	if (WallBase -> IsBlock) {
		Pop(Lang . Get(FStatic::BuildBlockedText), FStatic::WarningFontMaterial);
		return;
	}
	if (!WallBase -> IsAttach) {
		Pop(Lang . Get(FStatic::BuildNoFoundationText), FStatic::WarningFontMaterial);
		return;
	}
	if (WallBase -> EnemyRange) {
		Pop(Lang . Get(FStatic::BuildNearEnemyText), FStatic::WarningFontMaterial);
		return;
	}
	FString Key = CreateBuildingKey(FStatic::Wall);
	WallBase -> Key = Key;
	WallBase -> IsSet = true;
	FBuildings Cache;
	Cache . UID = UID;
	Cache . CID = CID;
	Cache . InGameKey = Key;
	Cache . HealthPoints = FStatic::Hundred;
	Cache . Type . Wall = true;
	Cache . Building = WallBase;
	Cache . Location = BuildLocation;
	Cache . Rotation = WallBase -> GetActorRotation();
	for (auto& Item : WallBase -> BlockSideCache) {
		if (Global -> Buildings[Item . Key] . Type . Foundation || Global -> Buildings[Item . Key] . Type . Floor) {
			if (Item . Value . Right) {
				Global -> Buildings[Item . Key] . WallRight = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Global -> Buildings[Item . Key] . WallLow = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Global -> Buildings[Item . Key] . WallLeft = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Global -> Buildings[Item . Key] . WallUp = Item . Value . Up;
			}
			if (Item . Value . DownWallRight) {
				Global -> Buildings[Item . Key] . DownWallRight = Item . Value . DownWallRight;
			}
			if (Item . Value . DownWallLow) {
				Global -> Buildings[Item . Key] . DownWallLow = Item . Value . DownWallLow;
			}
			if (Item . Value . DownWallLeft) {
				Global -> Buildings[Item . Key] . DownWallLeft = Item . Value . DownWallLeft;
			}
			if (Item . Value . DownWallUp) {
				Global -> Buildings[Item . Key] . DownWallUp = Item . Value . DownWallUp;
			}
		} else {
			if (Item . Value . Low) {
				Global -> Buildings[Item . Key] . Low = Item . Value . Low;
			} else if (Item . Value . Up) {
				Global -> Buildings[Item . Key] . Up = Item . Value . Up;
			}
		}
	}
	Cache . Front = WallBase -> Front;
	Cache . Back = WallBase -> Back;
	Cache . Low = WallBase -> Low;
	Cache . Up = WallBase -> Up;
	Global -> Buildings . Emplace(Key, Cache);
	BuildType = nullptr;
	WallBase = nullptr;
}

void UBuildSystem::Floor_Implementation()
{
	if (BuildType != FStatic::Floor) {
		this -> UnSetBuild();
	}
	if (FloorBase == nullptr) {
		FActorSpawnParameters Parameters;
		Parameters . Owner = Player;
		Parameters . bNoFail = true;
		FloorBase = GetWorld() -> SpawnActor<AFloor>(
			FVector(FStatic::Zero, FStatic::Zero, FStatic::FiveThousand),
			FRotator(FStatic::Zero),
			Parameters
		);
		BuildType = FStatic::Floor;
	} else {
		this -> UnSetBuild();
	}
}

void UBuildSystem::FloorBlurAttach_Implementation(FHitResult RayHit, int CID, int UID)
{
	if (FloorBase != nullptr) {
		//视角旋转的Yaw值是极坐标ρ
		const FRotator ViewRotation = Player -> GetController() -> GetControlRotation();
		const FVector MainLocation = Player -> GetActorLocation();
		float BuildRotation = ViewRotation . Yaw;
		//ρ值有一个变化区间可以使建筑前后移动，这个变化区间值的求出应当使用人物与摄像机的夹角 z*tan
		const float OriginAngle = FStatic::Ninety - (FStatic::ThreeHundredAndSixty - ViewRotation . Pitch);
		float Angle = OriginAngle;
		if (Angle <= FStatic::Zero) {
			Angle = FStatic::EightyNine;
		}
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(Angle)) * FStatic::Hundred;
		if (BuildDistance > FStatic::Thousand) {
			BuildDistance = FStatic::Thousand;
		}
		/**
		 * x = ρcos
		 * y = ρsin
		 */
		float X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * BuildDistance;
		float Z = FloorBase -> LandZ;
		if (OriginAngle < -FStatic::HundredAndEighty && OriginAngle > -FStatic::TwoHundredAndSeventy) {
			Z = FMath::Tan(FMath::DegreesToRadians(OriginAngle + FStatic::TwoHundredAndSeventy)) * FStatic::FiveHundred;
			if (Z > FStatic::Thousand) {
				Z = FStatic::Thousand;
			}
			X = FMath::Cos(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
			Y = FMath::Sin(FMath::DegreesToRadians(ViewRotation . Yaw)) * (FStatic::ThousandOne - Z);
		}
		BuildLocation = FVector(MainLocation . X, MainLocation . Y, FStatic::Zero) + FVector(X, Y, Z);
		// 光线击中处附着
		if (RayHit . IsValidBlockingHit()) {
			const FString HitName = RayHit . GetActor() -> GetName();
			const bool IsFoundation = FStr::IsContain(HitName, FStatic::Foundation);
			const bool IsWall = FStr::IsContain(HitName, FStatic::Wall);
			const bool IsFloor = FStr::IsContain(HitName, FStatic::Floor);
			FString AttachSide = RayHit . GetComponent() -> GetName();
			if (IsFoundation || IsFloor) {
				const FString Index = RayHit . GetActor() -> Tags[FStatic::Zero] . ToString();
				const FBuildings BuildBlock = Global -> Buildings[Index];
				bool bIsAlly = false;
				if (BuildBlock . CID == CID && CID != FStatic::Zero) {
					bIsAlly = true;
				}
				if (BuildBlock . CID == FStatic::Zero && BuildBlock . UID == UID) {
					bIsAlly = true;
				}
				if (bIsAlly) {
					const float Side = FloorBase -> HalfXY * FStatic::Two;
					float CalZ = FStatic::Zero;
					if (IsFoundation) {
						CalZ = FStatic::TwentyFive;
					}
					if (IsFloor) {
						CalZ = FStatic::Zero;
					}
					if (AttachSide == FStatic::Right && !BuildBlock . Right) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Low && !BuildBlock . Low) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Left && !BuildBlock . Left) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Up && !BuildBlock . Up) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::DownWallRight && !BuildBlock . Right) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::Ninety)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::DownWallLow && !BuildBlock . Low) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::DownWallLeft && !BuildBlock . Left) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::TwoHundredAndSeventy)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::DownWallUp && !BuildBlock . Up) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, CalZ);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
				}
			}
			if (IsWall) {
				const FString Index = RayHit . GetActor() -> Tags[FStatic::Zero] . ToString();
				const FBuildings BuildBlock = Global -> Buildings[Index];
				bool bIsAlly = false;
				if (BuildBlock . CID == CID && CID != FStatic::Zero) {
					bIsAlly = true;
				}
				if (BuildBlock . CID == FStatic::Zero && BuildBlock . UID == UID) {
					bIsAlly = true;
				}
				if (bIsAlly) {
					const float Side = FloorBase -> HalfXY;
					if (AttachSide == FStatic::Front && !BuildBlock . Front) {
						const float CalX = FMath::Cos(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						const float CalY = FMath::Sin(
							FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::HundredAndEighty)) * Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::TwoHundredAndFour);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
					if (AttachSide == FStatic::Back && !BuildBlock . Back) {
						const float CalX = FMath::Cos(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						const float CalY = FMath::Sin(
								FMath::DegreesToRadians(BuildBlock . Rotation . Yaw + FStatic::ThreeHundredAndSixty)) *
							Side;
						BuildLocation = BuildBlock . Location + FVector(CalX, CalY, FStatic::TwoHundredAndFour);
						BuildRotation = BuildBlock . Rotation . Yaw;
					}
				}
			}
		}
		FloorBase -> SetActorRotation(FRotator(FStatic::Zero, BuildRotation, FStatic::Zero));
		FloorBase -> SetActorLocation(BuildLocation);
	}
}

void UBuildSystem::FloorBuild_Implementation(int CID, int UID)
{
	if (FloorBase == nullptr) {
		return;
	}
	if (FloorBase -> IsBlock) {
		Pop(Lang . Get(FStatic::BuildBlockedText), FStatic::WarningFontMaterial);
		return;
	}
	if (!FloorBase -> IsAttach) {
		Pop(Lang . Get(FStatic::BuildNoFoundationText), FStatic::WarningFontMaterial);
		return;
	}
	if (FloorBase -> EnemyRange) {
		Pop(Lang . Get(FStatic::BuildNearEnemyText), FStatic::WarningFontMaterial);
		return;
	}
	FString Key = CreateBuildingKey(FStatic::Floor);
	FloorBase -> Key = Key;
	FloorBase -> IsSet = true;
	FBuildings Cache;
	Cache . UID = UID;
	Cache . CID = CID;
	Cache . InGameKey = Key;
	Cache . HealthPoints = FStatic::Hundred;
	Cache . Type . Floor = true;
	Cache . Building = FloorBase;
	Cache . Location = BuildLocation;
	Cache . Rotation = FloorBase -> GetActorRotation();
	for (auto& Item : FloorBase -> BlockSideCache) {
		if (Global -> Buildings[Item . Key] . Type . Foundation || Global -> Buildings[Item . Key] . Type . Floor) {
			if (Item . Value . Right) {
				Global -> Buildings[Item . Key] . Right = Item . Value . Right;
			}
			if (Item . Value . Low) {
				Global -> Buildings[Item . Key] . Low = Item . Value . Low;
			}
			if (Item . Value . Left) {
				Global -> Buildings[Item . Key] . Left = Item . Value . Left;
			}
			if (Item . Value . Up) {
				Global -> Buildings[Item . Key] . Up = Item . Value . Up;
			}
		} else {
			if (Item . Value . Front) {
				Global -> Buildings[Item . Key] . Front = Item . Value . Front;
			} else if (Item . Value . Back) {
				Global -> Buildings[Item . Key] . Back = Item . Value . Back;
			}
		}
	}
	Cache . Right = FloorBase -> Right;
	Cache . Low = FloorBase -> Low;
	Cache . Left = FloorBase -> Left;
	Cache . Up = FloorBase -> Up;
	Cache . WallRight = FloorBase -> WallRight;
	Cache . WallLow = FloorBase -> WallLow;
	Cache . WallLeft = FloorBase -> WallLeft;
	Cache . WallUp = FloorBase -> WallUp;
	Cache . DownWallRight = FloorBase -> DownWallRight;
	Cache . DownWallLow = FloorBase -> DownWallLow;
	Cache . DownWallLeft = FloorBase -> DownWallLeft;
	Cache . DownWallUp = FloorBase -> DownWallUp;
	Global -> Buildings . Emplace(Key, Cache);
	BuildType = nullptr;
	FloorBase = nullptr;
}
