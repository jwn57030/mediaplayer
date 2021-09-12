#Get the Media Player Directory 
MEDIA_PLAYER_DIR = $(firstword $(subst /media_player, ,$(CURDIR)))/media_player
include $(MEDIA_PLAYER_DIR)/common.mk 
include $(MEDIA_PLAYER_PLUGIN_DIR)/common.mk
include $(MEDIA_PLAYER_ELEMENT_DIR)/Makefile

plugins: media_player_plugin

clean_plugins: clean_media_player_plugin

