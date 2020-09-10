//
// Copyright 2020 Electronic Arts Inc.
//
// The Command & Conquer Map Editor and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// The Command & Conquer Map Editor and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection
namespace MobiusEditor.RedAlert
{
    public static class ActionDataTypes
    {
        public enum ThemeType
        {
            THEME_QUIET = -3,
            THEME_PICK_ANOTHER = -2,
            THEME_NONE = -1,
            THEME_BIGF,
            THEME_CRUS,
            THEME_FAC1,
            THEME_FAC2,
            THEME_HELL,
            THEME_RUN1,
            THEME_SMSH,
            THEME_TREN,
            THEME_WORK,
            THEME_AWAIT,
            THEME_DENSE_R,
            THEME_FOGGER1A,
            THEME_MUD1A,
            THEME_RADIO2,
            THEME_ROLLOUT,
            THEME_SNAKE,
            THEME_TERMINAT,
            THEME_TWIN,
            THEME_VECTOR1A,
            THEME_MAP,
            THEME_SCORE,
            THEME_INTRO,
            THEME_CREDITS,
            THEME_2ND_HAND,
            THEME_ARAZOID,
            THEME_BACKSTAB,
            THEME_CHAOS2,
            THEME_SHUT_IT,
            THEME_TWINMIX1,
            THEME_UNDER3,
            THEME_VR2,
            THEME_BOG,
            THEME_FLOAT_V2,
            THEME_GLOOM,
            THEME_GRNDWIRE,
            THEME_RPT,
            THEME_SEARCH,
            THEME_TRACTION,
            THEME_WASTELND,
        };

