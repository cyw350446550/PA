#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t len) {
  int key=_read_key();
	bool flag=false;
	if(key&0x8000){
		key^=0x8000;
		flag=true;
	}
	if(key!=_KEY_NONE){
		sprintf(buf,"%s %s\n",flag?"kd":"ku",keyname[key]);
	}else{
		sprintf(buf,"t %d\n",_uptime());
	}
	return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
  strncpy(buf, dispinfo + offset, len);
}

void fb_write(const void *buf, off_t offset, size_t len) {
	int width=_screen.width;
	_draw_rect((uint32_t *)(buf),(offset/4)%width,(offset/4)/width,len/4,1);
}

void init_device() {
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  strcpy(dispinfo ,"WIDTH:400\nHEIGHT:300");
}
