/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * AActor.h - Actor class for tangible game objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/UMath.h"
#include "Core/UObject.h"
#include "Engine/UMesh.h"
#include "Engine/URender.h"
#include "Engine/USkelAnim.h"
#include "Engine/UTexture.h"

class APlayerReplicationInfo;
class AProjector;
class AZoneInfo;
class ALevelInfo;
class APawn;
class AInventory;
class ANavigationPoint;
class ATrigger;

// Includes variable state for:
//   Unreal 227j_39
//   UT 436
//   Deus Ex 1.112
class LIBUNR_API AActor : public UObject
{
  DECLARE_NATIVE_CLASS( AActor, UObject, CLASS_NativeReplication, Engine )
  EXPOSE_TO_USCRIPT()

  AActor();
  virtual void Tick( float DeltaTime ) {}
  virtual void PostDefaultLoad();

  // Flags
  bool bStatic, bHidden, bNoDelete, bAnimFinished, bAnimLoop;
  bool bAnimNotify, bAnimByOwner, bDeleteMe, bAssimilated;
  bool bTicked, bLightChanged, bDynamicLight, bTimerLoop;

  // 227 booleans (how much of this is needed?)
  bool bNetNotify, bHandleOwnCorona, bRenderMultiEnviroMaps, bWorldGeometry;
  bool bUseMeshCollision, bEditorSelectRender, bNoDynamicShadowCast;
  bool bIsInOctree, bProjectorDecal, bUseLitSprite;

  // UT99 booleans
  bool bTrailerPrePivot, bClientAnim, bSimFall, bClientDemoRecording, bClientDemoNetFunc;

  // Editing flags
  bool bHiddenEd, bDirectional, bSelected, bMemorized;
  bool bHighlighted, bEdLocked, bEdShouldSnap, bEdSnap;
  bool bTempEditor;

  // Gameplay scenario flags
  bool bDifficulty0, bDifficulty1, bDifficulty2, bDifficulty3;
  bool bSinglePlayer, bNet, bNetSpecial;

  // Prevent re-init
  bool bScriptInitialized;

  // Other flags
  bool bCanTeleport, bIsSecretGoal, bIsKillGoal, bIsItemGoal;
  bool bCollideWhenPlacing, bTravel, bMovable, bHighDetail;
  bool bStasis, bForceStasis, bIsPawn, bNetTemporary;
  bool bNetOptional, bReplicateInstigator, bTrailerSameRotation;

  // Display
  bool bUnlit, bNoSmooth, bParticles, bRandomFrame;
  bool bMeshEnviroMap, bFilterByVolume, bMeshCurvy;
  bool bShadowCast;

  // Advanced
  bool bOwnerNoSee, bOnlyOwnerSee, bIsMover, bAlwaysRelevant;
  bool bAlwaysTick, bHurtEntry, bGameRelevant, bCarriedItem;
  bool bForcePhysicsUpdate;

  // 227 network modifiers (do we even need this?)
  bool bSkipActorReplication, bRepAnimations, bRepAmbientSound;
  bool bSimulatedPawnRep, bRepMesh;

  // Collision flags
  bool bCollideActors, bCollideWorld, bBlockActors, bBlockPlayers;
  bool bProjTarget, bPathCollision;

  // Lighting
  bool bSpecialLit, bActorShadows, bCorona, bLensFlare, bDarkLight;

  // Replication (again, needed or not?)
  bool bNetInitial, bNetOwner, bNetRelevant, bNetSee, bNetHear;
  bool bNetFeel, bSimulatedPawn, bDemoRecording;

  // Physics
  bool bBounce, bFixedRotationDir, bRotateToDesired;
  bool bInterpolating, bJustTeleported;