        public enum VocType
        {
            VOC_NONE = -1,
            VOC_GIRL_OKAY,          // "okay"
            VOC_GIRL_YEAH,          // "yeah?"
            VOC_GUY_OKAY,           //	"okay"
            VOC_GUY_YEAH,           // "yeah?"
            VOC_MINELAY1,           // mine layer sound
            VOC_ACKNOWL,            //	"acknowledged"
            VOC_AFFIRM,             //	"affirmative"
            VOC_AWAIT,              //	"awaiting orders"
            VOC_ENG_AFFIRM,     // Engineer: "affirmative"
            VOC_ENG_ENG,            // Engineer: "engineering"
            VOC_NO_PROB,            //	"not a problem"
            VOC_READY,              //	"ready and waiting"
            VOC_REPORT,             //	"reporting"
            VOC_RIGHT_AWAY,     //	"right away sir"
            VOC_ROGER,              //	"roger"
            VOC_UGOTIT,             //	"you got it"
            VOC_VEHIC,              //	"vehicle reporting"
            VOC_YESSIR,             //	"yes sir"
            VOC_SCREAM1,            //	short infantry scream
            VOC_SCREAM3,            //	short infantry scream
            VOC_SCREAM4,            //	short infantry scream
            VOC_SCREAM5,            //	short infantry scream
            VOC_SCREAM6,            //	short infantry scream
            VOC_SCREAM7,            //	short infantry scream
            VOC_SCREAM10,           //	short infantry scream
            VOC_SCREAM11,           //	short infantry scream
            VOC_YELL1,              //	long infantry scream
            VOC_CHRONO,             //	Chronosphere sound.
            VOC_CANNON1,            // Cannon sound (medium).
            VOC_CANNON2,            // Cannon sound (short).
            VOC_IRON1,
            VOC_ENG_MOVEOUT,        // Engineer: "movin' out"
            VOC_SONAR,              // sonar pulse
            VOC_SANDBAG,            // sand bag crunch
            VOC_MINEBLOW,
            VOC_CHUTE1,             // wind swoosh sound
            VOC_DOG_BARK,           // dog bark
            VOC_DOG_WHINE,          // dog whine
            VOC_DOG_GROWL2,     // strong dog growl
            VOC_FIRE_LAUNCH,        // fireball launch sound
            VOC_FIRE_EXPLODE,       // fireball explode sound
            VOC_GRENADE_TOSS,       // grenade toss
            VOC_GUN_5,              // 5 round gun burst (slow).
            VOC_GUN_7,              // 7 round gun burst (fast).
            VOC_ENG_YES,            // Engineer: "yes sir"
            VOC_GUN_RIFLE,          // Rifle shot.
            VOC_HEAL,               // Healing effect.
            VOC_DOOR,               // Hyrdrolic door.
            VOC_INVULNERABLE,       // Invulnerability effect.
            VOC_KABOOM1,            // Long explosion (muffled).
            VOC_KABOOM12,           // Very long explosion (muffled).
            VOC_KABOOM15,           // Very long explosion (muffled).
            VOC_SPLASH,             // Water splash
            VOC_KABOOM22,           // Long explosion (sharp).
            VOC_AACANON3,           // AA-Cannon
            VOC_TANYA_DIE,          // Tanya: scream
            VOC_GUN_5F,             // 5 round gun burst (fast).
            VOC_MISSILE_1,          // Missile with high tech effect.
            VOC_MISSILE_2,          // Long missile launch.
            VOC_MISSILE_3,          // Short missile launch.
            VOC_x6,
            VOC_GUN_5R,             // 5 round gun burst (rattles).
            VOC_BEEP,               // Generic beep sound.
            VOC_CLICK,              //	Generic click sound.
            VOC_SILENCER,           // Silencer.
            VOC_CANNON6,            // Long muffled cannon shot.
            VOC_CANNON7,            // Sharp mechanical cannon fire.
            VOC_TORPEDO,            // Torpedo launch.
            VOC_CANNON8,            // Sharp cannon fire.
            VOC_TESLA_POWER_UP, // Hum charge up.
            VOC_TESLA_ZAP,          // Tesla zap effect.
            VOC_SQUISH,             // Squish effect.
            VOC_SCOLD,              // Scold bleep.
            VOC_RADAR_ON,           // Powering up electronics.
            VOC_RADAR_OFF,          // B movie power down effect.
            VOC_PLACE_BUILDING_DOWN,    // Building slam down sound.
            VOC_KABOOM30,           // Short explosion (HE).
            VOC_KABOOM25,           // Short growling explosion.
            VOC_x7,
            VOC_DOG_HURT,           //	Dog whine.
            VOC_DOG_YES,            // Dog 'yes sir'.
            VOC_CRUMBLE,            // Building crumble.
            VOC_MONEY_UP,           // Rising money tick.
            VOC_MONEY_DOWN,     // Falling money tick.
            VOC_CONSTRUCTION,       // Building construction sound.
            VOC_GAME_CLOSED,        // Long bleep.
            VOC_INCOMING_MESSAGE,   // Soft happy warble.
            VOC_SYS_ERROR,          // Sharp soft warble.
            VOC_OPTIONS_CHANGED,    // Mid range soft warble.
            VOC_GAME_FORMING,       // Long warble.
            VOC_PLAYER_LEFT,        // Chirp sequence.
            VOC_PLAYER_JOINED,  // Reverse chirp sequence.
            VOC_DEPTH_CHARGE,       // Distant explosion sound.
            VOC_CASHTURN,           // Airbrake.
            VOC_TANYA_CHEW,     // Tanya: "Chew on this"
            VOC_TANYA_ROCK,     // Tanya: "Let's rock"
            VOC_TANYA_LAUGH,        // Tanya: "ha ha ha"
            VOC_TANYA_SHAKE,        // Tanya: "Shake it baby"
            VOC_TANYA_CHING,        // Tanya: "Cha Ching"
            VOC_TANYA_GOT,          // Tanya: "That's all you got"
            VOC_TANYA_KISS,     // Tanya: "Kiss it bye bye"
            VOC_TANYA_THERE,        // Tanya: "I'm there"
            VOC_TANYA_GIVE,     // Tanya: "Give it to me"
            VOC_TANYA_YEA,          // Tanya: "Yea?"
            VOC_TANYA_YES,          // Tanya: "Yes sir?"
            VOC_TANYA_WHATS,        // Tanya: "What's up."
            VOC_WALLKILL2,          // Crushing wall sound.
            VOC_x8,
            VOC_TRIPLE_SHOT,        // Three quick shots in succession.
            VOC_SUBSHOW,            // Submarine surfacing.
            VOC_E_AH,               // Einstein "ah"
            VOC_E_OK,               // Einstein "ok"
            VOC_E_YES,              // Einstein "yes"
            VOC_TRIP_MINE,          // mine explosion sound
            VOC_SPY_COMMANDER,  // Spy: "commander?"
            VOC_SPY_YESSIR,     // Spy: "yes sir"
            VOC_SPY_INDEED,     // Spy: "indeed"
            VOC_SPY_ONWAY,          // Spy: "on my way"
            VOC_SPY_KING,           // Spy: "for king and country"
            VOC_MED_REPORTING,  // Medic: "reporting"
            VOC_MED_YESSIR,     // Medic: "yes sir"
            VOC_MED_AFFIRM,     // Medic: "affirmative"
            VOC_MED_MOVEOUT,        // Medic: "movin' out"
            VOC_BEEP_SELECT,        // map selection beep
            VOC_THIEF_YEA,          // Thief: "yea?"
            VOC_ANTDIE,
            VOC_ANTBITE,
            VOC_THIEF_MOVEOUT,  // Thief: "movin' out"
            VOC_THIEF_OKAY,     // Thief: "ok"
            VOC_x11,
            VOC_THIEF_WHAT,     // Thief: "what"
            VOC_THIEF_AFFIRM,       // Thief: "affirmative"
            VOC_STAVCMDR,
            VOC_STAVCRSE,
            VOC_STAVYES,
            VOC_STAVMOV,
            VOC_BUZZY1,
            VOC_RAMBO1,
            VOC_RAMBO2,
            VOC_RAMBO3,
            VOC_MECHYES1,
            VOC_MECHHOWDY1,
            VOC_MECHRISE1,
            VOC_MECHHUH1,
            VOC_MECHHEAR1,
            VOC_MECHLAFF1,
            VOC_MECHBOSS1,
            VOC_MECHYEEHAW1,
            VOC_MECHHOTDIG1,
            VOC_MECHWRENCH1,
            VOC_STBURN1,
            VOC_STCHRGE1,
            VOC_STCRISP1,
            VOC_STDANCE1,
            VOC_STJUICE1,
            VOC_STJUMP1,
            VOC_STLIGHT1,
            VOC_STPOWER1,
            VOC_STSHOCK1,
            VOC_STYES1,
            VOC_CHRONOTANK1,
            VOC_MECH_FIXIT1,
            VOC_MAD_CHARGE,
            VOC_MAD_EXPLODE,
            VOC_SHOCK_TROOP1,
        };

