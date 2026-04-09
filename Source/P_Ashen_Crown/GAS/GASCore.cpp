// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCore.h"

namespace AC_PlayerState
{
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Rolling, "Player.State.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_StaminaRegenDelay, "Player.State.StaminaRegenDelay");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Healing, "Player.State.Healing");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Hit, "Player.State.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Death, "Player.State.Death");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Skill_Active01, "Player.Skill.Active01");

	UE_DEFINE_GAMEPLAY_TAG(Player_Combat, "Player.Combat");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combat_Attacking1, "Player.Combat.Attacking1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combat_Attacking2, "Player.Combat.Attacking2");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combat_Attacking3, "Player.Combat.Attacking3");
	UE_DEFINE_GAMEPLAY_TAG(Player_Combat_Attacking4, "Player.Combat.Attacking4");

	UE_DEFINE_GAMEPLAY_TAG(Data_Rolling_Cost, "Data.Rolling.StaminaCost");
	UE_DEFINE_GAMEPLAY_TAG(Data_Attacking_Cost, "Data.Attacking.StaminaCost");
	UE_DEFINE_GAMEPLAY_TAG(Data_Skill_01Cost, "Data.Skill.01Cost");
	UE_DEFINE_GAMEPLAY_TAG(Data_LoadData_HP, "Data_LoadData_HP");
	UE_DEFINE_GAMEPLAY_TAG(Data_Skill_01Cooldown, "Data.Skill.01Cooldown");
	UE_DEFINE_GAMEPLAY_TAG(Data_NormalAttack_Damage, "Data.NormalAttack.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Data_Skill01_Damage, "Data.Skill01.Damage");

	UE_DEFINE_GAMEPLAY_TAG(Event_ComboWindow_Open, "Event.ComboWindow.Open");
	UE_DEFINE_GAMEPLAY_TAG(Event_ComboWindow_2, "Event.ComboWindow.2");
	UE_DEFINE_GAMEPLAY_TAG(Event_ComboWindow_3, "Event.ComboWindow.3");
	UE_DEFINE_GAMEPLAY_TAG(Event_AttackInput_Block, "Event.AttackInput.Block");
	UE_DEFINE_GAMEPLAY_TAG(Event_ComboWindow_Close, "Event.ComboWindow.Close");
	
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Attack_BP_NormalAttackCue, "GameplayCue.Player.Attack.BP_NormalAttackCue");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Skill, "GameplayCue.Player.Skill");
}

namespace AC_NPCEmotion
{
	UE_DEFINE_GAMEPLAY_TAG(NPC_Emotion, "NPC.Emotion");
	UE_DEFINE_GAMEPLAY_TAG(NPC_Emotion_None, "NPC.Emotion.None");

	UE_DEFINE_GAMEPLAY_TAG(NPC_Emotion_Happy, "NPC.Emotion.Happy");
	UE_DEFINE_GAMEPLAY_TAG(NPC_Emotion_Scared, "NPC.Emotion.Scared");
	UE_DEFINE_GAMEPLAY_TAG(NPC_Emotion_Disappointed, "NPC.Emotion.Disappointed");
	UE_DEFINE_GAMEPLAY_TAG(NPC_Emotion_Tired, "NPC.Emotion.Tired");
}