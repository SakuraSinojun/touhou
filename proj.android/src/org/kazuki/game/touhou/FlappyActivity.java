
package org.kazuki.game.touhou;

import org.cocos2dx.lib.Cocos2dxActivity;
// import android.app.Activity;
import android.os.Bundle;

public class FlappyActivity extends Cocos2dxActivity {
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
	}
	
    static {
        System.loadLibrary("game");
    }
}

