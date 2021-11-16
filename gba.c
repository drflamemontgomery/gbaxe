#define HL_NAME(n) gba_##n

#include <hl.h>
#include <locale.h>

#include <tonc.h>
#include <maxmod.h>
#include <stdlib.h>

#ifdef ISMAIN
#define def
#else
#define def extern
#endif

typedef struct {
  void (*show)(void);
  void (*hide)(void);
  void (*update)(void);
} scene_t;

static void nothing(void) {}

static scene_t next_scene = {
  .show = nothing,
  .hide = nothing,
  .update = nothing,
};
static scene_t current_scene = {
  .show = nothing,
  .hide = nothing,
  .update = nothing,
};

HL_PRIM void HL_NAME(scene_set)(scene_t scene) {
  next_scene = next;
  scene_changed = true;
}

HL_PRIM void HL_NAME(scene_update)() {
  if(scene_changed) {
    scene_changed = false;
    current_scene.hide();
    current_scene = next_scene;
    current_scene.show();
  }
  current_scene.update();
}

#define _SCENE _ABSTRACT(scene_t)
DEFINE_PRIM(_VOID, scene_set, _SCENE);
DEFINE_PRIM(_VOID, scene_update, _NO_ARG);

typedef struct {
  int *frames;
  int len, speed;
  int loop;
} anim_t;

typedef struct {
  FIXED x, y;
} vec2;

#define HFLIP 0x1000
#define VFLIP 0x2000
#define HVFLIP (HFLIP|VFLIP)

// Initialize

HL_PRIM void HL_NAME(initOnce)() {
  irq_init(NULL);
  irq_enable(II_VBLANK);

  irq_add(II_VBLANK, mmVBlank);

  REG_DISPCNT = DCNT_MODE0
    | DCNT_BG0
    | DCNT_OBJ
    | DCNT_OBJ_1D;
}

HL_PRIM void HL_NAME(gameLoop)() {
  qran();

  VBlankIntrWait();
  key_poll();
}

DEFINE_PRIM(_VOID, initOnce, _NO_ARGS);
DEFINE_PRIM(_VOID, gameLoop, _NO_ARGS);



// Object Memory Access

static const int obj_max = 128;
static int aff_max = 32;
static int obj_n = 0, aff_n = 0;

HL_PRIM int HL_NAME(reserve_obj)() {
  return obj_n < obj_max ? obj_n++ : -1;
}

HL_PRIM int HL_NAME(reserve_obj_multi)(int n) {
  int id = obj_n;
  if (obj_n + n <= obj_max) {
    obj_n += n;
    return id;
  }
  return -1;
}

HL_PRIM int HL_NAME(reserve_aff)() {
  return aff_n < aff_max ? aff_n++ : -1;
}

HL_PRIM int HL_NAME(reserve_aff_perm)() {
  return --aff_max;
}

HL_PRIM void HL_NAME(oam_update)() {
  // hide remaining objects in case they were used last frame
  obj_hide_multi(&obj_mem[obj_n], 128 - obj_n);
  obj_n = 0;
  aff_n = 0;
}

DEFINE_PRIM(_I32, reserve_obj, _NO_ARG);
DEFINE_PRIM(_I32, reserve_obj_multi, _I32);
DEFINE_PRIM(_I32, reserve_aff, _NO_ARG);
DEFINE_PRIM(_I32, reserve_aff_perm, _NO_ARG);
DEFINE_PRIM(_VOID, oam_update, _NO_ARG);
