#include<SDL.h>
#include<stdlib.h>
int main(){
	SDL_Surface *screen;
	Uint32 color;
	int x;
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		fprintf(stderr,"abc %s\n",SDL_GetError());
		exit(1);
	}
	screen=SDL_SetVideoMode(640,480,16,SDL_SWSURFACE);
	if(screen==NULL){
		fprintf(stderr," 640x480x16  %s",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
	for(x=0;x<=255;x+=1)
	{
		color=SDL_MapRGB(screen->format,255,255,x);
		SDL_FillRect(screen,NULL,color);
		SDL_UpdateRect(screen,0,0,0,0);
	}
	SDL_Delay(6000);
	return 0;
}
