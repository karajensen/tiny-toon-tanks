////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SoundEngine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SoundEngine.h"
#include "common.h"

SoundEngine::SoundEngine()
{
    InitialiseFmod();

    m_sounds.resize(NUMBER_OF_SOUNDS);
    m_sounds.assign(NUMBER_OF_SOUNDS, nullptr);

    m_system->createStream("./resources/Click.wav", FMOD_DEFAULT, 0, &m_sounds[CLICK]);
    m_system->createStream("./resources/Shoot.wav", FMOD_DEFAULT, 0, &m_sounds[SHOOT]);
    m_system->createStream("./resources/Explode.wav", FMOD_DEFAULT, 0, &m_sounds[EXPLODE]);
    m_system->createStream("./resources/Hit.wav", FMOD_DEFAULT, 0, &m_sounds[WALLBANG]);
    m_system->createStream("./resources/WallHit.wav", FMOD_DEFAULT, 0, &m_sounds[BANG]);
    m_system->createStream("./resources/music.mp3", FMOD_LOOP_NORMAL, 0, &m_sounds[GAME]);
}

SoundEngine::~SoundEngine()
{
    for (auto* sound : m_sounds)
    {
        if (sound)
        {
            sound->release();
        }
    }

    if(m_system)
    {
        m_system->release();
    }
}

void SoundEngine::PlaySoundEffect(Sound ID)
{
    m_system->playSound(FMOD_CHANNEL_FREE, m_sounds[ID], false, 0);
}

void SoundEngine::PlayMusic(Sound ID)
{
    m_nextTrack = ID;

    if (m_currentTrack == NOTRACK)
    {
        PlayMusic();
    }
    else
    {
        m_shouldFade = true;
        m_fadeIn = false;
        m_musicChannel->getVolume(&m_volume);
    }
}

void SoundEngine::FadeMusic()
{
    if (!m_shouldFade)
    {
        return;
    }

    if(m_fadeIn)
    {
        m_volume += m_fadeSpeed;
        if (m_volume >= m_maxVolume)
        {
            m_volume = m_maxVolume;
            m_shouldFade = false;
        }
        m_musicChannel->setVolume(m_volume);
    }
    else
    {
        m_volume -= m_fadeSpeed;
        m_musicChannel->setVolume(m_volume);
        if(m_volume <= 0)
        {
            m_volume = 0;
            m_shouldFade = false;
            m_musicChannel->stop();
            PlayMusic();
        }
    }
}

void SoundEngine::PlayMusic()
{
    if (m_nextTrack != NOTRACK)
    {
        m_currentTrack = m_nextTrack;
        m_nextTrack = NOTRACK;

        m_system->playSound(FMOD_CHANNEL_FREE, 
            m_sounds[m_currentTrack], false, &m_musicChannel);

        m_musicChannel->isPlaying(&m_playing);
        m_musicChannel->getVolume(&m_maxVolume);
        m_musicChannel->setVolume(0);
        m_fadeIn = true;
        m_shouldFade = true;
        m_volume = 0;
    }
}

void SoundEngine::CheckResult(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        LogError("FMOD Error Occured: " + std::to_string(result));
    }
}

void SoundEngine::InitialiseFmod()
{
    unsigned int version;
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    const int bufferSize = 256;
    char name[bufferSize];

    //Create a System object and initialize.
    CheckResult(FMOD::System_Create(&m_system));
    CheckResult(m_system->getVersion(&version));

    if (version < FMOD_VERSION)
    {
        LogError("Error! You are using an old version of FMOD " +
            std::to_string(version) + " this program requires " +
            std::to_string(FMOD_VERSION));
    }

    CheckResult(m_system->getNumDrivers(&numdrivers));

    if (numdrivers == 0)
    {
        CheckResult(m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
    }
    else
    {
        CheckResult(m_system->getDriverCaps(0, &caps, 0, &speakermode));

        //Set the user selected speaker mode.
        CheckResult(m_system->setSpeakerMode(speakermode));
        if (caps & FMOD_CAPS_HARDWARE_EMULATED)
        {
            //The user has the 'Acceleration' slider set to off. This is really bad for latency.
            CheckResult(m_system->setDSPBufferSize(1024, 10));
        }
        CheckResult(m_system->getDriverInfo(0, name, bufferSize, 0));

        if (strstr(name, "SigmaTel"))
        {
            //Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
            //PCM floating point output seems to solve it.
            CheckResult(m_system->setSoftwareFormat(48000, 
                FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR));
        }
    }
    FMOD_RESULT result = m_system->init(100, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
        //The speaker mode selected isn't supported by this soundcard. Switch it back to stereo.
        CheckResult(m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));
        CheckResult(m_system->init(100, FMOD_INIT_NORMAL, 0));
    }
    else
    {
        CheckResult(result);
    }
}
