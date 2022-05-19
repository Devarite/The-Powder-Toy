#include <iostream>
#include "Appearance.h"
#include "graphics/Graphics.h"

namespace ui
{
	Appearance::Appearance():
		texture(NULL),

		VerticalAlign(AlignMiddle),
		HorizontalAlign(AlignCentre),

		BackgroundHover(20, 20, 20),
		BackgroundInactive(0, 0, 0),
		BackgroundActive(200, 200, 200),
		BackgroundDisabled(10, 10, 10),

		TextHover(255, 255, 255),
		TextInactive(255, 255, 255),
		TextActive(0, 0, 0),
		TextDisabled(45, 45, 45),

		BorderHover(200, 200, 200),
		BorderInactive(50, 50, 50),
		BorderActive(100, 100, 100),
		BorderDisabled(50, 50, 50),

		Margin(1, 4),
		Border(1),

		icon(NoIcon)
	{}

	VideoBuffer * Appearance::GetTexture()
	{
		return texture;
	}

	void Appearance::SetTexture(VideoBuffer * texture)
	{
		delete this->texture;
		if(texture)
			this->texture = new VideoBuffer(texture);
		else
			this->texture = NULL;
	}

	Appearance::~Appearance()
	{
		delete texture;
	}

}