        public enum VoxType
        {
            VOX_NONE = -1,
            VOX_ACCOMPLISHED,                   //	mission accomplished
            VOX_FAIL,                           //	your mission has failed
            VOX_NO_FACTORY,                 //	unable to comply, building in progress
            VOX_CONSTRUCTION,                   //	construction complete
            VOX_UNIT_READY,                 // unit ready
            VOX_NEW_CONSTRUCT,              //	new construction options
            VOX_DEPLOY,                         //	cannot deploy here
            VOX_STRUCTURE_DESTROYED,        // structure destroyed
            VOX_INSUFFICIENT_POWER,         // insufficient power
            VOX_NO_CASH,                        //	insufficient funds
            VOX_CONTROL_EXIT,                   //	battle control terminated
            VOX_REINFORCEMENTS,             //	reinforcements have arrived
            VOX_CANCELED,                       //	canceled
            VOX_BUILDING,                       //	building
            VOX_LOW_POWER,                      //	low power
            VOX_NEED_MO_MONEY,              //	need more funds
            VOX_BASE_UNDER_ATTACK,          //	our base is under attack
            VOX_UNABLE_TO_BUILD,                //	unable to build more
            VOX_PRIMARY_SELECTED,           //	primary building selected
            VOX_MADTANK_DEPLOYED,			// M.A.D. Tank Deployed
            VOX_none4,
            VOX_UNIT_LOST,                      //	unit lost
            VOX_SELECT_TARGET,              // select target
            VOX_PREPARE,                        //	enemy approaching
            VOX_NEED_MO_CAPACITY,           //	silos needed
            VOX_SUSPENDED,                      //	on hold
            VOX_REPAIRING,                      //	repairing
            VOX_none5,
            VOX_none6,
            VOX_AIRCRAFT_LOST,
            VOX_none7,
            VOX_ALLIED_FORCES_APPROACHING,
            VOX_ALLIED_APPROACHING,
            VOX_none8,
            VOX_none9,
            VOX_BUILDING_INFILTRATED,
            VOX_CHRONO_CHARGING,
            VOX_CHRONO_READY,
            VOX_CHRONO_TEST,
            VOX_HQ_UNDER_ATTACK,
            VOX_CENTER_DEACTIVATED,
            VOX_CONVOY_APPROACHING,
            VOX_CONVOY_UNIT_LOST,
            VOX_EXPLOSIVE_PLACED,
            VOX_MONEY_STOLEN,
            VOX_SHIP_LOST,
            VOX_SATALITE_LAUNCHED,
            VOX_SONAR_AVAILABLE,
            VOX_none10,
            VOX_SOVIET_FORCES_APPROACHING,
            VOX_SOVIET_REINFORCEMENTS,
            VOX_TRAINING,
            VOX_ABOMB_READY,
            VOX_ABOMB_LAUNCH,
            VOX_ALLIES_N,
            VOX_ALLIES_S,
            VOX_ALLIES_E,
            VOX_ALLIES_W,
            VOX_OBJECTIVE1,
            VOX_OBJECTIVE2,
            VOX_OBJECTIVE3,
            VOX_IRON_CHARGING,
            VOX_IRON_READY,
            VOX_RESCUED,
            VOX_OBJECTIVE_NOT,
            VOX_SIGNAL_N,
            VOX_SIGNAL_S,
            VOX_SIGNAL_E,
            VOX_SIGNAL_W,
            VOX_SPY_PLANE,
            VOX_FREED,
            VOX_UPGRADE_ARMOR,
            VOX_UPGRADE_FIREPOWER,
            VOX_UPGRADE_SPEED,
            VOX_MISSION_TIMER,
            VOX_UNIT_FULL,
            VOX_UNIT_REPAIRED,
            VOX_TIME_40,
            VOX_TIME_30,
            VOX_TIME_20,
            VOX_TIME_10,
            VOX_TIME_5,
            VOX_TIME_4,
            VOX_TIME_3,
            VOX_TIME_2,
            VOX_TIME_1,
            VOX_TIME_STOP,
            VOX_UNIT_SOLD,
            VOX_TIMER_STARTED,
            VOX_TARGET_RESCUED,
            VOX_TARGET_FREED,
            VOX_TANYA_RESCUED,
            VOX_STRUCTURE_SOLD,
            VOX_SOVIET_FORCES_FALLEN,
            VOX_SOVIET_SELECTED,
            VOX_SOVIET_EMPIRE_FALLEN,
            VOX_OPERATION_TERMINATED,
            VOX_OBJECTIVE_REACHED,
            VOX_OBJECTIVE_NOT_REACHED,
            VOX_OBJECTIVE_MET,
            VOX_MERCENARY_RESCUED,
            VOX_MERCENARY_FREED,
            VOX_KOSOYGEN_FREED,
            VOX_FLARE_DETECTED,
            VOX_COMMANDO_RESCUED,
            VOX_COMMANDO_FREED,
            VOX_BUILDING_IN_PROGRESS,
            VOX_ATOM_PREPPING,
            VOX_ALLIED_SELECTED,
            VOX_ABOMB_PREPPING,
            VOX_ATOM_LAUNCHED,
            VOX_ALLIED_FORCES_FALLEN,
            VOX_ABOMB_AVAILABLE,
            VOX_ALLIED_REINFORCEMENTS,
            VOX_SAVE1,
            VOX_LOAD1,
        };

