#Define Directory Locations for Plugins Directory
MEDIA_PLAYER_ELEMENT_DIR        := $(MEDIA_PLAYER_PLUGIN_DIR)/media_player
MEDIA_PLAYER_PLUGIN_INCLUDE_DIR := $(MEDIA_PLAYER_PLUGIN_DIR)/include
MEDIA_PLAYER_BUILD_PLUGIN_DIR   := $(MEDIA_PLAYER_BUILD_DIR)/plugins

#Define common libs and CFLAGS for Plugins directory to use
MEDIA_PLAYER_PLUGIN_LIBS   := $(shell pkg-config --libs gstreamer-1.0) 
MEDIA_PLAYER_PLUGIN_CFLAGS := $(CFLAGS) \
							  $(shell pkg-config --cflags gstreamer-1.0) \
							 -I$(MEDIA_PLAYER_PLUGIN_INCLUDE_DIR)
