
#Common Flags
CFLAGS := -Werror

#Directory Defines
MEDIA_PLAYER_DIR                := $(firstword $(subst /mediaplayer, ,$(CURDIR)))/mediaplayer
MEDIA_PLAYER_BUILD_DIR          := $(MEDIA_PLAYER_DIR)/build
MEDIA_PLAYER_PLUGIN_DIR         := $(MEDIA_PLAYER_DIR)/plugin
MEDIA_PLAYER_PUBLIC_INCLUDE_DIR := $(MEDIA_PLAYER_DIR)/public_include
MEDIA_PLAYER_API_DIR            := $(MEDIA_PLAYER_DIR)/api


