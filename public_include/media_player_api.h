/**
* \file      media_player_api.h
* \details   Media Player API Definition
* \author    Jason Neitzert
* \date      8/22/2021
* \Copyright Jason Neitzert 
*/

#ifndef MEDIA_PLAYER_H
/***************** Includes *******************************************/
#include <stdbool.h>

/***************** Defines ********************************************/

/************************* Structures and Enums ***********************/
/* Messages Player can Emit */
typedef enum
{
    eMP_EOS /* End of Stream */
} MpMessage;

/***************** Types **********************************************/
typedef struct MediaPlayer MediaPlayer;

/* Definition of callback used for message handling. */
typedef void (*MpMessageCallback)(MpMessage message);

/***************** Public Functions ***********************************/
void media_player_api_init();
void media_player_api_uninit();
MediaPlayer *media_player_new(MpMessageCallback mp_message_callback);
void media_player_destroy(MediaPlayer *p_media_player);

bool media_player_play(MediaPlayer *p_media_player);
bool media_player_pause(MediaPlayer *p_media_player);
#endif
