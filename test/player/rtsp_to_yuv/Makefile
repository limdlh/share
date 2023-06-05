#CONFIG += c++11
CC := gcc
#CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += rtsp_to_yuv_to_file.c

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
LIBS += -lavformat -lavcodec -lswscale -lswresample -lavutil -lm -lz -pthread #-lbz2 -llzma
#-lNetDEVSDK -lcloud -lcloudclient -lcloudhttpcurl -lCloudSDK -lcurl -lcyassl -levent -lFAAC -lG711 -lHW_H265dec_Linux64 -lMP2 -lmxml -lNDCodec -lNDFace -lNDMP4 -lNDPlayer -lNDRM -lNDRSA -lNDVideomjpeg -lNetDiscovery -lpolarssl -lSDL2 -lstun -lt2u

all:
	$(CC) -o rtsp_to_yuv_to_file rtsp_to_yuv_to_file.c $(INCLUDEPATH) $(LIBS) -g
clean:
	rm -rf *.o rtsp_to_yuv_to_file