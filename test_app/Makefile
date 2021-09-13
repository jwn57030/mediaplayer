#Get the Media Player Directory 
MEDIA_PLAYER_DIR = $(firstword $(subst /mediaplayer, ,$(CURDIR)))/mediaplayer

################### Includes ##########################
include $(MEDIA_PLAYER_DIR)/common.mk
include $(MEDIA_PLAYER_API_DIR)/Makefile

################### Targets ###########################
$(MEDIA_PLAYER_BUILD_DIR):
	-mkdir $(MEDIA_PLAYER_BUILD_DIR) 

test_app: mediaplayer_api  
	gcc test_app.c $(MEDIA_PLAYER_API_CFLAGS) -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -L$(MEDIA_PLAYER_BUILD_DIR) \
	    -lcunit -Wl,-rpath=$(MEDIA_PLAYER_BUILD_DIR) -lmediaplayer -lglib-2.0 -o $(MEDIA_PLAYER_BUILD_DIR)/test_app

all: $(MEDIA_PLAYER_DIR)/build test_app

clean: clean_api
	rm -f $(MEDIA_PLAYER_BUILD_DIR)/test_app
