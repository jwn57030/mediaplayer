/**
* \file      media_player_plugin.c
* \details   Media Player Plugin Implementation
* \author    Jason Neitzert
* \date      8/30/2021
* \Copyright Jason Neitzert 
*/

/***************** Includes ********************/
#include <gst/gst.h>

/***************** Defines *********************/
#define PACKAGE                     "MediaPlayerPlugin"
#define MEDIA_PLAYER_VERSION        "1.0.0"
#define MEDIA_PLAYER_LICENSE        "Proprietary"
#define MEDIA_PLAYER_PACKAGE_NAME   "MediaPlayerPlugin"
#define MEDIA_PLAYER_PACKAGE_ORIGIN "JasonNeitzert"

#define GST_TYPE_MEDIA_PLAYER gst_mediaplayer_get_type()

/******************** Enums   ****************************/
enum
{
  SIGNAL_MESSAGE_CALLBACK,
  LAST_SIGNAL
};

/***************** Structures ****************************/
typedef struct
{
    GstElement element;

    GstElement *p_pipeline;    
    GThread    *p_msg_thread;
    gboolean    shutdown;
    GstBus     *p_bus;
} GstMediaPlayer;

typedef struct 
{
    GstElementClass element_klass;
} GstMediaPlayerClass;


/***************** Private Function Definitions **********/
static gboolean mediaplayer_plugin_init(GstPlugin *p_plugin);
static GstStateChangeReturn gst_mediaplayer_change_state(GstElement *p_element,
                                                         GstStateChange transition);


/***************** Public Global Variables ***************/
/* Expose plugin definition to plugin scanner */
GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  mediaplayer,
                  "Awesome Media Player Plugin",
                  mediaplayer_plugin_init,
                  MEDIA_PLAYER_VERSION,
                  MEDIA_PLAYER_LICENSE,
                  MEDIA_PLAYER_PACKAGE_NAME, 
                  MEDIA_PLAYER_PACKAGE_ORIGIN)

/***************** Private Global Variables **************/
GST_DEBUG_CATEGORY_STATIC(media_player_plugin_debug);

static guint gst_mediaplayer_signals[LAST_SIGNAL] = {0};

/************** Private Functions ****************/
/* Define Functions to register class with GObject */
G_DEFINE_TYPE(GstMediaPlayer, gst_mediaplayer, GST_TYPE_ELEMENT)

/**
 * \brief Init Function for MediaPlayer Plugin
 * 
 * \param[in] p_plugin - pointer to plugin structure
 * 
 * \return gboolean - TRUE if no issues
 * \author Jason Neitzert
 */
static gboolean mediaplayer_plugin_init(GstPlugin *p_plugin)
{
    GST_DEBUG_CATEGORY_INIT(media_player_plugin_debug, "mediaplayer", 0, "Media Player Plugin Debug");

    return gst_element_register(p_plugin, "mediaplayer", GST_RANK_PRIMARY, GST_TYPE_MEDIA_PLAYER);
}

/**
 * \brief Class Init Function for MediaPlayer Plugin
 * 
 * \param[in] p_klass - pointer to mediaplayer plugin class structure
 * 
 * \return void
 * \author Jason Neitzert
 */
static void gst_mediaplayer_class_init(GstMediaPlayerClass *p_klass)
{
    GstElementClass *p_element_class = (GstElementClass*)p_klass;
    GObjectClass    *p_object_class  = (GObjectClass*)p_klass;
 
    /* Add signal for notification of selected GstMessages to user */
    /* When connecting provide function in following format */
    /* void (*message_callback) (GstElement *p_mediaplayer, GstMessage *p_message, gpointer p_user_data) */
    gst_mediaplayer_signals[SIGNAL_MESSAGE_CALLBACK] = g_signal_new("message-callback",
                                                                     GST_TYPE_MEDIA_PLAYER, G_SIGNAL_NO_HOOKS,
                                                                     0, NULL, NULL, NULL, G_TYPE_NONE, 1, GST_TYPE_MESSAGE);
    
    gst_element_class_set_static_metadata(p_element_class, 
                                         "Awesome Media Player",
                                         "Generic",
                                         "An awesome media player plugin",
                                         "Jason Neitzert <jwn_57030@yahoo.com>");

    
    p_element_class->change_state = gst_mediaplayer_change_state;    
}

/**
 * \brief Instance Init for mediaplayer class
 * 
 * \param[in] p_mediaplayer - pointer to instance structure
 * 
 * \return void
 * \author Jason Neitzert
 */
static void gst_mediaplayer_init(GstMediaPlayer *p_mediaplayer)
{   
}

/**
 * \brief Message Handler for Media Player
 * 
 * \param[in] p_data - generic pointer to mediaplayer plugin instance
 * 
 * \return gpointer - Generic Return value
 * \author Jason Neitzert
 */
