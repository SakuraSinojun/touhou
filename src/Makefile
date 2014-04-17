EXECUTABLE = touhou

DEBUG := 1

INCLUDES = -I. -IChapter01 -Wall

SOURCES = 	main.cpp \
		  	logging.cpp \
        	AppDelegate.cpp \
        	MainScene.cpp \
        	HeroCreator.cpp \
        	MapTile.cpp \
        	gamemap.cpp \
        	dice.cpp \
        	StatusLayer.cpp \
        	creature.cpp \
        	hero.cpp \
        	mapgenerator_room.cpp \
        	HpBar.cpp \
        	ai/normal.cpp \
        	monsters/monster.cpp \
        	ornaments/ornament.cpp \
        	GameOverScene.cpp \
        	PlayingScene.cpp \
        	MainLayer.cpp \
        	MapLayer.cpp \
        	MapWrapper.cpp \
        	GameResource.cpp \

COCOS_ROOT = /home/kazuki/projects/kazuki/third_party/cocos2d-x-2.2.2
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: %.cc $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
