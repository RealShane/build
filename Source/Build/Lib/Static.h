#pragma once

#include "CoreMinimal.h"

/**
 * 工具类
 */
class BUILD_API FStatic
{
public:

	/**
	 * 常用数值
	 */
	constexpr static int Zero = 0;
	constexpr static int One = 1;
	constexpr static int Two = 2;
	constexpr static float TwoPointFive = 2.5;
	constexpr static int Three = 3;
	constexpr static int Four = 4;
	constexpr static int Five = 5;
	constexpr static int Six = 6;
	constexpr static int Seven = 7;
	constexpr static int Eight = 8;
	constexpr static int Nine = 9;
	constexpr static int Ten = 10;
	constexpr static int Fifteen = 15;
	constexpr static int Twenty = 20;
	constexpr static int TwentyFive = 25;
	constexpr static int Thirty = 30;
	constexpr static int ThirtyOne = 31;
	constexpr static int FortyTwo = 42;
	constexpr static int Fifty = 50;
	constexpr static int FiftyOne = 51;
	constexpr static int FiftyFive = 55;
	constexpr static int Sixty = 60;
	constexpr static int Eighty = 80;
	constexpr static int EightyFive = 85;
	constexpr static int EightyNine = 89;
	constexpr static int Ninety = 90;
	constexpr static int NinetyFive = 95;
	constexpr static int Hundred = 100;
	constexpr static int HundredAndTen = 110;
	constexpr static int HundredAndTwenty = 120;
	constexpr static int HundredAndTwentyFive = 125;
	constexpr static int HundredAndForty = 140;
	constexpr static int HundredAndFifty = 150;
	constexpr static int HundredAndFiftyOne = 151;
	constexpr static int HundredAndFiftyFive = 155;
	constexpr static int HundredAndSixty = 160;
	constexpr static int HundredAndSixtyFive = 165;
	constexpr static int HundredAndSeventyFive = 175;
	constexpr static int HundredAndEighty = 180;
	constexpr static int HundredAndNinety = 190;
	constexpr static int HundredAndNinetyFive = 195;
	constexpr static int HundredAndNinetySix = 196;
	constexpr static int HundredAndNinetySeven = 197;
	constexpr static int HundredAndNinetyEight = 198;
	constexpr static int TwoHundred = 200;
	constexpr static int TwoHundredAndOne = 201;
	constexpr static int TwoHundredAndTwo = 202;
	constexpr static int TwoHundredAndThree = 203;
	constexpr static int TwoHundredAndFour = 204;
	constexpr static int TwoHundredAndFive = 205;
	constexpr static int TwoHundredAndTen = 210;
	constexpr static int TwoHundredAndThirty = 230;
	constexpr static int TwoHundredAndForty = 240;
	constexpr static int TwoHundredAndFifty = 250;
	constexpr static int TwoHundredAndSeventy = 270;
	constexpr static int ThreeHundredAndSixty = 360;
	constexpr static int FiveHundred = 500;
	constexpr static int FiveHundredAndForty = 540;
	constexpr static int SixHundred = 600;
	constexpr static int EightHundred = 800;
	constexpr static int Thousand = 1000;
	constexpr static int ThousandOne = 1001;
	constexpr static int ThousandAndTwoHundred = 1200;
	constexpr static int ThousandAndFiveHundred = 1500;
	constexpr static int FourThousand = 4000;
	constexpr static int FiveThousand = 5000;
	constexpr static int NineThousandAndNineHundred = 9900;
	constexpr static int TenThousand = 10000;
	constexpr static int FiftyThousand = 50000;

	/**
	 * 建筑有关字符
	 */
	inline static FString Foundation = "Foundation";
	inline static FString Wall = "Wall";
	inline static FString Floor = "Floor";

	//建筑边方向
	inline static FString Right = "Right";
	inline static FString Low = "Low";
	inline static FString Left = "Left";
	inline static FString Up = "Up";
	inline static FString WallRight = "WallRight";
	inline static FString WallLow = "WallLow";
	inline static FString WallLeft = "WallLeft";
	inline static FString WallUp = "WallUp";
	inline static FString DownWallRight = "DownWallRight";
	inline static FString DownWallLow = "DownWallLow";
	inline static FString DownWallLeft = "DownWallLeft";
	inline static FString DownWallUp = "DownWallUp";
	inline static FString Down = "Down";
	inline static FString Front = "Front";
	inline static FString Back = "Back";
	
