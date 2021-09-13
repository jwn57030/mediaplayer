#THINGS TO FIX: 
#WRITE SCRIPT/set plugin directory so it runs., add main makefile at top level. make sure build stops if it fails, 

#Get the Media Player Directory 
MEDIA_PLAYER_DIR = $(firstword $(subst /mediaplayer, ,$(CURDIR)))/mediaplayer

################### Includes ##########################
include $(MEDIA_PLAYER_DIR)/common.mk
include $(MEDIA_PLAYER_API_DIR)/Makefile

################### Targets ###########################
$(MEDIA_PLAYER_BUILD_DIR):
	-mkdir $(MEDIA_PLAYER_BUILD_DIR) 

test_app: mediaplayer_api
	gcc test_app.c $(MEDIA_PLAYER_API_CFLAGS) $(MEDIA_PLAYER_API_LIBS) -L$(MEDIA_PLAYER_BUILD_DIR) \
	    -lcunit -Wl,-rpath=$(MEDIA_PLAYER_BUILD_DIR) -lmediaplayer -o $(MEDIA_PLAYER_BUILD_DIR)/test_app

all: $(MEDIA_PLAYER_DIR)/build test_app

clean: clean_api
	rm -f $(MEDIA_PLAYER_BUILD_DIR)/test_app

.PHONY: all clean test_app
