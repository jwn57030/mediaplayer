#Setup the Media Player Directories needed for includes 
MEDIA_PLAYER_DIR := $(firstword $(subst /mediaplayer, ,$(CURDIR)))/mediaplayer
include $(MEDIA_PLAYER_DIR)/common.mk
include $(MEDIA_PLAYER_PLUGIN_DIR)/common.mk

######################## Builds Flags ##############################

############################# File Definitions ######################
LIB_MEDIA_PLAYER_PLUGIN := $(MEDIA_PLAYER_BUILD_PLUGIN_DIR)/libgstmediaplayer.so

######################## Targets ####################################
$(LIB_MEDIA_PLAYER_PLUGIN): $(MEDIA_PLAYER_ELEMENT_DIR)/media_player_plugin.c
	gcc -shared -fPIC -ffile-prefix-map=$(MEDIA_PLAYER_ELEMENT_DIR)/= $(MEDIA_PLAYER_PLUGIN_CFLAGS) $(MEDIA_PLAYER_PLUGIN_LIBS) \
		$(MEDIA_PLAYER_ELEMENT_DIR)/media_player_plugin.c -o $(LIB_MEDIA_PLAYER_PLUGIN)

media_player_plugin: $(LIB_MEDIA_PLAYER_PLUGIN)
	
clean_media_player_plugin:
	rm -f $(LIB_MEDIA_PLAYER_PLUGIN)

.PHONY: media_player_plugin clean_media_player_plugin