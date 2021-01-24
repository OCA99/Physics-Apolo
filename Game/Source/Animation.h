#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 100

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}


	void Reset()
	{
		currentFrame = 0;
		loopCount = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void GenerateAnimation(const SDL_Rect& rect, int rows, int collumns, int marginX, int marginY)
	{
		int frameWidth = rect.w;
		int frameHeight = rect.h;
		for (int i = 0; i <= rows; ++i)
		{
			for (int j = 0; j <= collumns; ++j)
			{
				SDL_Rect frame;

				if (j == 0)
				{
					frame.x = rect.x;
					frame.y = rect.y;
				}
				else
				{
					frame.x = rect.x + j * frameWidth + marginX;
					frame.y = (rect.y + i * frameHeight) + marginY;
				}

				frame.w = frameWidth;
				frame.h = frameHeight;
				PushBack(frame);
			}
		}
	}

};

#endif