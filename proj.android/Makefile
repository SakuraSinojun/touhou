
ANDROID_SDK_PATH := /opt/adt-bundle-linux-x86_64-20131030/sdk
TOOLS_PATH := /opt/adt-bundle-linux-x86_64-20131030/sdk/build-tools/android-4.4
CROSS_PATH := /opt/android-ndk-r9c/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86_64/bin/arm-linux-androideabi-


OUTPUT_PATH := $(PWD)/out


PACKAGE_EXPORT := /opt/adt-bundle-linux-x86_64-20131030/sdk/platforms/android-19/android.jar
CLASS_PATH	:= /opt/adt-bundle-linux-x86_64-20131030/sdk/platforms/android-19/android.jar:/opt/adt-bundle-linux-x86_64-20131030/sdk/extras/android/support/v13/

AAPT	:= $(TOOLS_PATH)/aapt
JAVAC 	:= javac
CC		:= $(CROSS_PATH)gcc
CXX		:= $(CROSS_PATH)g++
ACP		:= cp
DX		:= $(TOOLS_PATH)/dx
ZIPALIGN := $(ANDROID_SDK_PATH)/tools/zipalign
SIGNAPK := java -jar tools/signapk.jar 


JFLAGS  := -J-Xmx512M -target 1.5 -Xmaxerrs 9999999 -encoding UTF-8 -g 
JFLAGS  += -classpath $(CLASS_PATH)

SOURCES := jni/hellocpp/main.cpp
SOURCES += ../src/logging.cpp
SOURCES += ../src/AppDelegate.cpp
SOURCES += ../src/MainScene.cpp

OBJS	:= $(SOURCES:.cpp=.o)
DEPS	:= $(SOURCES:.cpp=.d)

INCLUDES := 
INCLUDES += -I jni
INCLUDES += -I /opt/android-ndk-r9c/platforms/android-19/arch-arm/usr/include
# INCLUDES += -I $(ANDROID_SDK_PATH)/out/target/product/Hi3719MV100_A/obj/SHARED_LIBRARIES/libiptvmw_jni_intermediates
INCLUDES += -I $(ANDROID_SDK_PATH)/libnativehelper/include/nativehelper

CXXFLAGS := -fno-exceptions -Wno-multichar -msoft-float -fpic -fPIE -ffunction-sections -fdata-sections -funwind-tables 
CXXFLAGS += -fstack-protector -Wa,--noexecstack -Werror=format-security -D_FORTIFY_SOURCE=1 -fno-short-enums -march=armv7-a 
CXXFLAGS += -mfloat-abi=softfp -mfpu=neon -Wno-unused-but-set-variable -fno-builtin-sin -fno-strict-volatile-bitfields 
CXXFLAGS += -Wno-psabi -mthumb-interwork -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith 
CXXFLAGS += -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -g -Wstrict-aliasing=2 
CXXFLAGS += -fgcse-after-reload -frerun-cse-after-loop -frename-registers -fvisibility-inlines-hidden -fmessage-length=0 
CXXFLAGS += -W -Wall -Wno-unused -Winit-self -Wpointer-arith -Wsign-promo -Werror=return-type -Werror=non-virtual-dtor 
CXXFLAGS += -Werror=address -Werror=sequence-point -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -fno-rtti -g 
CXXFLAGS += -O2 -Wno-missing-field-initializers -MD 

CXXFLAGS += -DCHIP_TYPE_hi3716cv101 -DUSE_AEC -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DADJTIME_MISSING -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -D_linux_=1 -DNO_DRAND48 -DHAVE_PATHS_H -DHAVE_MMAP -DRP_VERSION="" -DNDEBUG -UDEBUG -DANDROID 
# -MF out/target/product/Hi3719MV100_A/obj/SHARED_LIBRARIES/libiptvmw_jni_intermediates/IPTVMiddlewareJNI.d

LINKFLAGS := 
WHOLE_LIB := jni/res/lib/iptvLib/libandroidmw.a 
WHOLE_LIB += jni/res/lib/takinLib/libHippoGlue.a
WHOLE_LIB += jni/res/lib/takinLib/libTakin.a 
WHOLE_LIB += jni/res/lib/takinLib/libwebkit-takin.a 
WHOLE_LIB += jni/res/lib/takinLib/libjsc.a 
WHOLE_LIB += jni/res/lib/takinLib/libwtf.a 
WHOLE_LIB += jni/res/lib/takinLib/libwebcore-takin.a 

LINKFLAGS := -lc -llog -lcutils -lstdc++ -licuuc -licui18n -lstlport -ldl -lcurl_withssl -lcares -lcrypto -lssh2 -lssl -lgloox -lzebra -lutils -landroid_runtime -lnativehelper -lbinder -lui -llog -lcutils -lgui -lhi_png -ljpeg -lfreetype2 -lz -lc -lstdc++ -lm
LINKFLAGS += -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--icf=safe -Wl,--fix-cortex-a8  