        public enum SpecialWeaponType
        {
            SPC_NONE = -1,
            SPC_SONAR_PULSE,            // Momentarily reveals submarines.
            SPC_NUCLEAR_BOMB,           //	Tactical nuclear weapon.
            SPC_CHRONOSPHERE,           // Paradox device, for teleportation
            SPC_PARA_BOMB,              // Parachute bomb delivery.
            SPC_PARA_INFANTRY,      // Parachute reinforcement delivery.
            SPC_SPY_MISSION,            // Spy plane to take photo recon mission.
            SPC_IRON_CURTAIN,           // Bestow invulnerability on a unit/building
            SPC_GPS,						// give allies free unjammable radar.
        };

        public enum QuarryType
        {
            QUARRY_NONE,
            QUARRY_ANYTHING,                    // Attack any enemy (same as "hunt").
            QUARRY_BUILDINGS,                   // Attack buildings (in general).
            QUARRY_HARVESTERS,              // Attack harvesters or refineries.
            QUARRY_INFANTRY,                    // Attack infantry.
            QUARRY_VEHICLES,                    // Attack combat vehicles.
            QUARRY_VESSELS,                 // Attach ships.
            QUARRY_FACTORIES,                   // Attack factories (all types).
            QUARRY_DEFENSE,                 // Attack base defense buildings.
            QUARRY_THREAT,                      // Attack enemies near friendly base.
            QUARRY_POWER,                       // Attack power facilities.
            QUARRY_FAKES,						// Prefer to attack fake buildings.
        };

