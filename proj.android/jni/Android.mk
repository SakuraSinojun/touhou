LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../src/logging.cpp \
                   ../../src/AppDelegate.cpp \
                   ../../src/MainScene.cpp \
                   ../../src/HeroCreator.cpp \
                   ../../src/GameScene.cpp \
                   ../../src/GameMapLayer.cpp \
                   ../../src/MapTile.cpp \
                   ../../src/gamemap.cpp \
                   ../../src/StatusLayer.cpp \
                   ../../src/creature.cpp \
                   ../../src/hero.cpp \
                   ../../src/mapgenerator.cpp \
                   ../../src/dice.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../src

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)
