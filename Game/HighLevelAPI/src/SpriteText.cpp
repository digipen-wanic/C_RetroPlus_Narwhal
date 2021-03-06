//------------------------------------------------------------------------------
//
// File Name:	SpriteText.cpp
// Author(s):	Bar Ben-zvi
// Project:		Game Project 1
// Course:		WANIC VGP2 2018-2019
//
// Brief: Header file for the spritext class, which displays a given string.
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteText.h"
#include "Transform.h"

SpriteText::SpriteText()
	: text(""), alignment(CENTER)
{

}


Component* SpriteText::Clone() const
{
	return new SpriteText(*this);
}

void SpriteText::Draw()
{
	Vector2D currOffset(0, 0);
	Vector2D scale = transform->GetScale();

	float xOffset = 0;

	size_t textCount = text.length();

	if (alignment == CENTER)
	{
		xOffset = -scale.x * textCount / 2;
	}
	else if (alignment == RIGHT)
	{
		xOffset = -scale.x * textCount;
	}

	currOffset.x = xOffset;

	for (size_t i = 0; i < textCount; ++i)
	{
		int asciiVal = text[i];

		//capital letters
		if (asciiVal < 91 && asciiVal > 64)
		{
			SetFrame(asciiVal - 65);
			Sprite::Draw(currOffset);

			
		}
		//numbers
		else if (asciiVal < 58 && asciiVal > 47)
		{
			SetFrame(asciiVal - 22);
			Sprite::Draw(currOffset);
		}
		//period
		else if (asciiVal == 46)
		{
			SetFrame(35);
			Sprite::Draw(currOffset);
		}
		else if (asciiVal == 44)
		{
			SetFrame(36);
			Sprite::Draw(currOffset);
		}
		currOffset.x += scale.x;
	}

}

void SpriteText::SetString(std::string newString)
{
	text = newString;
}

void SpriteText::SetAlignment(TextAlignment newAlignment)
{
	alignment = newAlignment;
}