  // 227 variables
  bool bAlwaysRender;
  bool bIsPlayerPawn;
  bool bIsProjectile;
  bool bZoneNormalLight;
  bool bIsFrobable;
  float RandomDelayTime, RandomValue, LastRandomTime, LastRenderedTime;
  FColor ActorRenderColor;
  FColor ActorGUnlitColor;
  FColor AmbientGlowColor;
  float AmbientGlowPulseSpeed;
  float NormalLightRadius;
  UPrimitive* CollisionOverride;
  USkeletalMeshInstance* MeshInstance;
  FVector RelativeLocation;
  FRotator RelativeRotation;
  void* LightDataPtr;
  void* MeshDataPtr;
  FVector DrawScale3D;
  TArray<AProjector*>* ProjectorList;
  void* NetInitialProperties;
  TArray<AActor*>* RealTouching;

  struct MultiTimerType
  {
    idx Func;
    float Rate, Counter;
  };
  TArray<MultiTimerType>* MultiTimers;

  // Current physics mode
  enum EPhysics
  {
    PHYS_None,
    PHYS_Walking,
    PHYS_Falling,
    PHYS_Swimming,
    PHYS_Flying,
    PHYS_Rotating,
    PHYS_Projectile,
    PHYS_Rolling,
    PHYS_Interpolating,
    PHYS_MovingBrush,
    PHYS_Spider,
    PHYS_Trailer
  };
  EPhysics Physics;

  // More net variables
  enum ENetRole
  {
    ROLE_None,             // No role at all.
    ROLE_DumbProxy,        // Dumb proxy of this actor.
    ROLE_SimulatedProxy,   // Locally simulated proxy of this actor.
    ROLE_AutonomousProxy,  // Locally autonomous proxy of this actor.
    ROLE_Authority,        // Authoritative control over the actor.
  };
  ENetRole Role, RemoteRole;
  
  // Owner
  AActor* Owner;
  idx InitialState;
  idx Group;

  // Execution and timer variables
  float TimerRate, TimerCounter, LifeSpan;

  // Animation variables
  idx AnimSequence;
  float AnimFrame, AnimRate, TweenRate;
  UAnimation* SkelAnim;
  float LODBias;
  UClass* DefaultAnimationNotify;
  UAnimationNotify* AnimationNotify;

  // Structures
  struct PointRegion
  {
    AZoneInfo* Zone;
    int iLeaf;
    u8  ZoneNumber;
  };

  // Major actor properties
  ALevelInfo* Level;
  class ULevel* XLevel;
  FName       Tag;
  FName       Event;
  AActor*     Target;
  APawn*      Instigator;
  AInventory* Inventory;
  AActor*     Base;
  PointRegion Region;
  FName       AttachTag;

  // Internal
  u8      StandingCount;
  u8      MiscNumber;
  u8      LatentByte;
  int     LatentInt;
  float   LatentFloat;
  AActor* LatentActor;
  AActor* Touching[4];
  AActor* Deleted;

  // Internal tags
  int CollisionTag, LightingTag, NetTag, OtherTag, ExtraTag, SpecialTag;

  // Actor position and rotation
  FVector  Location;
  FRotator Rotation;
  FVector  OldLocation;
  FVector  ColLocation; // Actor's old location one move ago
  FVector  Velocity;
  FVector  Acceleration;
  float    OddsOfAppearing;

  // Editor support (?)
  AActor* HitActor;

  //-----------------------------------------------------------------------------
  // Display properties

  // Drawing effect
  enum EDrawType
  {
    DT_None,
    DT_Sprite,
    DT_Mesh,
    DT_Brush,
    DT_RopeSprite,
    DT_VerticalSprite,
    DT_Terraform,
    DT_SpriteAnimOnce,
  };
  EDrawType DrawType;

  // Style for rendering sprites, meshes
  enum ERenderStyle
  {
    STY_None,
    STY_Normal,
    STY_Masked,
    STY_Translucent,
    STY_Modulated,
    STY_AlphaBlend,
  };
  ERenderStyle Style;

  // Other display properties
  UTexture* Sprite;
  UTexture* Texture;
  UTexture* Skin;
  UMesh*    Mesh;
  UMesh*    ShadowMesh;
  class UModel*   Brush;
  float     DrawScale;
  FVector   PrePivot;
  float     ScaleGlow;
  float     VisibilityRadius, VisibilityHeight;
  u8        AmbientGlow;
  u8        Fatness;
  u16       _pad0;
  float     SpriteProjForward;

