#Define Directory Locations for Plugins Directory
MEDIA_PLAYER_ELEMENT_DIR=$(MEDIA_PLAYER_PLUGIN_DIR)/media_player
MEDIA_PLAYER_PLUGIN_INCLUDE_DIR=$(MEDIA_PLAYER_PLUGIN_DIR)/include


MEDIA_PLAYER_PLUGIN_LIBS = -pthread -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
MEDIA_PLAYER_PLUGIN_CFLAGS = -I/usr/include/gstreamer-1.0 \
							 -I/usr/include/glib-2.0 \
							 -I/usr/lib64/glib-2.0/include \
							 -I/usr/include/sysprof-4 \
							 -I$(MEDIA_PLAYER_PLUGIN_INCLUDE_DIR)