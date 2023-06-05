#CONFIG += c++11
#CXX := g++
CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += post_setip_get_reply_by_curl.c

INCLUDEPATH += -I/home/limd/ffmpeg_for_rk/third/third/include/include

#LIBS += -L/home/limd/work/opencv-3.4.1/build/lib \
#-lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  \
#-lopencv_shape -lopencv_videoio -lopencv_imgcodecs \
#-lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video \
#-lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
#-lippicv

#mnn
INCLUDEPATH += -I/home/limd/work/NETDEVSDK/NETDEVSDK_Linux64_V2.6.0.0/include \
#-I/home/limd/work/MNN/schema/current

LIBS += -L./
#LIBS += -lNetDEVSDK -lcloud -lcloudclient -lcloudhttpcurl -lCloudSDK -lcurl -lcyassl -levent -lFAAC -lG711 -lHW_H265dec_Linux64 -lMP2 -lmxml -lNDCodec -lNDFace -lNDMP4 -lNDPlayer -lNDRM -lNDRSA -lNDVideomjpeg -lNetDiscovery -lpolarssl -lSDL2 -lstun -lt2u
LIBS += -lcurl

all:
	$(CC) -o post_setip_get_reply_by_curl post_setip_get_reply_by_curl.c $(INCLUDEPATH) $(LIBS)
clean:
	rm -rf *.o post_setip_get_reply_by_curl

