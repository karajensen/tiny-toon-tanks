////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SoundEngine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SoundEngine.h"
#include "Logger.h"

SoundEngine* SoundEngine::sm_engine = nullptr;

SoundEngine::SoundEngine()
{
    sm_engine = this;
 
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
        m_system->close();
        m_system->release();
    }
}

void SoundEngine::PlaySoundEffect(Sound ID)
{
    sm_engine->m_system->playSound(sm_engine->m_sounds[ID], 0, false, &sm_engine->m_sfxChannel);
}

void SoundEngine::PlayMusic(Sound ID)
{
    sm_engine->m_nextTrack = ID;

    if (sm_engine->m_currentTrack == NOTRACK)
    {
        sm_engine->PlayMusic();
    }
    else
    {
        sm_engine->m_shouldFade = true;
        sm_engine->m_fadeIn = false;
        sm_engine->m_musicChannel->getVolume(&sm_engine->m_volume);
    }
}

void SoundEngine::Update()
{
    CheckResult(m_system->update());

    if (m_shouldFade && m_musicChannel)
    {
        if (m_fadeIn)
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
            if (m_volume <= 0)
            {
                m_volume = 0;
                m_shouldFade = false;
                m_musicChannel->stop();
                PlayMusic();
            }
        }
    }
}

void SoundEngine::PlayMusic()
{
    if (m_nextTrack != NOTRACK)
    {
        m_currentTrack = m_nextTrack;
        m_nextTrack = NOTRACK;

        m_system->playSound(m_sounds[m_currentTrack], 0, false, &m_musicChannel);

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
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    CheckResult(result);

    unsigned int version;
    result = m_system->getVersion(&version);
    CheckResult(result);

    if (version < FMOD_VERSION)
    {
        LogError("FMOD lib version mismatch");
    }

    void* extradriverdata = 0;
    result = m_system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    CheckResult(result);
}
