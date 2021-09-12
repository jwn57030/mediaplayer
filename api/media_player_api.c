/*************************************************
* \file      media_player_api.c
* \details   Media Player api Implementation
* \author    Jason Neitzert
* \date      8/22/2021
* \Copyright Jason Neitzert 
*************************************************/

/***************** Includes *********************/
#include <stdio.h>
#include <gst/gst.h>
#include "media_player_api.h"


/***************** Defines **********************/
/* PRINTF Text Colors and Formats */ 
#define DEFAULT_TEXT       "\x1B[0m"
#define BOLD_TEXT          "\x1B[1m" 
#define DULL_TEXT          "\x1B[2m"
#define ITALIC_TEXT        "\x1B[3m"
#define UNDERLINE_TEXT     "\x1B[4m"
#define STRIKETHROUGH_TEXT "\x1B[9m"

#define FLASHING_TEXT      "\x1B[5m"
#define FLASHING_TEXT_2    "\x1B[6m" /* Doesn't seem diffrent than 5 */

#define SWAP_FOREGROUND_BACKGROUND_COLORS  "\x1B[7m"
#define SET_BACKGROUND_TO_FOREGROUND_COLOR "\x1B[8m" /* Would be useful in entering password, so not able to see it */

#define RED_TEXT      "\x1B[31m"
#define BLUE_TEXT     "\x1B[34m"
#define CYAN_TEXT     "\x1B[36m"

/***************** Structures and Enums *********/
struct MediaPlayer
{
   GstElement *p_element;
   gulong media_player_signal_handler_id;
   MpMessageCallback mp_message_callback;
};

/***************** Private Global Variables *************/
static GMutex   init_mutex = {0};
static gboolean lib_inited = FALSE;

/***************** Private Function Definitions **********/

/****************** Private Functions *******************/
/**
 * \brief Media Player Debug handler for gstreamer debug
 * 
 * \param[in] p_category - Debug Category
 * \param[in] level      - debug level
 * \param[in] p_file     - file name debug comes from
 * \param[in] p_function - function name debug is comming from
 * \param[in] line       - line debug is on
 * \param[in] p_object   - object debug is associated with
 * \param[in] p_message  - the debug message itself
 * \param[in] user_data  - user data provided when logging was inited
 * 
 * \return void
 * \author Jason Neitzert
 */
G_GNUC_NO_INSTRUMENT
static void media_player_log(GstDebugCategory *p_category,
                             GstDebugLevel level,
                             const gchar *p_file,
                             const gchar *p_function,
                             gint line,
                             GObject *p_object,
                             GstDebugMessage *p_message,
                             gpointer user_data)
{
   /* Print Debug Category in Cyan Text */
   printf(BOLD_TEXT CYAN_TEXT "[%s]", gst_debug_category_get_name(p_category));

   /* Print Log level in appropriate Color */
   printf(BOLD_TEXT RED_TEXT " %s",  gst_debug_level_get_name(level));

   /* Print everything else in default text */
   printf(DEFAULT_TEXT "%s(%d): %s\n", p_file, line, gst_debug_message_get(p_message));

   /* Always make sure text gets set back to default, before leaving this function */
}

/**
 * \brief Recieves Message Notifications from the MediaPlayer Plugin 
 *
 * \param p_element - pointer to GstElement message originated from
 * \param p_message - pointer to GstMessage
 * \param p_user_data - pointer to callback function provided in media_player_new
 * 
 * \return void
 * \author Jason Neitzert
 */
static void mediaplayer_message_callback(GstElement *p_element, GstMessage *p_message, MediaPlayer *p_media_player)
{
   if (p_media_player->mp_message_callback)
   {
      p_media_player->mp_message_callback(eMP_EOS);
   }     
}

/***************** Public Functions *************/

/**
 * \brief Inits media player library
 * 
 * \return void
 * \author Jason Neitzert
 */
void media_player_api_init()
{
   g_mutex_lock(&init_mutex);    
   if (!lib_inited)
   {  
      /* Setup Gstreamer Debug to output errors */
      gst_debug_set_default_threshold(GST_LEVEL_ERROR);
      gst_debug_remove_log_function(NULL);
      gst_debug_add_log_function(media_player_log, NULL, NULL);

      gst_init(NULL, NULL);

      lib_inited = TRUE;
    }
    g_mutex_unlock(&init_mutex);   

}

/**
 * \brief Uninits media player library
 * \details This does not need to be used normally. In normal usage
 *          it would be expected this library would be loaded for duration
 *          of process. This would normally be used for assisting in
 *          finding memory leaks.  
 * 
 * \return void
 * \author Jason Neitzert
 */
void media_player_api_uninit()
{
   g_mutex_lock(&init_mutex);

   gst_deinit();
   gst_debug_remove_log_function(media_player_log);
   gst_debug_add_log_function(gst_debug_log_default, NULL, NULL);
   gst_debug_set_default_threshold(GST_LEVEL_NONE);
   
   lib_inited = FALSE;
   g_mutex_unlock(&init_mutex);
}

/**
 * \brief Creates a new media player
 * 
 * \return MediaPlayer* - pointer to new media player object
 * \author Jason Neitzert
 */
MediaPlayer *media_player_new(MpMessageCallback mp_message_callback)
{
   MediaPlayer *p_media_player = g_slice_new0(MediaPlayer);
   
   if (!p_media_player)
   {
      GST_ERROR("Failed to alloc MediaPlayer");
      return p_media_player;
   }

   if (!(p_media_player->p_element = gst_element_factory_make("mediaplayer", NULL)) ||
            !(p_media_player->media_player_signal_handler_id = 
                 g_signal_connect(p_media_player->p_element, "message-callback",
                                  (GCallback)mediaplayer_message_callback, p_media_player)))
   {
      GST_ERROR("Failed to Initialize MediaPlayer");
      media_player_destroy(p_media_player);
      p_media_player = NULL;
      return p_media_player;
   }

    p_media_player->mp_message_callback = mp_message_callback;

    return p_media_player;
} 

/**
 * \brief destroy a media player
 * 
 * \param[in] p_media_player - pointer to media player object
 * 
 * \return void
 * \author Jason Neitzert
 */
void media_player_destroy(MediaPlayer *p_media_player)
{
   /* We don't care about failure her as we will try to continue with destruction */
   (void)gst_element_set_state(p_media_player->p_element, GST_STATE_NULL);
   g_signal_handler_disconnect(p_media_player->p_element, p_media_player->media_player_signal_handler_id);
   gst_object_unref(p_media_player->p_element);
   g_slice_free(MediaPlayer, p_media_player);
}

/**
 * \brief Put player into playing state
 * 
 * \param[in] p_media_player - pointer to media player object
 * 
 * \return bool - true if succedded
 * \author Jason Neitzert
 */
bool media_player_play(MediaPlayer *p_media_player)
{
   bool retval = true;

   if (GST_STATE_CHANGE_FAILURE ==  gst_element_set_state(p_media_player->p_element, GST_STATE_PLAYING))
   {
      retval = false;
   }

   return retval;
}

/**
 * \brief put player into paused state
 * 
 * \param[in] p_media_player - pointer to media player object
 * 
 * \return bool - true if succedded
 * \author Jason Neitzert
 */
bool media_player_pause(MediaPlayer *p_media_player)
{
   bool retval = true;

   if (GST_STATE_CHANGE_FAILURE ==  gst_element_set_state(p_media_player->p_element, GST_STATE_PAUSED))
   {
      retval = false;
   }

   return retval;
}

