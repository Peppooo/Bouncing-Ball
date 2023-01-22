#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>

using namespace std;

class vec2 {
public:
	double x,y;
};

SDL_Window* window;
SDL_Renderer* renderer;

double rads(double degrees) {
	return (degrees * (M_PI / 180));
}

vec2 angle(double degrees) {
	return {(sin(rads(degrees))),cos(rads(degrees))};
}


void drawCircle(SDL_Renderer* renderer,vec2 center,int r,double approx) {
	for(double i = 0; i < 180; i += approx) {
		vec2 v = angle(i);
		SDL_RenderDrawLineF(renderer,center.x + v.x * r,center.y + v.y * r,center.x + (-v.x * r),center.y + (-v.y * r));
	};
}

int rrand(int min,int max) {
	return min + (std::rand() % (max - min + 1));
}

int main() {
	int seed = time(0);
	srand(seed);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
	SDL_CreateWindowAndRenderer(1920,1080,SDL_WINDOW_BORDERLESS,&window,&renderer);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"2");
	SDL_RenderSetVSync(renderer,1);
	vec2 pos = {1920/2,1080/2};
	vec2 size = {15,15};
	double speed = 5;
	double angle_o = 45;
	cout << "seed: " << time(0) << endl;
	//SDL_SetRenderDrawColor(renderer,25,25,25,255);
	//SDL_RenderClear(renderer);
	while(1) {
		SDL_PollEvent(nullptr);
		SDL_SetRenderDrawColor(renderer,25,25,25,255);
		SDL_RenderClear(renderer);
		if((pos.x + (size.x*2)) >= 1920 || (pos.x - (size.x * 2)) <= 0 || (pos.y + (size.y * 2)) >= 1080 || (pos.y - (size.y * 2)) <= 0) {
			//cout << "bounce" << endl;
			angle_o += 90;
			//angle_o += rrand(80,120);
			//speed = rrand(1,10)/10.0;
			//SDL_SetRenderDrawColor(renderer,255,255,255,255);
			//SDL_RenderClear(renderer);
		}
		//SDL_SetRenderDrawColor(renderer,rrand(0,255),rrand(0,255),rrand(0,255),255);
		SDL_SetRenderDrawColor(renderer,0xFF,0xFD,0xD0,195);
		vec2 v = angle(angle_o);
		pos.x+=v.x*speed;
		pos.y+=v.y*speed;
		drawCircle(renderer,pos,size.x,1);
		SDL_RenderPresent(renderer);
		cout << "x: " << pos.x << ", y: " << pos.y << ", angle: " << rads(angle_o) << "\r";
	};
}