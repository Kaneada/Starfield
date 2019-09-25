#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <string.h>
#include "Timer.h"


#define RGBA_WHITE SDL_Color {255,255,255,255}
#define RGBA_RED   SDL_Color {255,0,0,255}
#define RGBA_BLUE  SDL_Color {0,255,255,255}
#define RGBA_GREEN SDL_Color {0,255,0,255}

enum demoState
{
	DEMO_INIT = 0,
	DEMO_FADE_LOGO = 1,
	DEMO_FADE_TITLE = 2,
	DEMO_START_FIELD = 3,
	DEMO_END = 4

};

enum setStarColor
{
	STAR_WHITE = 0,
	STAR_RED   = 1,
	STAR_BLUE  = 2,
	STAR_GREEN = 3,

};

#define FRAME_RATE 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define NUM_STARS 2000



struct Logo
{
	SDL_Texture* mLogoTexture;
	SDL_Surface* mLogoSurface;
	SDL_Rect src;
	SDL_Rect dst;
	SDL_Rect trn;
	bool mColorKey;

	float currentAlpha;

	Logo() {};
	Logo(std::string path, SDL_Renderer* renderer, bool colorKey)
	{
		mColorKey = colorKey;
		mLogoSurface = IMG_Load(path.c_str());

		if(colorKey)
		{
			SDL_SetColorKey(mLogoSurface, SDL_TRUE, SDL_MapRGB(mLogoSurface->format, 255, 255, 255));
		
		}
		
		mLogoTexture = SDL_CreateTextureFromSurface(renderer, mLogoSurface);

		if(mLogoTexture == nullptr)
		{
			std::cout << "Failed to load img: " << path << std::endl;
		
		}

		src.x = 0;
		src.y = 0;
		
		SDL_QueryTexture(mLogoTexture, NULL, NULL, &src.w, &src.h);
		
		dst.x = 0;
		dst.y = 0;
		dst.w = 0;
		dst.h = 0;

		currentAlpha = 0.0f;

		SDL_SetTextureBlendMode(mLogoTexture, SDL_BLENDMODE_BLEND);
	
	}

	int getDstX() { return dst.x; }
	int getDstY() { return dst.y; }
	int getDstW() { return dst.w; }
	int getDstH() { return dst.h; }

	void setDstWidth(int width)
	{
		dst.w = width;
	}

	void setDstHeight(int height)
	{
		dst.h = height;
	}

	void setDstX(int x) 
	{
		dst.x = x;
	}
	void setDsty(int y) 
	{
		dst.y = y;
	}

	void setDstRect(int x, int y, int w, int h)
	{
		dst.x = x;
		dst.y = y;
		dst.w = w;
		dst.h = h;

	
	}

	void setDstToSrcW()
	{
		dst.w = src.w;
	
	}

	void setDstToSrcWH()
	{
		dst.w = src.w;
		dst.h = src.h;
	
	}

	void setSrcH()
	{
		src.h = 0;
	
	}

	void fadeTextureAlpha(Uint8 alpha, float deltaTime)
	{
		if((Uint8)currentAlpha < SDL_ALPHA_OPAQUE)
		{
			currentAlpha += alpha * deltaTime;
			SDL_SetTextureAlphaMod(mLogoTexture, floor(currentAlpha));
		
		}else if((Uint8)currentAlpha >= SDL_ALPHA_OPAQUE)
		{
			currentAlpha = (float)SDL_ALPHA_OPAQUE;
		
		}
		
		
	
	}

	void fadeOutTextureAlpha(Uint8 alpha, float deltaTime)
	{
		if ((Uint8)currentAlpha > SDL_ALPHA_TRANSPARENT)
		{
			currentAlpha -= alpha * deltaTime;
			SDL_SetTextureAlphaMod(mLogoTexture, floor(currentAlpha));

		}
		else if ((Uint8)currentAlpha == SDL_ALPHA_TRANSPARENT)
		{
			currentAlpha = (float)SDL_ALPHA_TRANSPARENT;

		}

	
	}

	Uint8 getAlpha() { return (Uint8)currentAlpha; }