        public enum VQType
        {
            VQ_NONE = -1,
            VQ_AAGUN,
            VQ_MIG,
            VQ_SFROZEN,
            VQ_AIRFIELD,
            VQ_BATTLE,
            VQ_BMAP,
            VQ_BOMBRUN,
            VQ_DPTHCHRG,
            VQ_GRVESTNE,
            VQ_MONTPASS,
            VQ_MTNKFACT,
            VQ_CRONTEST,
            VQ_OILDRUM,
            VQ_ALLYEND,
            VQ_RADRRAID,
            VQ_SHIPYARD,
            VQ_SHORBOMB,
            VQ_SITDUCK,
            VQ_SLNTSRVC,
            VQ_SNOWBASE,
            VQ_EXECUTE,
            VQ_TITLE,               // Low res.
            VQ_NUKESTOK,
            VQ_V2ROCKET,
            VQ_SEARCH,
            VQ_BINOC,
            VQ_ELEVATOR,
            VQ_FROZEN,
            VQ_MCV,
            VQ_SHIPSINK,
            VQ_SOVMCV,
            VQ_TRINITY,
            VQ_ALLYMORF,
            VQ_APCESCPE,
            VQ_BRDGTILT,
            VQ_CRONFAIL,
            VQ_STRAFE,
            VQ_DESTROYR,
            VQ_DOUBLE,
            VQ_FLARE,
            VQ_SNSTRAFE,
            VQ_LANDING,
            VQ_ONTHPRWL,
            VQ_OVERRUN,
            VQ_SNOWBOMB,
            VQ_SOVCEMET,
            VQ_TAKE_OFF,
            VQ_TESLA,
            VQ_SOVIET8,
            VQ_SPOTTER,
            VQ_SCENE1,
            VQ_SCENE2,
            VQ_SCENE4,
            VQ_SOVFINAL,
            VQ_ASSESS,
            VQ_SOVIET10,
            VQ_DUD,
            VQ_MCV_LAND,
            VQ_MCVBRDGE,
            VQ_PERISCOP,
            VQ_SHORBOM1,
            VQ_SHORBOM2,
            VQ_SOVBATL,
            VQ_SOVTSTAR,
            VQ_AFTRMATH,
            VQ_SOVIET11,
            VQ_MASASSLT,
            VQ_REDINTRO,        // High res
            VQ_SOVIET1,
            VQ_SOVIET2,
            VQ_SOVIET3,
            VQ_SOVIET4,
            VQ_SOVIET5,
            VQ_SOVIET6,
            VQ_SOVIET7,
            VQ_INTRO_MOVIE,
            VQ_AVERTED,
            VQ_COUNTDWN,
            VQ_MOVINGIN,
            VQ_ALLIED10,
            VQ_ALLIED12,
            VQ_ALLIED5,
            VQ_ALLIED6,
            VQ_ALLIED8,
            VQ_TANYA1,
            VQ_TANYA2,
            VQ_ALLY10B,
            VQ_ALLY11,
            VQ_ALLY14,
            VQ_ALLY9,
            VQ_SPY,
            VQ_TOOFAR,
            VQ_SOVIET12,
            VQ_SOVIET13,
            VQ_SOVIET9,
            VQ_BEACHEAD,
            VQ_SOVIET14,
            VQ_SIZZLE,
            VQ_SIZZLE2,
            VQ_ANTEND,
            VQ_ANTINTRO,
            VQ_RETALIATION_ALLIED1,
            VQ_RETALIATION_ALLIED2,
            VQ_RETALIATION_ALLIED3,
            VQ_RETALIATION_ALLIED4,
            VQ_RETALIATION_ALLIED5,
            VQ_RETALIATION_ALLIED6,
            VQ_RETALIATION_ALLIED7,
            VQ_RETALIATION_ALLIED8,
            VQ_RETALIATION_ALLIED9,
            VQ_RETALIATION_ALLIED10,
            VQ_RETALIATION_SOVIET1,
            VQ_RETALIATION_SOVIET2,
            VQ_RETALIATION_SOVIET3,
            VQ_RETALIATION_SOVIET4,
            VQ_RETALIATION_SOVIET5,
            VQ_RETALIATION_SOVIET6,
            VQ_RETALIATION_SOVIET7,
            VQ_RETALIATION_SOVIET8,
            VQ_RETALIATION_SOVIET9,
            VQ_RETALIATION_SOVIET10,
            VQ_RETALIATION_WINA,
            VQ_RETALIATION_WINS,
            VQ_RETALIATION_ANTS,
        };
    }
}
