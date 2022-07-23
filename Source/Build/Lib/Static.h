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
	constexpr static float ZeroPointOne = 0.1;
	constexpr static float ZeroPointTwo = 0.2;
	constexpr static float ZeroPointThree = 0.3;
	constexpr static float ZeroPointFour = 0.4;
	constexpr static float ZeroPointFive = 0.5;
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
	constexpr static int Eleven = 11;
	constexpr static int Twelve = 12;
	constexpr static int Thirteen = 13;
	constexpr static int Fourteen = 14;
	constexpr static int Fifteen = 15;
	constexpr static int Twenty = 20;
	constexpr static int TwentyFive = 25;
	constexpr static int Thirty = 30;
	constexpr static int ThirtyOne = 31;
	constexpr static int Forty = 40;
	constexpr static int FortyTwo = 42;
	constexpr static int FortyThree = 43;
	constexpr static int FortyFour = 44;
	constexpr static int FortyFive = 45;
	constexpr static int Fifty = 50;
	constexpr static int FiftyOne = 51;
	constexpr static int FiftyFive = 55;
	constexpr static int Sixty = 60;
	constexpr static int SixtyFive = 65;
	constexpr static int Seventy = 70;
	constexpr static int SeventyFive = 75;
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
	constexpr static int HundredAndSeventy = 170;
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
	constexpr static int FourHundred = 400;
	constexpr static int FourHundredAndFifty = 450;
	constexpr static int FourHundredAndSixty = 460;
	constexpr static int FourHundredAndSeventy = 470;
	constexpr static int FourHundredAndEighty = 480;
	constexpr static int FourHundredAndNinety = 490;
	constexpr static int FiveHundred = 500;
	constexpr static int FiveHundredAndForty = 540;
	constexpr static int SixHundred = 600;
	constexpr static int SevenHundred = 700;
	constexpr static int EightHundred = 800;
	constexpr static int NineHundred = 900;
	constexpr static int Thousand = 1000;
	constexpr static int ThousandOne = 1001;
	constexpr static int ThousandAndTwoHundred = 1200;
	constexpr static int ThousandAndFiveHundred = 1500;
	constexpr static int TwoThousand = 2000;
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

	inline static FString CamelWallRight = "wallRight";
	inline static FString CamelWallLow = "wallLow";
	inline static FString CamelWallLeft = "wallLeft";
	inline static FString CamelWallUp = "wallUp";
	inline static FString CamelDownWallRight = "downWallRight";
	inline static FString CamelDownWallLow = "downWallLow";
	inline static FString CamelDownWallLeft = "downWallLeft";
	inline static FString CamelDownWallUp = "downWallUp";

	inline static FString Down = "Down";
	inline static FString Front = "Front";
	inline static FString Back = "Back";

	//材料
	inline static FString BlurMaterial =
		"Material'/Game/StarterContent/Materials/M_ColorGrid_LowSpec.M_ColorGrid_LowSpec'";
	inline static FString WoodMaterial =
		"Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'";
	//网格体
	inline static FString CubeStaticMesh = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'";

	/**
	 * Main模型
	 */

	//骨骼模型
	inline static FString MainSKM = "SkeletalMesh'/Game/Characters/Main/Main.Main'";
	inline static FString MainAnim = "Class'/Game/Blueprint/Animation/Main/Main_Anim_BP.Main_Anim_BP_C'";
	inline static FString RightHandSocket = "right_hand";

	//Main状态机
	inline static FString BehaviorState = "Behavior";

	//Main行为
	inline static FString Punch = "Punch";
	inline static FString RunPunch = "Run_Punch";

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
	inline static FLinearColor SnowOneColor = FColor(255, 250, 250);
	inline static FLinearColor BlackColor = FColor(0, 0, 0);
	inline static FLinearColor WarningColor = FColor(112, 128, 105);
	inline static FLinearColor SuccessColor = FColor::Turquoise;

	/**
	 * 地图关卡
	 */
	inline static FString IslandMap = "World'/Game/Maps/Island.Island'";
	inline static FString StartMap = "World'/Game/Maps/Start.Start'";

	/**
	 * 网络请求有关字符
	 */
	inline static FString ContentType = "Content-Type";
	inline static FString ContentTypeString = "application/json";
	inline static FString Get = "GET";
	inline static FString Post = "POST";

	inline static FString WSURL = "ws://localhost:9502";
	inline static FString WSRemoteURL = "wss://ue.huihuagongxue.top:9502";
	inline static FString WS = "ws";
	inline static FString WSS = "wss";
	inline static FString WebSockets = "WebSockets";
	inline static FString Request = "Request";
	inline static FString UI = "UI";

	inline static FString Status = "status";
	inline static FString Message = "message";
	inline static FString Result = "result";
	inline static FString Data = "Data";
	inline static FString UID = "uId";
	inline static FString CID = "cId";
	inline static FString Token = "token";
	inline static FString KeyPre = "keyPre";
	inline static FString NickName = "nickName";
	inline static FString Name = "name";
	inline static FString Type = "type";
	inline static FString IP = "ip";

	// 方法名
	inline static FString Pong = "pong";
	inline static FString Sync = "sync";
	inline static FString SyncMethod = "Sync";
	inline static FString HeartBeatPongMethod = "HeartBeatPong";
	inline static FString SaveBuilding = "saveBuilding";
	inline static FString UpdateBuilding = "updateBuilding";


	inline static FString Location = "Location";
	inline static FString Rotation = "Rotation";
	inline static FString X = "X";
	inline static FString Y = "Y";
	inline static FString Z = "Z";
	inline static FString Pitch = "Pitch";
	inline static FString Yaw = "Yaw";
	inline static FString Roll = "Roll";
	inline static FString LocationX = "locationX";
	inline static FString LocationY = "locationY";
	inline static FString LocationZ = "locationZ";
	inline static FString RotationPitch = "rotationPitch";
	inline static FString RotationYaw = "rotationYaw";
	inline static FString RotationRoll = "rotationRoll";
	inline static FString VelocityX = "velocityX";
	inline static FString VelocityY = "velocityY";
	inline static FString VelocityZ = "velocityZ";
	inline static FString IsFalling = "isFalling";
	inline static FString Speed = "speed";
	inline static FString SyncType = "syncType";
	inline static FString Add = "add";
	inline static FString Remove = "remove";
	inline static FString InGameKey = "inGameKey";
	inline static FString HealthPoints = "healthPoints";

	inline static FString MoveType = "moveType";
	inline static FString MoveForward = "moveForward";
	inline static FString MoveRight = "moveRight";
	inline static FString Jump = "jump";
	inline static FString StopJumping = "stopJumping";
	inline static FString ControlYaw = "controlYaw";
	inline static FString Value = "value";

	inline static FString Guest = "Guest";

	/**
	 * API路由
	 */
	inline static FString Domain = "http://localhost";
	inline static FString RemoteDomain = "https://ue.huihuagongxue.top";
	inline static FString LoginUrl = "/api/User/login";
	inline static FString ServerListUrl = "/api/getServerList";

	/**
	 * 时间日期
	 */
	inline static FString DateTimeString = "%Y-%m-%d-%H-%M-%S";

	/**
	 * 标点符号
	 */
	inline static FString Empty = "";
	inline static FString Space = " ";
	inline static FString FullStop = ".";
	inline static FString Comma = ",";
	inline static FString ExclamatoryMark = "!";
	inline static FString QuestionMark = "?";
	inline static FString Underline = "_";
	inline static FString Equal = "=";
	inline static FString And = "&";

	/**
	 * 资产
	 */
	inline static FString TranslucentImage = "Texture2D'/Game/UI/Image/Translucent.Translucent'";
	inline static FString ButtonImage = "Texture2D'/Game/UI/Image/Button.Button'";
	inline static FString LoadingImage = "Texture2D'/Game/UI/Image/Loading_BG.Loading_BG'";
	inline static FString LoginImage = "Texture2D'/Game/UI/Image/Login_BG.Login_BG'";
	inline static FString InputImage = "Texture2D'/Game/UI/Image/Input_BG.Input_BG'";
	inline static FString PopLayerImage = "Texture2D'/Game/UI/Image/PopLayer_BG.PopLayer_BG'";
	inline static FString RedImage = "Texture2D'/Game/UI/Image/Red.Red'";
	inline static FString HealthBarImage = "Texture2D'/Game/UI/Image/HealthBar_BG.HealthBar_BG'";
	inline static FString MaoBiFont = FPaths::ProjectContentDir() + "UI/Font/MaoBi.ttf";
	inline static FString FontMaterial = "Material'/Game/UI/Material/Font_M.Font_M'";
	inline static FString WarningFontMaterial = "Material'/Game/UI/Material/WarningFont_M.WarningFont_M'";

	/**
	 * UI蓝图
	 */
	inline static FString LoginBP = "/Game/Blueprint/UI/Login_WBP.Login_WBP_C";
	inline static FString MenuBP = "/Game/Blueprint/UI/Menu_WBP.Menu_WBP_C";
	inline static FString MainBP = "/Game/Blueprint/UI/Main_WBP.Main_WBP_C";
	inline static FString PopLayerBP = "/Game/Blueprint/UI/PopLayer_WBP.PopLayer_WBP_C";
	inline static FString PopNoticeBP = "/Game/Blueprint/UI/PopNotice_WBP.PopNotice_WBP_C";


	/**
	 * 本地化文字
	 */
	inline static FString TestText = "Test";
	inline static FString HintInputEmailText = "HintInputEmail";
	inline static FString HintInputPasswordText = "HintInputPassword";
	inline static FString LoginButtonText = "LoginButton";
	inline static FString JoinButtonText = "JoinButton";
	inline static FString ServerListReadFailText = "ServerListReadFail";
	inline static FString NeverSelectText = "NeverSelect";
	inline static FString EnteringServerText = "EnteringServer";
	inline static FString NewJoinBroadcastText = "NewJoinBroadcast";
	inline static FString LostConnectionText = "LostConnection";
	inline static FString BuildBlockedText = "BuildBlocked";
	inline static FString BuildNearEnemyText = "BuildNearEnemy";
	inline static FString BuildNoFoundationText = "BuildNoFoundation";
};
