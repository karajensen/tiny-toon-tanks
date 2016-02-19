////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SoundEngine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "fmod/include/fmod.h"
#include "fmod/include/fmod.hpp"
#include <vector>

class SoundEngine
{
public:

    /**
    * Constructor
    **/
    SoundEngine();

    /**
    * Destructor
    **/
    ~SoundEngine();

    /**
    * Avaliable sounds to play
    **/
    enum Sound
    { 
        NOTRACK = -1,
        GAME = 0,
        CLICK, 
        SHOOT, 
        BANG, 
        EXPLODE, 
        WALLBANG, 
        NUMBER_OF_SOUNDS 
    };
    
    /**
    * Updates the engine
    */
    void Update();

    /**
    * Plays a sound effect
    * @param ID the ID of the sound
    */
    void PlaySoundEffect(Sound ID);

    /**
    * Plays a music track
    * @param ID the ID of the sound
    */
    void PlayMusic(Sound ID);

private:

    /**
    * Initialises the FMOD sound engine
    */
    void InitialiseFmod();

    /**
    * Logs an error if needed for FMOD
    */
    void CheckResult(FMOD_RESULT result);

    /**
    * Plays the next music track
    */
    void PlayMusic();

    FMOD::System* m_system = nullptr;            ///< FMOD System controller
    FMOD::Channel* m_musicChannel = nullptr;     ///< Current channel music is played on
	FMOD::Channel* m_sfxChannel = nullptr;       ///< Current channel sounds are played on
    Sound m_currentTrack = NOTRACK;              ///< The current music track playing
    Sound m_nextTrack = NOTRACK;                 ///< The next music track to play
    float m_maxVolume = 0.0f;                    ///< The maximum volume for the track
    float m_volume = 0.0f;                       ///< The current volume of the track
    float m_fadeSpeed = 10.0f;                   ///< The speed to fade between tracks
    bool m_shouldFade = true;                    ///< Whether the music requires fading
    bool m_fadeIn = true;                        ///< Whether to fade in or out
    bool m_playing = false;                      ///< Whether music is playing
    std::vector<FMOD::Sound*> m_sounds;          ///< All fmod sounds
};