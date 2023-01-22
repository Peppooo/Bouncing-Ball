#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

#define W 1920	
#define H 1080

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


double frames = 0;
double elapsed = 0;
int tAdded = 0;
int elapsedSeconds = 0;
bool endTs = false;
bool inPause = false;

void uptimer() {
	while(!endTs) {
		this_thread::sleep_for(chrono::milliseconds(950));
		tAdded++;
		elapsed += 0.1;
		elapsedSeconds = (int)(tAdded / 10);
	}
}

void fpsUpdater() {
	while(!endTs) {
		cout << "fps: " << frames/elapsed << "         \r";
	}
}

class ball {
public:
	double xRand = rrand((W / 4),W - (W / 4));
	double yRand = rrand((H / 4),H - (H / 4));
	vec2 pos = {W / 2,H / 2};
	vec2 size = {4,4};
	double speed = 4;
	double angle_o = rrand(0,360);
	void update() {
		if((pos.x + (size.x * 2)) >= W || (pos.x - (size.x * 2)) <= 0 || (pos.y + (size.y * 2)) >= H || (pos.y - (size.y * 2)) <= 0) {
			//cout << "bounce" << endl;
			//angle_o += 90;
			angle_o += rrand(90,130);
			//speed = rrand(1,10)/10.0;
			//SDL_SetRenderDrawColor(renderer,255,255,255,255);
			//SDL_RenderClear(renderer);
		}
		vec2 v = angle(angle_o);
		pos.x += v.x * speed;
		pos.y += v.y * speed;
		//cout << "x: " << pos.x << ", y: " << pos.y << ", angle: " << (angle_o) << "\r";
	}
};

int main() {
	int seed = time(0);
	srand(seed);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(W,H,0,&window,&renderer);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	SDL_RenderSetVSync(renderer,1);
	cout << "seed: " << time(0) << endl;
	SDL_SetRenderDrawColor(renderer,25,25,25,255);
	SDL_RenderClear(renderer);

	vector<ball> balls(500); // balls to render at start

	thread timT(uptimer);
	thread fpsT(fpsUpdater);
	SDL_Event e;
	while(1) {
		SDL_PollEvent(&e);
		if(!inPause)
		{
			SDL_SetRenderDrawColor(renderer,25,25,25,255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer,0xFF,0xFD,0xD0,195);
			for(int i = 0; i < balls.size(); i++) {
				balls[i].update();
				drawCircle(renderer,balls[i].pos,balls[i].size.x,1);
			}
			/*if(tAdded % 1 == 0) {
				for(int i = 0; i < 200; i++) {
					balls.push_back(ball());
				}
			}*/
			SDL_RenderPresent(renderer);
			frames++;
			if(e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_Q) {
				endTs = true;
				timT.join();
				fpsT.join();
				return 0;
			}
		}
		if(e.type == SDL_KEYDOWN) {
			if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				inPause = !inPause;
			}
		}
	};
}