LANGUAGES := en_US,en_US,cs_CZ,da_DK,de_AT,de_CH,de_DE,de_LI,el_GR,en_AU,en_CA,en_GB,en_NZ,en_SG,eo_EU,es_ES,fr_CA,fr_CH,fr_BE,fr_FR,it_CH,it_IT,ja_JP,ko_KR,nb_NO,nl_BE,nl_NL,pl_PL,pt_PT,ru_RU,sv_SE,tr_TR,zh_CN,zh_HK,zh_TW,am_ET,hi_IN,en_US,fr_FR,it_IT,es_ES,de_DE,nl_NL,cs_CZ,pl_PL,ja_JP,zh_TW,zh_CN,ru_RU,ko_KR,nb_NO,es_US,da_DK,el_GR,tr_TR,pt_PT,pt_BR,rm_CH,sv_SE,bg_BG,ca_ES,en_GB,fi_FI,hi_IN,hr_HR,hu_HU,in_ID,iw_IL,lt_LT,lv_LV,ro_RO,sk_SK,sl_SI,sr_RS,uk_UA,vi_VN,tl_PH,ar_EG,fa_IR,th_TH,sw_TZ,ms_MY,af_ZA,zu_ZA,am_ET,hi_IN,mdpi,nodpi 

all: makeassets R iptv jni acp makejar dex package sign

sign:
	$(SIGNAPK) keys/security/platform.x509.pem keys/security/platform.pk8 $(OUTPUT_PATH)/classes/package.apk $(OUTPUT_PATH)/classes/package.apk.signed
	$(ZIPALIGN) -f 4 $(OUTPUT_PATH)/classes/package.apk.signed $(OUTPUT_PATH)/classes/package.apk.aligned
	$(ACP) $(OUTPUT_PATH)/classes/package.apk.aligned $(OUTPUT_PATH)/touhou.apk

package:
	touch $(OUTPUT_PATH)/classes/dummy
	(cd $(OUTPUT_PATH)/classes/ && jar cf package.apk dummy)
	zip -qd $(OUTPUT_PATH)/classes/package.apk dummy
	rm $(OUTPUT_PATH)/classes/dummy
	$(AAPT) package -u -z -c $(LANGUAGES) --preferred-configurations xhdpi -M AndroidManifest.xml -S res -A assets -I $(PACKAGE_EXPORT) --min-sdk-version 17 --target-sdk-version 17 --product tablet --version-code 17 --version-name 4.2.2-eng.kazuki.20140226.180232 -F $(OUTPUT_PATH)/classes/package.apk
	rm -fr $(OUTPUT_PATH)/classes/lib
	mkdir -pv $(OUTPUT_PATH)/classes/lib/armeabi/
	# cp $(OUTPUT_PATH)/libs/libiptvmw_jni.so $(OUTPUT_PATH)/classes/lib/armeabi
	cp libs/armeabi/* $(OUTPUT_PATH)/classes/lib/armeabi
	(cd $(OUTPUT_PATH)/classes && zip -r package.apk lib)
	cp $(OUTPUT_PATH)/classes/noproguard.classes.dex $(OUTPUT_PATH)/classes/classes.dex
	$(AAPT) add -k $(OUTPUT_PATH)/classes/package.apk $(OUTPUT_PATH)/classes/classes.dex

dex:
	$(DX) -JXms16M -JXmx2048M --dex --output=$(OUTPUT_PATH)/classes/noproguard.classes-with-local.dex $(OUTPUT_PATH)/classes/noproguard.classes.jar
	$(ACP) $(OUTPUT_PATH)/classes/noproguard.classes-with-local.dex $(OUTPUT_PATH)/classes/noproguard.classes.dex


makejar:
	# rm -fr $(OUTPUT_PATH)/jarjar
	# mkdir -pv $(OUTPUT_PATH)/jarjar
	# cp -ard $(OUTPUT_PATH)/com $(OUTPUT_PATH)/android $(OUTPUT_PATH)/jarjar/
	# jar -cf $(OUTPUT_PATH)/classes/classes-full-debug.jar -C $(OUTPUT_PATH)/jarjar .
	jar -cf $(OUTPUT_PATH)/classes/classes-full-debug.jar -C $(OUTPUT_PATH)/ org
	$(ACP) -fp $(OUTPUT_PATH)/classes/classes-full-debug.jar $(OUTPUT_PATH)/classes/classes.jar
	$(ACP) -fp $(OUTPUT_PATH)/classes/classes-full-debug.jar $(OUTPUT_PATH)/classes/noproguard.classes.jar

acp:
	# $(ACP) -fp $(OUTPUT_PATH)/libs/libiptvmw_jni.so  $(OUTPUT_PATH)/target/libs/libiptvmw_jni.so

jni:

iptv:
	find src -name '*.java' > out/java-source-list-uniq
	find out -name 'R.java' >> out/java-source-list-uniq
	$(JAVAC) $(JFLAGS) -d $(OUTPUT_PATH) -extdirs $(OUTPUT_PATH) @$(OUTPUT_PATH)/java-source-list-uniq
	echo "done."

R:
	$(AAPT) package -f -m -J $(OUTPUT_PATH) -S res -I $(PACKAGE_EXPORT) -M AndroidManifest.xml

makeassets:
	rm assets -fr
	mkdir assets
	cp ../Resources/* assets -ard
	./build_native.sh

prebuilt: clean
	mkdir -pv $(OUTPUT_PATH)/libs
	mkdir -pv $(OUTPUT_PATH)/classes
	mkdir -pv $(OUTPUT_PATH)/target/libs

.cpp.o:
	$(CXX) -c $< -o $@ $(INCLUDES) $(CXXFLAGS)


clean:
	rm -fr $(OUTPUT_PATH)
	rm -fr assets
	rm -fr obj
	rm libs -fr
	find jni -name '*.o' -exec rm -f {} \;
	find jni -name '*.d' -exec rm -f {} \;


