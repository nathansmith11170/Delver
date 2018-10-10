#include <SDL.h>
#include <SDL_image.h>
#include <string>
#ifndef LTEXTURE
#define LTEXTURE
class LTexture {
public:
	LTexture(SDL_Renderer* rendPtr) {
		gRenderer = rendPtr;
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}

	~LTexture() {
		free();
	}

	bool loadFromFile(std::string path) {
		//Reset the texture
		free();

		//Set up temporary texture
		SDL_Texture *newTexture = NULL;

		//Load image as surface
		SDL_Surface *loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) {
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else {
			//Create texture from surface
			newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
			if (newTexture == NULL) {
				printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
			}
			else {
				mWidth = loadedSurface->w;
				mHeight = loadedSurface->h;
			}
			SDL_FreeSurface(loadedSurface);
		}
		mTexture = newTexture;
		return mTexture != NULL;
	}

	void free() {
		if (mTexture != NULL) {
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	void render(int x, int y, SDL_Rect *clip = NULL) {
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };

		//Set clip rendering dimensions
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		if (SDL_RenderCopy(gRenderer, mTexture, clip, NULL) < 0) {
			printf("Render failed. SDL_Error: %s\n", SDL_GetError());
		}
	}

	int getWidth() {
		return mWidth;
	}

	int getHeight() {
		return mHeight;
	}

private:
	SDL_Texture *mTexture;
	SDL_Renderer *gRenderer;

	int mWidth;
	int mHeight;
};
#endif