  // Multiple skin support
  UTexture* MultiSkins[8];

  // Render Iterator
  UClass* RenderIteratorClass;
  URenderIterator* RenderInterface;

  //-----------------------------------------------------------------------------
  // Sound

  // Ambient sound
  u8 SoundRadius;
  u8 SoundVolume;
  u8 SoundPitch;
  u8 _pad1;
  USound* AmbientSound;

  // Regular sounds;
  float TransientSoundVolume, TransientSoundRadius;

  // Sound slots for actors.
  enum ESoundSlot
  {
    SLOT_None,
    SLOT_Misc,
    SLOT_Pain,
    SLOT_Interact,
    SLOT_Ambient,
    SLOT_Talk,
    SLOT_Interface,
  };
  
  // Music transitions.
  enum EMusicTransition
  {
    MTRAN_None,
    MTRAN_Instant,
    MTRAN_Segue,
    MTRAN_Fade,
    MTRAN_FastFade,
    MTRAN_SlowFade,
  };
  
  //-----------------------------------------------------------------------------
  // Collision.
  
  // Collision size.
  float CollisionRadius; // Radius of collision cyllinder.
  float CollisionHeight; // Half-height cyllinder.
  float CollisionWidth;  // Y of collision box.
  enum ECollideType
  {
    CT_AlignedCylinder,
    CT_OrientedCylinder,
    CT_Box,
    CT_Shape,
  } CollideType;
  
  //-----------------------------------------------------------------------------
  // Lighting.
  
  // Light modulation.
  enum ELightType
  {
    LT_None,
    LT_Steady,
    LT_Pulse,
    LT_Blink,
    LT_Flicker,
    LT_Strobe,
    LT_BackdropLight,
    LT_SubtlePulse,
    LT_TexturePaletteOnce,
    LT_TexturePaletteLoop,
    LT_RandomPulse, //using random LightPeriod.
    LT_RandomSubtlePulse //using random LightPeriod.
  }; 
  ELightType LightType;
  
  // Spatial light effect to use.
  enum ELightEffect
  {
    LE_None,
    LE_TorchWaver,
    LE_FireWaver,
    LE_WateryShimmer,
    LE_Searchlight,
    LE_SlowWave,
    LE_FastWave,
    LE_CloudCast,
    LE_StaticSpot,
    LE_Shock,
    LE_Disco,
    LE_Warp,
    LE_Spotlight,
    LE_NonIncidence,
    LE_Shell,
    LE_OmniBumpMap,
    LE_Interference,
    LE_Cylinder,
    LE_Rotor,
    LE_Sunlight
  };
  ELightEffect LightEffect;
  
  // Lighting info.
  u8 LightBrightness, LightHue, LightSaturation;

  // Light properties
  u8 LightRadius, LightPeriod, LightPhase, LightCone;
  u8 VolumeBrightness, VolumeRadius, VolumeFog;
  float CoronaAlpha; // 227g: moved from Render to Actor

  //-----------------------------------------------------------------------------
  // Physics

  // Dodge move direction
  enum EDodgeDir
  {
    DODGE_None,
    DODGE_Left,
    DODGE_Right,
    DODGE_Forward,
    DODGE_Back,
    DODGE_Active,
    DODGE_Done
  };
  EDodgeDir DodgeDir;

  // Physics properties
  float Mass;
  float Buoyancy;
  FRotator RotationRate;
  FRotator DesiredRotation;
  float PhysAlpha;
  float PhysRate;
  AActor* PendingTouch;

  //-----------------------------------------------------------------------------
  // Animation
  
  // Animation control
  float  AnimLast;
  float  AnimMinRate;
  float  OldAnimRate;
  FPlane SimAnim;

  //-----------------------------------------------------------------------------
  // Networking

  // Network control
  float NetPriority;
  float NetUpdateFrequency;