	//材料
	inline static FString BlurMaterial = "Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'";
	inline static FString WoodMaterial = "Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'";
	//网格体
	inline static FString CubeStaticMesh = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'";

	/**
	 * 人物有关字符
	 */
	inline static FString Idle = "AnimSequence'/Game/Mannequin/Animations/ThirdPersonIdle.ThirdPersonIdle'";
	inline static FString Jog = "AnimSequence'/Game/Mannequin/Animations/ThirdPersonRun.ThirdPersonRun'";
	inline static FString JumpAnim = "AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_Start.ThirdPersonJump_Start'";
	inline static FString JumpLoopAnim = "AnimSequence'/Game/Mannequin/Animations/ThirdPersonJump_Loop.ThirdPersonJump_Loop'";
	
	inline static FString RightHandSocket = "hand_r";

	//骨骼模型
	inline static FString MannequinFemale = "SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'";

	/**
	 * 装备有关字符
	 */
	inline static FString Torch = "StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'";
	inline static FString TorchFire = "ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'";

	/**
	 * 组件名
	 */
	inline static FString StaticMeshComponent = "StaticMeshComponent";
	inline static FString BoxComponent = "BoxComponent";
	inline static FString InputComponent = "InputComponent";
	inline static FString SpringArmComponent = "SpringArmComponent";
	inline static FString CameraComponent = "CameraComponent";
	inline static FString ParticleSystemComponent = "ParticleSystemComponent";
	inline static FString PointLightComponent = "PointLightComponent";
	inline static FString CollisionCylinder = "CollisionCylinder";
	inline static FString BuildSystem = "BuildSystem";
	inline static FString NetWork = "NetWork";
	inline static FString Range = "Range";
	inline static FString Needle = "Needle";
	inline static FString RightNeedle = "RightNeedle";
	inline static FString LowNeedle = "LowNeedle";
	inline static FString LeftNeedle = "LeftNeedle";
	inline static FString UpNeedle = "UpNeedle";
	inline static FString Landscape = "Landscape";

	/**
	 * 颜色
	 */
	inline static FLinearColor SunYellowColor = FColor(243, 159, 024);
	inline static FLinearColor SnowOne = FColor(255, 250, 250);

	/**
	 * 地图关卡
	 */
	inline static FString DemoMap = "World'/Game/Maps/Demo.Demo'";

	/**
	 * 网络请求有关字符
	 */
	inline static FString ContentType = "Content-Type";
	inline static FString ContentTypeString = "application/json";
	inline static FString Get = "GET";
	inline static FString Post = "POST";
	
	inline static FString WSURL = "ws://localhost:9502";
	inline static FString WS = "WS";
	inline static FString Request = "Request";
	
	inline static FString Status = "status";
	inline static FString Message = "message";
	inline static FString Result = "result";
	inline static FString Data = "Data";
	inline static FString FreshJoin = "FreshJoin";
	inline static FString Move = "Move";

	
	inline static FString Name = "Name";
	inline static FString LocationX = "locationX";
	inline static FString LocationY = "locationY";
	inline static FString LocationZ = "locationZ";
	inline static FString RotationPitch = "rotationPitch";
	inline static FString RotationYaw = "rotationYaw";
	inline static FString RotationRoll = "rotationRoll";

	inline static FString Connect = "Connect";
	inline static FString Guest = "Guest";

	/**
	 * API路由
	 */
	inline static FString Domain = "http://localhost";
	inline static FString Test = "/api/test";

	/**
	 * 时间日期
	 */
	inline static FString DateTime = "%Y-%m-%d-%H-%M-%S";

	/**
	 * 标点符号
	 */
	inline static FString FullStop = ".";
	inline static FString Comma = ",";
	inline static FString ExclamatoryMark = "!";
	inline static FString QuestionMark = "?";
	inline static FString Underline = "_";
};
