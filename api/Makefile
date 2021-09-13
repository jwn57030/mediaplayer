#Setup the Media Player Directories needed for includes 
MEDIA_PLAYER_DIR = $(firstword $(subst /mediaplayer, ,$(CURDIR)))/mediaplayer
include $(MEDIA_PLAYER_DIR)/common.mk
include $(MEDIA_PLAYER_PLUGIN_DIR)/Makefile

LIB_MEDIA_PLAYER_API=$(MEDIA_PLAYER_BUILD_DIR)/libmediaplayer.so

MEDIA_PLAYER_API_CFLAGS=-I$(MEDIA_PLAYER_PUBLIC_INCLUDE_DIR)

######################## Targets ####################################
$(LIB_MEDIA_PLAYER_API): $(MEDIA_PLAYER_API_DIR)/media_player_api.c
	gcc -fPIC -shared $(MEDIA_PLAYER_API_CFLAGS) $(MEDIA_PLAYER_PLUGIN_LIBS) \
		$(MEDIA_PLAYER_PLUGIN_CFLAGS) $(MEDIA_PLAYER_API_DIR)/media_player_api.c -o $(LIB_MEDIA_PLAYER_API) 

mediaplayer_api: plugins $(LIB_MEDIA_PLAYER_API) 

clean_api: clean_plugins
	rm -f $(LIB_MEDIA_PLAYER_API)