  //-----------------------------------------------------------------------------
  // Enums

  // Travelling from server to server
  enum ETravelType
  {
    TRAVEL_Absolute, // Absolute URL
    TRAVEL_Partial,  // Partial (carry name, reset server)
    TRAVEL_Relative, // Relative URL
  };

  // Input system states.
  enum EInputAction
  {
    IST_None,    // Not performing special input processing.
    IST_Press,   // Handling a keypress or button press.
    IST_Hold,    // Handling holding a key or button.
    IST_Release, // Handling a key or button release.
    IST_Axis,    // Handling analog axis movement.
  };

  // Deus Ex Variables
  bool  bBlockSight;
  bool  bDetectable;
  bool  bTransient;
  bool  bIgnore;
  float LastRenderTime;
  float DistanceFromPlayer;
  FName BlendAnimSequence[4];
  float BlendAnimFrame[4];
  float BlendAnimRate[4];
  float BlendTweenRate[4];
  float BlendAnimLast[4];
  float BlendAnimMinRate[4];
  float OldBlendAnimRate[4];
  FPlane SimBlendAnim[4];
  FString* BindName;
  FString* BarkBindName;
  FString* FamiliarName;
  FString* UnfamiliarName;
  UObject* ConListItems;
  float    LastConEndTime;
  float    ConStartInterval;
  float    VisUpdateTime;
  float    CurrentVisibility;
  float    LastVisibility;
  UClass*  SmellClass;
  class ASmellNode* LastSmellNode;
  bool     bOwned;
  bool     bVisionImportant;
  float    RelevantRadius;

  // Rune Variables
  bool bReleaseLock;
  bool bRenderedLastFrame;
  bool bSpecialRender;
  bool bFrameNotifies;
  bool bLookFocusPlayer;
  bool bLookFocusCreature;
  bool bForceRender;
  u8 BaseJoint;
  u8 BaseMatterType;
  FVector BaseScrollDir;
  bool bHasShadow;
  class UShadowTexture* ShadowTexture;
  FVector ShadowVector;
  FVector ColorAdjust;
  FVector DesiredColorAdjust;
  u8 DesiredFatness;
  float AlphaScale;
  int LODPolyCount;
  float LODDistMax;
  float LODDistMin;
  float LODPercentMin;
  float LODPercentMax;
  u8 LODCurve;
  bool bPointLight;
  bool bMirrored;
  bool bPreLight;
  bool bComplexOcclusion;
  AActor* AttachParent;
  u8 AttachParentJoint;
  u8 IncrementStat;
  bool bJointsBlock;
  bool bJointsTouch;
  bool bSweepable;
  bool bSpecialLit2;
  bool bSpecialLit3;
  bool bNegativeLight;
  bool bAffectWorld;
  bool bAffectActors;
  bool bNoSurfaceBob;
  bool bDrawSkel;
  bool bDrawJoints;
  bool bDrawAxes;
  bool bApplyLagToAccelerators;
  u8 SkelMesh;
  class USkelModel* Skeletal;
  class USkelModel* SubstituteMesh;
  float BlendAnimAlpha;
  class AAnimationProxy* AnimProxy;
  UTexture* SkelGroupSkins[16];
  int SkelGroupFlags[16];
  u8 JointFlags[50];
  AActor* JointChild[50];
};

