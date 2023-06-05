#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc ,char **argv){
	//if(system("ffmpeg -f alsa -i hw:0 limd_test.wav") == -1){
	//if(system("ffplay https://qncdnjs.aoscdn.com/local/mv.lightmake.cn/dist/static/media/banner-video.2d1156f6.mp4") == -1){
	//if(system("ffplay -rtsp_transport tcp rtsp://192.168.20.250:554/media/video3") == -1){
	if(system("/userdata/app/hy_gui/ff_player rtsp://192.168.21.180:554/media/video3 1 0 0 360 640") == -1){
		printf("execl filed!\n");
		
		perror("becasue");
	}
	printf("system successful\n");
	return 0;
}


