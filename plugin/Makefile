#Get the Media Player Directory 
MEDIA_PLAYER_DIR = $(firstword $(subst /mediaplayer, ,$(CURDIR)))/mediaplayer
include $(MEDIA_PLAYER_DIR)/common.mk 
include $(MEDIA_PLAYER_PLUGIN_DIR)/common.mk
include $(MEDIA_PLAYER_ELEMENT_DIR)/Makefile

$(MEDIA_PLAYER_BUILD_PLUGIN_DIR):
	-mkdir $(MEDIA_PLAYER_BUILD_PLUGIN_DIR)

plugins: $(MEDIA_PLAYER_BUILD_PLUGIN_DIR) media_player_plugin

clean_plugins: clean_media_player_plugin

