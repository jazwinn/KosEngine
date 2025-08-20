/********************************************************************/
/*!
\file      EventsDragFloat.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Nov 12, 2024
\brief     This header file declares and defines Member and Comp enums along with the drag

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "../Config/pch.h"

namespace dragfloat {
	/******************************************************************/
	/*!
		\brief     This is an enum for which member of which component is being edited
	*/
	/******************************************************************/
	enum class Member{
		POS,
		ROT,
		SCALE,
		SPRITELAYER,
		SPRITECLR,
		SPRITEALPHA,
		COLLOFFSET,
		COLLRAD,
		COLLSIZE,
		RIGIDVELO,
		RIGIDACCEL,
		RIGIDROT,
		RIGIDANGVELO,
		RIGIDANGACCEL,
		RIGIDMASS,
		RIGIDINVMASS,
		RIGIDLINDAMP,
		RIGIDANGDAMP,
		RIGIDFORCE,
		RIGIDTORQUE,
		ENEMYTAG,
		ENEMYTYPE,
		ENEMYBEHAVE,
		TEXTSIZE,
		TEXTCLR,
		TEXTLAYER,
		ANIMFRAMENUM,
		ANIMFPS,
		ANIMFT,
		ANIMSC,
		CAMLEFT,
		CAMRIGHT,
		CAMUP,
		CAMDOWN,
		CAMAR,
		BUTTONPOS,
		BUTTONSCALE,
		GRIDANCHOR,
		GRIDROW,
		GRIDCOL,
		GRIDKEY,
		LIGHTRAD,
		LIGHTCLR,
		LIGHTOFFSET,
		LIGHTSCALE,
		LIGHTINTENSE,
		LIGHTROT,
		LIGHTTYPE,
		PARTICLENUM,
		PARTICLELIFE,
		PARTICLEVELO,
		PARTICLEACCEL,
		PARTICLECLR,
		PARTICLECONEROT,
		PARTICLECONANG,
		PARTICLERAND,
		PARTICLESTRIP,
		PARTICLEFRAMENUM,
		PARTICLELAYER,
		PARTICLEFRICTION,
		PARTICLEFPS,
		PATHFINDINGSTART,
		PATHFINDINGEND,
		PATHFINDINGKEY,
		VIDEOLAYER,
		TILEMAPLAYER,
		TILEMAPCLR,
		TILEMAPALPHA,
		TILEMAPIDX,
		TILEMAPROW,
		TILEMAPCOL,
		TILEMAPPICROW,
		TILEMAPPICCOL,
		NONE
	};

	/******************************************************************/
	/*!
		\brief     This is an enum for which component is being edited
	*/
	/******************************************************************/
	enum class Comp {
		TRANSFORM,
		ANIM,
		TEXT,
		ENEMY,
		RIGID,
		COLL,
		SPRITE,
		CAMERA,
		BUTTON,
		GRID,
		LIGHT,
		PARTICLE,
		PATHFINDING,
		VIDEO,
		TILEMAP,
		NONE
	};

	/******************************************************************/
	/*!
		\brief     This is a singleton that helps to check when the editing of components is started and ended
				   This class helps to check which component is being edited and which member
	*/
	/******************************************************************/
	class DragFloatCheck {
		bool m_clicked = false;
		Comp m_lastComp;
		Member m_lastMember;
		bool m_calledBefore = false;
		static std::unique_ptr<DragFloatCheck> m_instance;
	public:

		bool m_Click(Comp inComp, Member inMemb) {
			if (!m_calledBefore) {
				m_calledBefore = true;
				m_lastComp = inComp;
				m_lastMember = inMemb;
				m_clicked = true;
			}
			else if(inComp != m_lastComp){
				m_calledBefore = true;
				m_lastComp = inComp;
				m_lastMember = inMemb;
				m_clicked = true;
			}
			else if (inMemb != m_lastMember) {
				m_calledBefore = true;
				m_lastComp = inComp;
				m_lastMember = inMemb;
				m_clicked = true;
			}
			else if(m_calledBefore && m_clicked){
				m_lastComp = Comp::NONE;
				m_lastMember = Member::NONE;
				m_calledBefore = false;
				m_clicked = false;
				return true;
			}
			return false;

		}

		bool m_GetCalledBefore() { return m_calledBefore; }
		Member m_GetPrevMem() { return m_lastMember; }

		static DragFloatCheck* m_GetInstance();
	};
}