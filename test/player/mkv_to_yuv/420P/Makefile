#CONFIG += c++11
CC := gcc
#CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += mkv_to_yuv_to_file.c

#opencv
#INCLUDEPATH += -I/usr/local/include/opencv

#LIBS += -L/home/limd/work/opencv-3.4.1/build/lib \
#-lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  \
#-lopencv_shape -lopencv_videoio -lopencv_imgcodecs \
#-lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video \
#-lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
#-lippicv

#mnn
#INCLUDEPATH += -I/home/limd/work/NETDEVSDK/NETDEVSDK_Linux64_V2.6.0.0/include \
#-I/home/limd/work/MNN/schema/current

LIBS += -L/usr/local/ffmpeg/lib
LIBS += -lavformat -lavcodec -lswscale -lswresample -lavutil -lm -lz -pthread -lavdevice -lavfilter -ldl -lxcb -lxcb-shm -lxcb-xfixes
#-lavdevice -lavfilter -lavformat -lavcodec -lswscale -lavresample -lavicap32 -lws2_32 -lpsapi -ladvapi32 -lshell32 -lavutil -lm

all:
	$(CC) -o mkv_to_yuv_to_file mkv_to_yuv_to_file.c $(INCLUDEPATH) $(LIBS) -g
clean:
	rm -rf *.o mkv_to_yuv_to_file
