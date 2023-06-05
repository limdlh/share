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
INCLUDEPATH += -I/home/limd/work/rk312x-linux-sdk/external/libdrm/include/drm  \
               -I/home/limd/work/git_dir/ffmpeg/ffmpeg-4.1.3

#LIBS += -L/home/limd/work/opencv-3.4.1/build/lib \
#-lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  \
#-lopencv_shape -lopencv_videoio -lopencv_imgcodecs \
#-lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video \
#-lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
#-lippicv

#mnn
#INCLUDEPATH += -I/home/limd/work/NETDEVSDK/NETDEVSDK_Linux64_V2.6.0.0/include \
#-I/home/limd/work/MNN/schema/current

#LIBS += -L/home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/target/usr/lib

#LIBS += -L/home/limd/ffmpeg_for_rk/third/ffmpeg/lib
LIBS += -L/home/limd/work/git_dir/lib_rk
LIBS += -lavutil -lavformat -lavcodec -lswscale -lswresample -lavdevice -lavfilter -ldl -lfdk-aac -lmp3lame #-lx264 -lpostproc
LIBS += -lm -lz -lpthread -lgnutls -lvpx -lSDL2 -lunistring -ltasn1 -lnettle -lhogweed -lgmp -ldrm -lrockchip_mpp #-lxcb -lxcb-shm 
#drm lvgl rga pthread m avutil avformat avdevice avcodec avfilter swresample swscale rockchip_mpp DeviceIo png freetype
all:
	$(CC) -o dec_drm $(SOURCES) $(INCLUDEPATH) $(LIBS) -g
clean:
	rm -rf *.o dec_drm
