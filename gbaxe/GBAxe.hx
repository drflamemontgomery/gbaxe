package gbaxe;

private typedef Scene = hl.Abstract<"scene_t">;

@:hlNative("gba")
class GBAxe {
    
    static var initDone = false;
    
    public static function init() {
	if( initDone) return;
	initDone = true;
	initOnce();
    }

    public static function update() {
	gameLoop();
    }
}
