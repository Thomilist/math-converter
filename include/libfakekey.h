#ifndef __LIBFAKEKEY_H__
#define __LIBFAKEKEY_H__

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic push

#ifdef __cplusplus
extern "C" {
#endif

#include <fakekey/fakekey.h>
#include <X11/extensions/XTest.h>

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define N_MODIFIER_INDEXES (Mod5MapIndex + 1)

typedef unsigned int FkChar32;

struct FakeKey
{
  Display *xdpy;
  int      min_keycode, max_keycode;
  int      n_keysyms_per_keycode;
  KeySym  *keysyms;
  int      held_keycode;
  int      held_state_flags;
  KeyCode  modifier_table[N_MODIFIER_INDEXES];
  int      shift_mod_index, alt_mod_index, meta_mod_index;
};

int 
utf8_to_ucs4 (const unsigned char *src_orig,
	      FkChar32	          *dst,
	      int	           len);

FakeKey*
utf8_fakekey_init(Display *xdpy);

int
utf8_fakekey_reload_keysyms(FakeKey *fk);

int 
utf8_fakekey_send_keyevent(FakeKey *fk, 
		      KeyCode  keycode,
		      Bool     is_press,
		      int      flags);

int
utf8_fakekey_press_keysym(FakeKey *fk, 
		     KeySym   keysym,
		     int      flags);

int
utf8_fakekey_press(FakeKey             *fk, 
	      const unsigned char *utf8_char_in,
	      int                  len_bytes,
	      int                  flags);

void
utf8_fakekey_repeat(FakeKey *fk);

void
utf8_fakekey_release(FakeKey *fk);

#ifdef __cplusplus
}
#endif

#pragma GCC diagnostic pop

#endif // __LIBFAKEKEY_H__