
/**
* \file      test_app.c
* \details   test_app for Media Player Class
* \author    Jason Neitzert
* \date      8/22/2021
* \Copyright Jason Neitzert 
*/

/************************* Includes *************************/
#include <stdio.h>
#include <unistd.h>
#include <CUnit/Console.h>
#include <glib-2.0/glib.h>
#include "media_player_api.h"

/************************* Private Global Variables ***********/
static GCond  eos_cond;
static GMutex eos_mutex;

/************************* Private Functions ******************/
/**
 * \brief   An example for testing memory leaks
 * 
 * \return void
 * \author Jason Neitzert
 */
static void example_test_playback_memory()
{
    media_player_api_init();
    media_player_api_uninit();

    /* Get baseline for memory that doesn't ever get freed */
    
    media_player_api_init();

    /* Do test Runs */

    media_player_api_uninit();

    /* Figure out if any memory was leftover that shouldn't be */

    /* Log Leftover Memory analysis */

    /* Report Test as Success or Failure */   
    CU_PASS("Memory Test Passed"); 
}

static void media_player_message_callback(MpMessage message)
{
    g_mutex_lock(&eos_mutex);
    g_cond_signal(&eos_cond);
    g_mutex_unlock(&eos_mutex);
}

static MediaPlayer *test_create_mediaplayer()
{
    MediaPlayer *p_media_player = media_player_new(media_player_message_callback);

    CU_ASSERT_PTR_NOT_NULL(p_media_player);

    return p_media_player;
}

static bool test_media_player_play(MediaPlayer *p_media_player)
{
    bool played = false;

    CU_ASSERT(played = media_player_play(p_media_player));

    if (played)
    {
        /* Let playback play for a couple of seconds */
        sleep(5);
    }

    return played;
}

/**
 * \brief  Test Mediaplayer Play
 * 
 * \return void
 * \author Jason Neitzert
 */
static void unit_test_play()
{
    MediaPlayer *p_media_player = test_create_mediaplayer();

    if (p_media_player)
    {
        test_media_player_play(p_media_player);         
        media_player_destroy(p_media_player);
    }    
}

/**
 * \brief  Test Mediaplayer EOS
 * 
 * \return void
 * \author Jason Neitzert
 */
static void unit_test_eos()
{
    MediaPlayer *p_media_player = test_create_mediaplayer();

    if (p_media_player)
    {
        test_media_player_play(p_media_player);

        g_mutex_lock(&eos_mutex);
        if (!g_cond_wait_until(&eos_cond, &eos_mutex, g_get_monotonic_time() + (1 * G_TIME_SPAN_MINUTE)))
        {
            /* Wait time failed without getting eos signal */
            CU_FAIL("Failed to get EOS signal in time");                        
        }
        g_mutex_unlock(&eos_mutex);

        media_player_destroy(p_media_player);
    }   
}

/**
 * \brief  Test Mediaplayer Pause
 * 
 * \return void
 * \author Jason Neitzert
 */
static void unit_test_pause()
{
    MediaPlayer *p_media_player = test_create_mediaplayer();

    if (p_media_player)
    {
        if (test_media_player_play(p_media_player))
        {
            if (!media_player_pause(p_media_player))
            {
                CU_FAIL("Failed To Pause");
            }
            else
            {
                sleep(5);
            }

            test_media_player_play(p_media_player);
        }          

        media_player_destroy(p_media_player);
    }    
}


/************************* Public Functions ******************/

/**
 * \brief  Main(Need I say more)
 * 
 * \return int - value you would like process to return on exit
 * \author Jason Neitzert
 */
int main()
{
    CU_Suite *p_media_player_suite        = NULL;
    CU_Suite *p_media_player_memory_suite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
    {
        printf("\nFailed To Init CUnit");
    }
    else
    {
        /* All the tests here will use media player library */
        media_player_api_init();
        
        /* Add suite and tests for general playback */
        p_media_player_suite = CU_add_suite("media_player_tests", NULL, NULL);
        CU_add_test(p_media_player_suite, "Playback", unit_test_play);
        CU_add_test(p_media_player_suite, "Pause", unit_test_pause);
        CU_add_test(p_media_player_suite, "EOS", unit_test_eos);

        /* Add suite and tests for memory testing */
        p_media_player_memory_suite = CU_add_suite("media_player_memory_tests", NULL, NULL);
        CU_add_test(p_media_player_memory_suite, "Playback Memory", example_test_playback_memory);

        CU_console_run_tests();

        CU_cleanup_registry();
    }

    return 0;
   
}