	void setTextureAlpha(Uint8 alpha)
	{
		SDL_SetTextureAlphaMod(mLogoTexture, alpha);
	
	}

	//stretches X, Y, W, H Frame by Frame
	void TransFormExplode(int velocity)
	{
		SDL_QueryTexture(mLogoTexture, NULL, NULL, &trn.w, &trn.h);
		
		
		if(src.h < trn.h)
		{
			src.h += velocity;
			dst.h += velocity;
			
		
		}
		
			
		
	
	}

	void render(SDL_Renderer* renderer)
	{
		
		SDL_RenderCopy(renderer, mLogoTexture, &src, &dst);
	
	}

	

};

struct Star
{

	SDL_Point mStar;
	SDL_Color mStarColor;
	Uint32 mStarVelocity;
	setStarColor mCurrentColor;

	Star() {};

	Star(int x, int y, int starColor)
	{
		mStar.x = x;
		mStar.y = y;
		//mStarColor = starColor;

		

		if (starColor == 0)
		{
			mStarColor = RGBA_WHITE;
			mStarVelocity = 2;
			mCurrentColor = STAR_WHITE;
		
		}
		else if (starColor == 1)
		{
			mStarColor = RGBA_RED;
			mStarVelocity = 1;
			mCurrentColor = STAR_RED;
		
		}
		else if (starColor == 2)
		{
			mStarColor = RGBA_GREEN;
			mStarVelocity = 5;
			mCurrentColor = STAR_GREEN;
		
		}
		else if (starColor == 3)
		{
			mStarColor = RGBA_BLUE;
			mStarVelocity = 7;
			mCurrentColor = STAR_BLUE;
		
		}

	

	}

	void Render(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, mStarColor.r, mStarColor.g, mStarColor.b, mStarColor.a);
		SDL_RenderDrawPoint(renderer, mStar.x, mStar.y);
	
	}

	void Update()
	{
		if(mStar.x > 0)
		{
			mStar.x -= mStarVelocity;
		
		}
		else if (mStar.x <= 0)
		{
			mStar.x = 1280;
		
		}

		
	}


};

struct StarField
{
	Star starArray[NUM_STARS];

	StarField()
	{
		for (int i = 0; i < NUM_STARS; i++)
		{
			starArray[i] = Star(rand() % 1280, rand() % 720, rand() % 4);

		}

	}

};

struct colorWarmth
{
	SDL_Color warmthState;
	colorWarmth()
	{
		warmthState.r = 0;
		warmthState.g = 0;
		warmthState.b = 0;
		warmthState.a = 255;

	}


	void updateWarm(Uint8 intensity)
	{
		if(warmthState.r < 255)
		{
			warmthState.r += intensity;
			warmthState.g += intensity;
			warmthState.b += intensity;
		
		}else if (warmthState.r >= 255)
		{
			warmthState.r = 255;
			warmthState.g = 255;
			warmthState.b = 255;
		
		}
	}

	void updateCool(Uint8 intensity)
	{
		if (warmthState.r > 0)
		{
			warmthState.r -= intensity;
			warmthState.g -= intensity;
			warmthState.b -= intensity;
		}
		
	
	}

	Uint8 getWarmthState() { return warmthState.r;}

};

