This is working example of gstreamer code meant to demonstrate my knowledge of gstreamer/glib.  It demostrates implementations of a gstreamer plugin, gstreamer debug, gstreamer message handling, threads, mutexes, conditional waits, pointers, function pointers/callback functions, and unit testing using CUnit.

You will need opensource packages glib-2.0 >= 2.68.4, cunit >= 2.1-3, and gstreamer >= 1.18.4  

1. Create directory you wish to clone repository in (mydir).
2. cd mydir
3. Clone repository to directory.
4. cd mydir/mediaplayer/test_app; make all
5. add mydir/mediaplayer/build/plugins to GST_PLUGIN_PATH.
6. mydir/build/test_app
