#pragma once

#include <Misc/StdAfx.h>

#include "IManager.h"
#include "TaskManager.h"
#include "Input/KeyAction.h"
#include "Input/MouseAction.h"

namespace Managers
{
	class InputManager : public IManager
	{
	protected:
		struct BufferedPress
		{
			bool Pressed;
			bool Processed;
		};

	public:
		static InputManager* GetInstance();
		static void DestroyInstance();

	private:
		static InputManager* m_Instance;

	public:
		InputManager();
		~InputManager();

	public:
		virtual bool Init() override;

	public:
		inline TaskManager::TaskSetFunction_t GetPostUpdateTask() const { return m_PostUpdateTask; }

	protected:
		void PostUpdateTask(void* p_Argument, int32_t p_ContextID, uint32_t p_Index, uint32_t p_Size);

	public:
		bool IsKeyboardKeyPressed(int p_Key) const;
		bool IsKeyboardKeyRepeated(int p_Key) const;
		bool WentKeyboardKeyDown(int p_Key)  const;

		bool IsMouseButtonPressed(int p_Button) const;
		bool IsMouseButtonRepeated(int p_Button) const;
		bool WentMouseButtonDown(int p_Button) const;

	public:
		inline bool IsCursorInFrame() const { return m_MouseInside; }
		inline double GetCursorX() { return m_MouseX; }
		inline double GetCursorY() { return m_MouseY; }

		inline int GetKeyboardKeyScanCode(int p_Key) const { return m_KeyActions[p_Key].ScanCode; }
		inline int GetKeyboardKeyModifiers(int p_Key) const { return m_KeyActions[p_Key].Modifiers; }

		inline int GetMouseButtonModifiers(int p_Button) const { return m_MouseActions[p_Button].Modifiers; }

	protected:
		static void OnKeyStatic(GLFWwindow* p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Modifiers);
		static void OnCharacterStatic(GLFWwindow* p_Window, unsigned int p_CodePoint, int p_Modifiers);
		static void OnCursorStatic(GLFWwindow* p_Window, double p_X, double p_Y);
		static void OnCursorEnterStatic(GLFWwindow* p_Window, int p_Entered);
		static void OnMouseButtonStatic(GLFWwindow* p_Window, int p_Button, int p_Action, int p_Modifiers);
		static void OnScrollStatic(GLFWwindow* p_Window, double p_OffsetX, double p_OffsetY);

	protected:
		void OnKey(GLFWwindow* p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Modifiers);
		void OnCharacter(GLFWwindow* p_Window, unsigned int p_CodePoint, int p_Modifiers);
		void OnCursor(GLFWwindow* p_Window, double p_X, double p_Y);
		void OnCursorEnter(GLFWwindow* p_Window, int p_Entered);
		void OnMouseButton(GLFWwindow* p_Window, int p_Button, int p_Action, int p_Modifiers);
		void OnScroll(GLFWwindow* p_Window, double p_OffsetX, double p_OffsetY);

	protected:
		GLFWwindow* m_Window;
		double m_MouseX;
		double m_MouseY;
		bool m_MouseInside;

	protected:
		Input::KeyAction m_KeyActions[GLFW_KEY_LAST];
		BufferedPress m_KeyBuffers[GLFW_KEY_LAST];
		Input::MouseAction m_MouseActions[GLFW_MOUSE_BUTTON_LAST];
		BufferedPress m_MouseBuffers[GLFW_MOUSE_BUTTON_LAST];

	protected:
		// Tasks
		TaskManager::TaskSetFunction_t m_PostUpdateTask;
	};
}