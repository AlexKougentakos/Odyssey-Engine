#pragma once
#include <glm/vec2.hpp>

namespace ody
{
	class XBox360Controller final
	{
	private:
		class XBox360ControllerImpl;
		XBox360ControllerImpl* pImpl{};

		unsigned int m_ControllerIndex{};

	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumbPress = 0x0040,
			RightThumbPress = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000,
			LeftThumbStick
		};

		void Update();

		bool IsDown(ControllerButton button) const;
		bool IsUp(ControllerButton button) const;
		bool IsPressed(ControllerButton button) const;

		glm::vec2 GetLeftThumbStickPos() const;

		unsigned int GetIdx() const { return m_ControllerIndex; }

		explicit XBox360Controller(unsigned int controllerIndex);
		~XBox360Controller();

	};
}
