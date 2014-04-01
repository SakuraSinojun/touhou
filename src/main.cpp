#include "AppDelegate.h"
#include "cocos2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <locale.h>

USING_NS_CC;

int main(int argc, char **argv)
{
    setlocale(LC_ALL, ""); 
    // create the application instance
    AppDelegate app;

    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize(1280 / 1.5f, 720/ 1.5f);
    // eglView->setFrameSize(2048 / 4, 1536/ 4);

    return CCApplication::sharedApplication()->run();
}
