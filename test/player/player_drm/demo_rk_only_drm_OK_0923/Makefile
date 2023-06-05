#CONFIG += c++11
#CC := gcc
CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += dec_drm.c
SOURCES += drm_display.c

#INCLUDEPATH += -I/usr/include/libdrm
INCLUDEPATH += -I/home/limd/work/rk312x-linux-sdk/external/libdrm/include/drm

LIBS += -L/home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/target/usr/lib
LIBS += -ldrm -g

#LIBS += -L/usr/local/ffmpeg/lib
#LIBS += -lavformat -lavcodec -lswscale -lswresample -lavutil -lm -lz -pthread -lavdevice -lavfilter -ldl -lxcb -lxcb-shm -lxcb-xfixes


all:
	$(CC) -o dec_drm $(SOURCES) $(INCLUDEPATH) $(LIBS)
clean:
	rm -rf *.o dec_drm
