#Get the Media Player Directory 
MEDIA_PLAYER_DIR = $(firstword $(subst /media_player, ,$(CURDIR)))/media_player

################### Includes ##########################
include $(MEDIA_PLAYER_DIR)/common.mk
include $(MEDIA_PLAYER_API_DIR)/Makefile

################### Targets ###########################
test_app: mediaplayer_api  
	gcc test_app.c $(MEDIA_PLAYER_API_CFLAGS) -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -L$(MEDIA_PLAYER_BUILD_DIR) \
	    -lcunit -Wl,-rpath=$(MEDIA_PLAYER_BUILD_DIR) -lmediaplayer -lglib-2.0 -o $(MEDIA_PLAYER_BUILD_DIR)/test_app

all: test_app

clean: clean_api
	rm -f $(MEDIA_PLAYER_BUILD_DIR)/test_app
