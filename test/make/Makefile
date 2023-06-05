#CONFIG += c++11
CC := gcc
#CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += get_by_curl.c

#opencv
#INCLUDEPATH += -I/usr/local/include/opencv

#LIBS += -L/home/limd/work/opencv-3.4.1/build/lib \
#-lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  \
#-lopencv_shape -lopencv_videoio -lopencv_imgcodecs \
#-lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video \
#-lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
#-lippicv

#mnn
INCLUDEPATH += -I/home/limd/work/NETDEVSDK/NETDEVSDK_Linux64_V2.6.0.0/include \
-I/usr/include/x86_64-linux-gnu/

LIBS += -L/home/limd/ffmpeg_for_rk/third/curl/lib/
#LIBS += -lcurl

all:
	$(CC) -o get_by_curl get_by_curl.c $(INCLUDEPATH) $(LIBS) -lcurl -g
clean:
	rm -rf *.o get_by_curl

