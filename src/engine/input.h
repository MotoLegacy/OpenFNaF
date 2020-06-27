// 16 should be enough.. right?
#define MAX_KEYFUNCTIONS       16

typedef struct {
  keycode_t key;
  void (*func)();
  bool pressed;
} keydata_t;

extern void Input_CheckButtons();
extern void Input_RegisterKey(keycode_t key, void (*func));
