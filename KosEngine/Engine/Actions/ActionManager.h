/********************************************************************/
/*!
\file      ActionManager.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Nov 12, 2024
\brief     This header file declares and defines the ActionManager class, it's functions and it's constructor and destructor
		   -m_DoAction
		   -m_Undo
		   -m_Redo
		   -m_Push
		   -m_CheckUndo
		   -m_CheckRedo
		   -m_GetManagerInstance

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include <stack>
#include "ModifyAction.h"

namespace actions {
	/******************************************************************/
	/*!
		\brief     This class holds 2 stacks for redoing and undoing actions done in the editor
				   This class is a singleton
				   This class has 7 functions: doAction, undo, redo, push, checkUndo, checkRedo, 
	*/
	/******************************************************************/
	class ActionManager {
		std::stack<Action*> m_undoStack;
		std::stack<Action*> m_redoStack;

	public:
		void m_DoAction(Action* inAction) {
			inAction->m_RedoAction();
			m_undoStack.push(inAction);
			while (!m_redoStack.empty()) {
				delete m_redoStack.top();
				m_redoStack.pop();
			}
		}

		void m_Undo() {
			if (!m_undoStack.empty()) {
				Action* previousAction = m_undoStack.top();
				previousAction->m_UndoAction();
				m_redoStack.push(previousAction);
				m_undoStack.pop();
			}
		}

		void m_Redo() {
			if (!m_redoStack.empty()) {
				Action* latestAction = m_redoStack.top();
				latestAction->m_RedoAction();
				m_undoStack.push(latestAction);
				m_redoStack.pop();
			}
		}

		void m_Push(Action* inAction) {
			m_undoStack.push(inAction);
			while (!m_redoStack.empty()) {
				delete m_redoStack.top();
				m_redoStack.pop();
			}
		}
		
		bool CheckUndo() {
			return m_undoStack.empty();
		}

		bool CheckRedo() {
			return m_redoStack.empty();
		}

		~ActionManager() {
			while (!m_undoStack.empty()) {
				delete m_undoStack.top();
				m_undoStack.pop();
			}

			while (!m_redoStack.empty()) {
				delete m_redoStack.top();
				m_redoStack.pop();
			}
		}

		static ActionManager* m_GetManagerInstance();

	private:
		static std::unique_ptr<ActionManager> m_instance;
	};
}