int main (int agrc, char** argv)
{
	
	demoState demo;
	colorWarmth warmth;

	//SDL INIT
	SDL_Init(SDL_INIT_EVERYTHING);

	//Initialize Mixer First for some strange reason
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Setup Random Seed for Star
	srand(time(0));

	//Setup Window and Renderer
	SDL_Window* window = SDL_CreateWindow("Star Field", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//IMG Init
	IMG_Init(IMG_INIT_PNG);

	//music stuff
	Mix_Music* song = Mix_LoadMUS("msc/04 Departure for Space.flac");
	if (song == NULL)
	{
		std::cout << "Something went wrong" << Mix_GetError() <<  std::endl;
		
	
	}

	//sound effects
	Mix_Chunk* ding = Mix_LoadWAV("msc/01 Konami Sound.flac");
	bool dingIsDone = false;

	//Event Var
	SDL_Event ev;

	//Delcare FPS Timer
	Timer fpsTimer;

	//Set Loop
	bool isRunning = true;

	//create logo
	Logo logo = Logo("gfx/G3logo.png", renderer, true);
	logo.setDstToSrcWH();
	logo.setDsty(720 - logo.getDstH());
	logo.setTextureAlpha(SDL_ALPHA_TRANSPARENT);

	//Setup Konami
	Logo konami = Logo("gfx/Konami.png", renderer, false);
	
	konami.setDsty(150);
	konami.setDstX(1280 / 3 + 50);
	konami.setSrcH();
	konami.setDstToSrcW();
	konami.setTextureAlpha(SDL_ALPHA_TRANSPARENT);

	StarField field;
	Uint8 currentFrame = 0;

	Mix_Volume(-1, 32);
	

	bool Transform = false;

	demo = DEMO_FADE_LOGO;
	//Run Loop
	while (isRunning)
	{
		while (demo == DEMO_FADE_LOGO)
		{
			fpsTimer.Update();
			while (SDL_PollEvent(&ev))
			{
				if (ev.type == SDL_QUIT)
				{
					isRunning = false;
					demo = DEMO_END;
					
				}

			}
			if(fpsTimer.getDeltaTime() >= 1.0f / FRAME_RATE)
			{
				
				
				//update
				konami.fadeTextureAlpha(95, fpsTimer.getDeltaTime());
				konami.TransFormExplode(5);
				warmth.updateWarm(5);
				

				if(dingIsDone == false)
				{
					Mix_PlayChannel(-1, ding, 0);
					dingIsDone = true;
				
				}

				
				


				//render
				SDL_SetRenderDrawColor(renderer, warmth.warmthState.r, warmth.warmthState.g, warmth.warmthState.b, 255);
				SDL_RenderClear(renderer);

				konami.render(renderer);

				SDL_RenderPresent(renderer);

				fpsTimer.Reset();
				
				
				if (konami.getAlpha() == 255 && warmth.getWarmthState() == 255 && dingIsDone == true)
				{
					demo = DEMO_FADE_TITLE;
					fpsTimer.Reset();
					//std::cout << "I got here!" << std::endl;

				}
			
			}
			
			

			
		}

		while(demo == DEMO_FADE_TITLE)
		{
			
			fpsTimer.Update();
			while (SDL_PollEvent(&ev))
			{
				if (ev.type == SDL_QUIT)
				{
					isRunning = false;
					demo = DEMO_END;

				}

			}

			if (fpsTimer.getDeltaTime() >= 1.0f / FRAME_RATE)
			{
				//update
				logo.fadeTextureAlpha(99, fpsTimer.getDeltaTime());


				//render

				SDL_SetRenderDrawColor(renderer, warmth.warmthState.r, warmth.warmthState.g, warmth.warmthState.b, 255);
				SDL_RenderClear(renderer);

				konami.render(renderer);
				logo.render(renderer);

				SDL_RenderPresent(renderer);

				fpsTimer.Reset();

				if(logo.getAlpha() == 255)
				{
					demo = DEMO_START_FIELD;
				
				}

			}
			
		
		}

		while (demo == DEMO_START_FIELD)
		{
			fpsTimer.Update();
			while (SDL_PollEvent(&ev))
			{
				if (ev.type == SDL_QUIT)
				{
					isRunning = false;
					demo = DEMO_END;

				}

			}

			if (fpsTimer.getDeltaTime() >= 1.0f / FRAME_RATE)
			{
				//update
				for(int i = 0; i < NUM_STARS; i++)
				{
					field.starArray[i].Update();
				
				}
				konami.fadeOutTextureAlpha(95, fpsTimer.getDeltaTime());
				warmth.updateCool(5);


				//render

				SDL_SetRenderDrawColor(renderer, warmth.warmthState.r, warmth.warmthState.g, warmth.warmthState.b, 255);
				SDL_RenderClear(renderer);

				for(int i = 0; i < NUM_STARS; i++)
				{
					field.starArray[i].Render(renderer);
				
				}
				konami.render(renderer);
				logo.render(renderer);

				SDL_RenderPresent(renderer);

				fpsTimer.Reset();
			}
		}

			
	
	}

	return 0;

}