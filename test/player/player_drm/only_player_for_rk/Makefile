#CONFIG += c++11
CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += mp4_to_yuv_to_file.c

INCLUDEPATH += -I/home/limd/work/rk312x-linux-sdk/external/libdrm/include/drm  \
               -I/home/limd/work/git_dir/ffmpeg/ffmpeg-4.1.3


LIBS += -L/home/limd/work/git_dir/lib_rk
LIBS += -lavutil -lavformat -lavcodec -lswscale -lswresample -lm -lz -pthread -lavdevice -lavfilter -ldl -lfdk-aac -lmp3lame -lx264 -lpostproc \
-lgnutls -lvpx -lSDL2 -lunistring -ltasn1 -lnettle -lhogweed -lgmp -ldrm # -pthread -lavdevice -lavfilter -ldl -lxcb -lxcb-shm 
#-lavdevice -lavfilter -lavformat -lavcodec -lswscale -lavresample -lavicap32 -lws2_32 -lpsapi -ladvapi32 -lshell32 -lavutil -lm

all:
	$(CC) -o mp4_to_yuv_to_file mp4_to_yuv_to_file.c $(INCLUDEPATH) $(LIBS) -g
clean:
	rm -rf *.o mp4_to_yuv_to_file
