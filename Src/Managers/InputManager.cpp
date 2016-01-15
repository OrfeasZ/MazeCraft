#include "InputManager.h"

#include <Application.h>

using namespace Managers;

InputManager* InputManager::m_Instance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (!m_Instance)
		m_Instance = new InputManager();

	return m_Instance;
}

void InputManager::DestroyInstance()
{
	if (!m_Instance)
		return;

	delete m_Instance;
	m_Instance = nullptr;
}

//////////////////////////////////////////////////////////////////////////

InputManager::InputManager() :
	m_Window(nullptr),
	m_MouseX(0.0),
	m_MouseY(0.0),
	m_MouseInside(false)
{

}

InputManager::~InputManager()
{

}

bool InputManager::Init()
{
	m_Window = Application::GetInstance()->GetWindow();
	m_PostUpdateTask = std::bind(&InputManager::PostUpdateTask, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	memset(m_KeyActions, 0x00, sizeof(m_KeyActions));
	memset(m_KeyBuffers, 0x00, sizeof(m_KeyBuffers));
	memset(m_MouseActions, 0x00, sizeof(m_MouseActions));
	memset(m_MouseBuffers, 0x00, sizeof(m_MouseBuffers));

	glfwSetKeyCallback(m_Window, OnKeyStatic);
	glfwSetCharModsCallback(m_Window, OnCharacterStatic);
	glfwSetCursorPosCallback(m_Window, OnCursorStatic);
	glfwSetCursorEnterCallback(m_Window, OnCursorEnterStatic);
	glfwSetMouseButtonCallback(m_Window, OnMouseButtonStatic);
	glfwSetScrollCallback(m_Window, OnScrollStatic);

	return true;
}

void InputManager::PostUpdateTask(void* p_Argument, int32_t p_ContextID, uint32_t p_Index, uint32_t p_Size)
{
	for (int i = 0; i < GLFW_KEY_LAST; ++i)
		m_KeyBuffers[i].Processed = true;

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
		m_MouseBuffers[i].Processed = true;
}

bool InputManager::IsKeyboardKeyPressed(int p_Key) const
{
	return m_KeyActions[p_Key].Action == GLFW_PRESS;
}

bool InputManager::IsKeyboardKeyRepeated(int p_Key) const
{
	return m_KeyActions[p_Key].Action == GLFW_REPEAT;
}

bool InputManager::WentKeyboardKeyDown(int p_Key) const
{
	return m_KeyBuffers[p_Key].Pressed && !m_KeyBuffers[p_Key].Processed;
}

bool InputManager::IsMouseButtonPressed(int p_Button) const
{
	return m_MouseActions[p_Button].Action == GLFW_PRESS;
}

bool InputManager::IsMouseButtonRepeated(int p_Button) const
{
	return m_MouseActions[p_Button].Action == GLFW_REPEAT;
}

bool InputManager::WentMouseButtonDown(int p_Button) const
{
	return m_MouseBuffers[p_Button].Pressed && !m_MouseBuffers[p_Button].Processed;
}

void InputManager::OnKeyStatic(GLFWwindow* p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Modifiers)
{
	GetInstance()->OnKey(p_Window, p_Key, p_ScanCode, p_Action, p_Modifiers);
}

void InputManager::OnCharacterStatic(GLFWwindow* p_Window, unsigned int p_CodePoint, int p_Modifiers)
{
	GetInstance()->OnCharacter(p_Window, p_CodePoint, p_Modifiers);
}

void InputManager::OnCursorStatic(GLFWwindow* p_Window, double p_X, double p_Y)
{
	GetInstance()->OnCursor(p_Window, p_X, p_Y);
}

void InputManager::OnCursorEnterStatic(GLFWwindow* p_Window, int p_Entered)
{
	GetInstance()->OnCursorEnter(p_Window, p_Entered);
}

void InputManager::OnMouseButtonStatic(GLFWwindow* p_Window, int p_Button, int p_Action, int p_Modifiers)
{
	GetInstance()->OnMouseButton(p_Window, p_Button, p_Action, p_Modifiers);
}

void InputManager::OnScrollStatic(GLFWwindow* p_Window, double p_OffsetX, double p_OffsetY)
{
	GetInstance()->OnScroll(p_Window, p_OffsetX, p_OffsetY);
}

void InputManager::OnKey(GLFWwindow* p_Window, int p_Key, int p_ScanCode, int p_Action, int p_Modifiers)
{
	m_KeyActions[p_Key].ScanCode = p_ScanCode;
	m_KeyActions[p_Key].Action = p_Action;
	m_KeyActions[p_Key].Modifiers = p_Modifiers;

	if ((!m_KeyBuffers[p_Key].Pressed && p_Action == GLFW_PRESS) || p_Action == GLFW_RELEASE)
		m_KeyBuffers[p_Key].Processed = false;

	m_KeyBuffers[p_Key].Pressed = p_Action != GLFW_RELEASE;
}

void InputManager::OnCharacter(GLFWwindow* p_Window, unsigned int p_CodePoint, int p_Modifiers)
{
	// TODO: Buffered characters.
}

void InputManager::OnCursor(GLFWwindow* p_Window, double p_X, double p_Y)
{
	m_MouseX = p_X;
	m_MouseY = p_Y;
}

void InputManager::OnCursorEnter(GLFWwindow* p_Window, int p_Entered)
{
	m_MouseInside = p_Entered != 0;
}

void InputManager::OnMouseButton(GLFWwindow* p_Window, int p_Button, int p_Action, int p_Modifiers)
{
	m_MouseActions[p_Button].Action = p_Action;
	m_MouseActions[p_Button].Modifiers = p_Modifiers;

	if ((!m_MouseBuffers[p_Button].Pressed && p_Action == GLFW_PRESS) || p_Action == GLFW_RELEASE)
		m_MouseBuffers[p_Button].Processed = false;

	m_MouseBuffers[p_Button].Pressed = p_Action != GLFW_RELEASE;
}

void InputManager::OnScroll(GLFWwindow* p_Window, double p_OffsetX, double p_OffsetY)
{
	// TODO: Buffered scrolling.
}