static gpointer gst_mediaplayer_message_handler(gpointer p_data)
{
    GstMediaPlayer *p_mediaplayer = (GstMediaPlayer*)p_data;
    GstBus     *p_bus      = p_mediaplayer->p_bus;
    GstMessage *p_message  = NULL;
    GstState    new_state  = GST_STATE_NULL;
    gboolean    exitThread = FALSE;

    while ((!exitThread) &&
            (p_message = gst_bus_timed_pop_filtered(p_bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ELEMENT | 
                                                                                GST_MESSAGE_EOS)))
    {
        if (p_message->type == GST_MESSAGE_EOS)
        {
            g_signal_emit(p_mediaplayer, gst_mediaplayer_signals[SIGNAL_MESSAGE_CALLBACK], 0, p_message);
        }
        else if (p_message->type == GST_MESSAGE_ELEMENT)
        {
            if (gst_structure_has_name(gst_message_get_structure(p_message),"DestroyedPipeline"))
            {
                exitThread = TRUE;
            }
        }
        else if (G_OBJECT_TYPE(p_message->src) == GST_TYPE_MEDIA_PLAYER)
        {
            gst_message_parse_state_changed(p_message, NULL, &new_state, NULL);
            GST_ERROR("[MediaPlayer]%s", gst_element_state_get_name(new_state));
        }

        gst_message_unref(p_message);
    }

    GST_ERROR("Exiting Message Thread");

    gst_object_unref(p_bus);

    return NULL;
}

/**
 * \brief Change state of the media player
 * 
 * \param[in] p_element - element to change state on
 * \param[in] transition - state to transition
 * 
 * \return void
 * \author Jason Neitzert
 */
static GstStateChangeReturn gst_mediaplayer_change_state(GstElement *p_element,
                                                         GstStateChange transition)
{
    GstMediaPlayer       *p_mediaplayer    = (GstMediaPlayer*)p_element;
    GstStateChangeReturn  retval           = GST_STATE_CHANGE_SUCCESS;
    GstStateChangeReturn  change_state_ret = GST_STATE_CHANGE_SUCCESS; 

    switch (transition)
    {
        case GST_STATE_CHANGE_NULL_TO_READY:
        {
            p_mediaplayer->p_pipeline = gst_parse_launch("playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm",
                                                          NULL);

            p_mediaplayer->p_bus = gst_element_get_bus(p_mediaplayer->p_pipeline);
            gst_element_set_bus(p_element, p_mediaplayer->p_bus);

            /* Create Message handling thread */
            p_mediaplayer->p_msg_thread = g_thread_new(NULL, gst_mediaplayer_message_handler, p_mediaplayer);            

            /* Unreffing thread here as we will not be doing a join later. This allows shutdown
               to be quicker as we don't need to wait for thread join to exit. */
            g_thread_unref(p_mediaplayer->p_msg_thread);
            p_mediaplayer->p_msg_thread = NULL;

            retval = gst_element_set_state(p_mediaplayer->p_pipeline, GST_STATE_READY);
            break;
        } 

        case GST_STATE_CHANGE_READY_TO_PAUSED:
        {
            retval = gst_element_set_state(p_mediaplayer->p_pipeline, GST_STATE_PAUSED);
            break;
        }
        case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
        {
            retval = gst_element_set_state(p_mediaplayer->p_pipeline, GST_STATE_PLAYING);
            break;
        }
        default:
        {
            break;
        }
    }

    change_state_ret = GST_ELEMENT_CLASS(gst_mediaplayer_parent_class)->change_state(p_element, transition);

    switch (transition)
    {
        case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
        {
            
            retval = gst_element_set_state(p_mediaplayer->p_pipeline, GST_STATE_PAUSED);
            break;
        }
        case GST_STATE_CHANGE_PAUSED_TO_READY:
        {
            retval = gst_element_set_state(p_mediaplayer->p_pipeline, GST_STATE_READY);
            break;
        }
        case GST_STATE_CHANGE_READY_TO_NULL:
        {
            /* Send message to self to unblock message thread */
            gst_element_post_message((GstElement*)p_mediaplayer, 
                                     gst_message_new_element((GstObject*)p_mediaplayer, 
                                                             gst_structure_new_empty("DestroyedPipeline")));

            retval = gst_element_set_state(p_mediaplayer->p_pipeline, GST_STATE_NULL);

            /* We can keep bus from the pipeline we are about to destroy.  No harm in it. If this
               object is destroyed the bus will be automatically destroyed. If we go back to the 
               Ready state, the new bus will be replaced with new one, and old one will be automatically 
               unreffed */

            gst_object_unref(p_mediaplayer->p_pipeline);

            break;
        }
        default:
        {
            break;
        }
    }

    /* If the element class failed to change state make sure the retval is set
       correctly.  It is done this way as we want to make sure a failure from
       the element class is not overwritten with the return value from our state
       changes. */
    if ((GST_STATE_CHANGE_FAILURE == change_state_ret) || (GST_STATE_CHANGE_FAILURE == retval))
    {
        retval = GST_STATE_CHANGE_FAILURE;
    }
    else
    {
        /* For Now override async return values as we are not concerned about 
           it at the moment. This allows state change to continue without needing
           to report async state change complete */
        retval = GST_STATE_CHANGE_SUCCESS;
    }

    return retval;
}



