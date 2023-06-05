#CONFIG += c++11
#CC := gcc
CC := /home/limd/work/rk312x-linux-sdk/buildroot/output/rockchip_rk312x/host/bin/arm-buildroot-linux-gnueabihf-gcc
#TARGET = mnist
#CONFIG += console
#CONFIG -= app_bundle
#TEMPLATE = app
SOURCES += zkpkey_getUserID_getUnlockKey_Unlock_OK.c

#opencv
#INCLUDEPATH += -I/usr/local/include/opencv

#LIBS += -L/home/limd/work/opencv-3.4.1/build/lib \
#-lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  \
#-lopencv_shape -lopencv_videoio -lopencv_imgcodecs \
#-lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video \
#-lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
#-lippicv

#mnn
INCLUDEPATH += -I/home/limd/work/dep/curl/include \
#-I/home/limd/work/MNN/schema/current

#LIBS += -L/home/limd/work/git_dir/test/http/post/make_for_rk
LIBS += -L./
LIBS += -lcurl

all:
	$(CC) -o zkpkey_getUserID_getUnlockKey_Unlock_OK zkpkey_getUserID_getUnlockKey_Unlock_OK.c cJSON.c $(INCLUDEPATH) $(LIBS)
clean:
	rm -rf *.o zkpkey_getUserID_getUnlockKey_Unlock_OK

