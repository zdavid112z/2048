#pragma once

#include "pch.h"
#include "math\math.h"
#include <SDL.h>
#include <SDL_image.h>

class Renderer;

enum BlendMode
{
	BLENDMODE_NONE = 0x00000000,     /**< no blending
										 dstRGBA = srcRGBA */
	BLENDMODE_BLEND = 0x00000001,    /**< alpha blending
										 dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
									 dstA = srcA + (dstA * (1-srcA)) */
	BLENDMODE_ADD = 0x00000002,      /**< additive blending
										 dstRGB = (srcRGB * srcA) + dstRGB
										 dstA = dstA */
	BLENDMODE_MOD = 0x00000004       /**< color modulate
										 dstRGB = srcRGB * dstRGB
										 dstA = dstA */
};

class Texture
{
public:
	Texture(Renderer*);
	Texture(Renderer*, const string&);
	~Texture();

	void CreateFromSurface(SDL_Surface*);
	bool LoadImage(const string&);
	void CreateBlank(int w, int h, uint format, int access);
	void Update(const SDL_Rect* area, byte* pixels, uint pitch);
	void FreeTexture();
	void SetColor(Vector4i);
	void SetAlpha(byte);
	void SetBlendMode(BlendMode);

	Renderer* renderer;
	SDL_Texture* texture;
	int w, h;
};