class LIBUNR_API ABlockingActor : public AActor
{
  DECLARE_NATIVE_CLASS( ABlockingActor, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ABlockingActor();

  bool bBlockSubClasses;
  TArray<UClass*>* BlockingClasses;
  TArray<UClass*>* IgnoreSubClasses;
  TArray<UClass*>* ScriptBlocking;
};

class LIBUNR_API ADecoration : public AActor
{
  DECLARE_NATIVE_CLASS( ADecoration, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADecoration();

  bool bPushable;
  bool bOnlyTriggerable;
  bool bSplash;
  bool bBobbing;
  bool bWasCarried;
  bool bPushSoundPlaying;

  UClass* EffectWhenDestroyed;
  USound* PushSound;
  int     NumLandings;
  UClass* Contents;
  UClass* Content2;
  UClass* Content3;
  USound* EndPushSound;

  AActor* Shadow;
  
  // Deus Ex Variables
  float BaseEyeHeight;
};

class LIBUNR_API ACarcass : public ADecoration
{
  DECLARE_NATIVE_CLASS( ACarcass, ADecoration, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ACarcass();

  bool bPlayerCarcass;
  u8   Flies;
  u8   Rats;
  bool bReducedHeight;
  bool bDecorative;
  bool bSlidingCarcass;
  int  CumulativeDamage;
  APlayerReplicationInfo* PlayerOwner;
  APawn* Bugs;
};

class LIBUNR_API AInfo : public AActor
{
  DECLARE_NATIVE_CLASS( AInfo, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInfo();
};

class LIBUNR_API ASavedMove : public AInfo
{
  DECLARE_NATIVE_CLASS( ASavedMove, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ASavedMove();

  class ASavedMove* NextMove;
  float TimeStamp;
  float Delta;
  EDodgeDir DodgeMove;
  bool bRun;
  bool bDuck;
  bool bPressedJump;
  bool bSent;
  bool bIsReducedCrouch;
  UClass* CustomPlayerState;

  // UT99 Variables
  bool bFire;
  bool bAltFire;
  bool bForceFire;
  bool bForceAltFire;
};

class LIBUNR_API AKeypoint : public AActor
{
  DECLARE_NATIVE_CLASS( AKeypoint, AActor, CLASS_Abstract, Engine )
  EXPOSE_TO_USCRIPT()

  AKeypoint();
};

class LIBUNR_API ALocationID : public AKeypoint
{
  DECLARE_NATIVE_CLASS( ALocationID, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ALocationID();

  FString* LocationName;
  float Radius;
  ALocationID* NextLocation;
};

class LIBUNR_API ACollisionPlane : public AKeypoint
{
  DECLARE_NATIVE_CLASS( ACollisionPlane, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ACollisionPlane();

  float CollisionBounds[4];
  bool bBlockZeroExtent;
  bool bBlockNonZeroExtent;
};

class LIBUNR_API AInterpolationPoint : public AKeypoint
{
  DECLARE_NATIVE_CLASS( AInterpolationPoint, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AInterpolationPoint();

  int     Position;
  float   RateModifier;
  float   GameSpeedModifier;
  float   FovModifier;
  bool    bEndOfPath;
  bool    bSkipNextPath;
  float   ScreenFlashScale;
  FVector ScreenFlashFog;

  // Rune vars
  bool bInterpInstant;
  bool bSplineThruPoints;

  AInterpolationPoint* Prev;
  AInterpolationPoint* Next;
};

class LIBUNR_API ALight : public AActor
{
  DECLARE_NATIVE_CLASS( ALight, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ALight();

  struct ShadowMoverInfo
  {
    class AMover* Mover;
    TArray<int>* BspSurfs;
  };

  TArray<idx>* ShadowMoverTags;
  TArray<ShadowMoverInfo>* MoverShadowSurfs;
};

class LIBUNR_API ADynamicCorona : public ALight
{
  DECLARE_NATIVE_CLASS( ADynamicCorona, ALight, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADynamicCorona();

  enum ECoronaAttenuateType
  {
    CATT_WorldOnly,
    CATT_WorldNMovers,
    CATT_Actors
  };

  bool bDirectionalCorona;
  bool bBrightnessDependent;
  float MaxSize;
  float DisplayDistance;
  float CoronaSize;
  float FadeOutDistance;
  FColor CoronaColor;
  FColor CloseDistanceColor;
  ECoronaAttenuateType CoronaAttenuate;
};

class LIBUNR_API ASunlightCorona : public ALight
{
  DECLARE_NATIVE_CLASS( ASunlightCorona, ALight, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ASunlightCorona();

  struct LensFlareCastType
  {
    float ZDistance;
    float Scale;
    UTexture* LensTexture;
    FColor LensColor;
  };

  bool bMustMatchZone;
  bool bRenderLensFlares;
  FColor SunlightColor;
  float BlindingScale;
  float BlindingFOV;
  float MaxSkyDistance;
  float SunFadeTimeScale;
  UTexture* SunlightTexture;
  LensFlareCastType LensFlares[8];
  float FlaresDisplayFov;
  bool bUScriptRenderHandler;
};

class LIBUNR_API AMutator : public AActor
{
  DECLARE_NATIVE_CLASS( AMutator, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AMutator();

  AMutator* NextMutator;
  AMutator* NextDamageMutator;
  AMutator* NextMessageMutator;
  AMutator* NextHUDMutator;
  bool bHUDMutator;
  UClass* DefaultWeapon;

  // Rune specific
  UClass* DefaultShield;
};

class LIBUNR_API AMenu : public AActor
{
  DECLARE_NATIVE_CLASS( AMenu, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AMenu();

  AMenu* ParentMenu;
  int    Selection;
  int    MenuLength;
  bool   bConfigChanged;
  bool   bExitAllMenus;
  APlayerPawn* PlayerOwner;

  FString* HelpMessage[26];
  FString* MenuList[26];
  FString* LeftString;
  FString* RightString;
  FString* CenterString;
  FString* EnabledString;
  FString* DisabledString;
  FString* MenuTitle;
  FString* YesString;
  FString* NoString;
  FString* OnString;
  FString* OffString;
};

class LIBUNR_API AProjectile : public AActor
{
  DECLARE_NATIVE_CLASS( AProjectile, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AProjectile();

  float Speed;
  float MaxSpeed;
  
  float Damage;
  int   MomentumTransfer;
  idx   MyDamageType;

  USound* SpawnSound;
  USound* ImpactSound;
  USound* MiscSound;

  float ExploWallOut;
  UClass* ExplosionDecal;

  AActor* LastHitActor;
};

class LIBUNR_API AStaticMeshActor : public AActor
{
  DECLARE_NATIVE_CLASS( AStaticMeshActor, AActor, CLASS_NoUserCreate, Engine )
  EXPOSE_TO_USCRIPT()

