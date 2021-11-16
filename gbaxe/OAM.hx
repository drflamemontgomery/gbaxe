package gbaxe;

@:hlNative("gba")
class OAM {

    public inline function reserve(?amount:Int) : Int {
	if(amount == null) {
	    return reserveObj();
	}
	else {
	    return reserveObjMulti(amount);
	}
    }

    public inline function affReserve() : Int {
	return reserveAff();
    }

    public inline function affPermReserve() : Int {
	return reserveAffPerm();
    }

    public inline function update() {
	oamUpdate();
    }
}