  AStaticMeshActor();

  UStaticLightData StaticLightD;
  bool bBuildStaticLights;
  bool bComputeUnlitColor;
};

class LIBUNR_API ASpawnNotify : public AActor
{
  DECLARE_NATIVE_CLASS( ASpawnNotify, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ASpawnNotify();

  UClass* ActorClass;
  ASpawnNotify* Next;
};

class LIBUNR_API ATriggers : public AActor
{
  DECLARE_NATIVE_CLASS( ATriggers, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ATriggers();
};

class LIBUNR_API ADispatcher : public ATriggers
{
  DECLARE_NATIVE_CLASS( ADispatcher, ATriggers, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADispatcher();

  FName OutEvents[8];
  float OutDelays[8];
  bool bIsLooping;
  int i;
};

class LIBUNR_API ATrigger : public ATriggers
{
  DECLARE_NATIVE_CLASS( ATrigger, ATriggers, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ATrigger();

  enum ETriggerType
  {
    TT_PlayerProximity,
    TT_PawnProximity,
    TT_ClassProximity,
    TT_AnyProximity,
    TT_Shoot,
  };
  ETriggerType TriggerType;

  FString* Message;
  bool    bTriggerOnceOnly;
  bool    bInitiallyActive;
  bool    BACKUP_InitActive;

  UClass* ClassProximityType;
  float   RepeatTriggerTime;
  float   ReTriggerDelay;
  float   TriggerTime;
  float   DamageThreshold;

  AActor* TriggerActor;
  AActor* TriggerActor2;

  // Rune Variables
  float SightDistance;
  float SightAngle;
};

class LIBUNR_API AVisibilityNotify : public AInfo
{
  DECLARE_NATIVE_CLASS( AVisibilityNotify, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AVisibilityNotify();

  AVisibilityNotify* NextNotify;
};

