//==================================================================-
/*
/file   SoundManager.cpp
/author Jakob McFarland
/date   2/6/2019
/brief

This is the implementation file for all member functions
of the class SoundManager as specified in specification
file SoundManager.h.

*/
//==================================================================-

#include "stdafx.h"
#include "SoundManager.h"
#include <fmod.hpp>
#include  <fmod_errors.h>

// Constructor - initializes FMOD.
SoundManager::SoundManager()
	: BetaObject("SoundManager"), numSounds(0), numBanks(0), musicChannel(nullptr),
	effectsChannelGroup(nullptr), system(nullptr), studioSystem(nullptr),
	musicVolume(1.0f), effectsVolume(1.0f), audioFilePath("Audio/"), 
	bankFilePath("Banks/"), eventPrefix("event:/")
	 
{
	FMOD_Assert( FMOD::Studio::System::create( &studioSystem ) );
	FMOD_Assert( studioSystem->initialize( 4, FMOD_STUDIO_INIT_NORMAL, FMOD_STUDIO_INIT_NORMAL, nullptr) );

	FMOD_Assert( studioSystem->getLowLevelSystem( &system ) );
	FMOD_Assert( system->createChannelGroup( "SoundEffects", &effectsChannelGroup ) );
}

// Destructor
SoundManager::~SoundManager()
{
	FMOD_Assert( studioSystem->release() );
}

// Update the FMOD system.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	FMOD_Assert( studioSystem->update() );
}

// Shutdown the sound manager.
void SoundManager::Shutdown(void)
{
	for ( int i = 0; i < numSounds; ++i)
	{
		FMOD_Assert( soundList[i]->release() );
	}

	for (int i = 0; i < numBanks; ++i)
	{
		FMOD_Assert( bankList[i]->unload() );
	}

	numSounds = 0;
	numBanks = 0;
}

// Creates a non-looping FMOD sound.
// Params:
//	 filename = Name of the sound file (WAV).
void SoundManager::AddEffect(const std::string& filename)
{
	AddSound( filename, FMOD_DEFAULT );
}

// Creates an FMOD stream for a music file.
// Params:
//	 filename = Name of the music file (MP3).
void SoundManager::AddMusic(const std::string& filename)
{
	AddSound(filename, FMOD_LOOP_NORMAL);
}

// Creates an FMOD sound bank
// Params:
//   filename = Name of the FMOD bank file.
void SoundManager::AddBank(const std::string& filename)
{
	if (numBanks < maxNumBanks)
	{
		std::string filePath = "Assets/" + audioFilePath + bankFilePath + filename;

		FMOD::Studio::Bank* bank;

		FMOD_Assert(studioSystem->loadBankFile(filePath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));

		bankList[numBanks] = bank;

		++numBanks;
	}
}

// Plays a sound with the specified name.
// Params:
//	 name = The name of the sound to be played.
// Return:
//   The channel that was used to play the given sound.
FMOD::Channel* SoundManager::PlaySound(const std::string& name_)
{
	char buffer[128];

	FMOD_MODE mode;

	for (int i = 0; i < numSounds; ++i)
	{
		FMOD_Assert( soundList[i]->getName(buffer, 128) );

		if ( std::strcmp(name_.c_str(), buffer) == 0 )
		{
			FMOD_Assert( soundList[i]->getMode( &mode ) );

			if (mode & FMOD_CREATESTREAM)
			{
				return PlayMusic( soundList[i] );
			}
			else
			{
				return PlayEffect( soundList[i] );
			}
		}
	}

	return nullptr;
}

// Starts an audio event with the given name.
// The event must be in one of the loaded banks.
// Params:
//	 name = The name of the event that will be started.
FMOD::Studio::EventInstance* SoundManager::PlayEvent(const std::string& name_)
{
	std::string eventName = eventPrefix + name_;
	FMOD::Studio::EventDescription* eventDesc;
	FMOD::Studio::EventInstance* eventInst;

	FMOD_Assert( studioSystem->getEvent( eventName.c_str(), &eventDesc ) );
	FMOD_Assert( eventDesc->createInstance( &eventInst ) );

	FMOD_Assert( eventInst->start() );
	FMOD_Assert( eventInst->release() );

	return eventInst;
}

// Set the volume of the music channel.
// Params:
//	 volume = New value for the music volume (0.0 to 1.0).
void SoundManager::SetMusicVolume(float volume)
{
	musicVolume = volume;

	if (musicChannel != nullptr) FMOD_Assert( musicChannel->setVolume( musicVolume ) );
}

// Set the volume of the sound effects channel group.
// Params:
//	 volume = New value for the FX volume (0.0 to 1.0).
void SoundManager::SetEffectsVolume(float volume)
{
	effectsVolume = volume;

	FMOD_Assert( effectsChannelGroup->setVolume( effectsVolume ) );
}

// Get the volume of the music channel.
// Return:
//	 volume = Current value for the music volume.
float SoundManager::GetMusicVolume() const
{
	return musicVolume;
}

// Get the volume of the sound effects channel group.
// Return:
//	 volume = Current value for the FX volume.
float SoundManager::GetEffectsVolume() const
{
	return effectsVolume;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Different behaviors for SFX vs BGM
FMOD::Channel* SoundManager::PlayEffect(FMOD::Sound* sound) const
{
	FMOD::Channel* channel;

	system->playSound( sound, nullptr, false, &channel );

	return channel;
}

FMOD::Channel* SoundManager::PlayMusic(FMOD::Sound* sound)
{
	if (musicChannel != nullptr) FMOD_Assert( musicChannel->stop() );

	system->playSound(sound, nullptr, false, &musicChannel);

	return musicChannel;
}

// Add FX/BGM helper
void SoundManager::AddSound(const std::string& filename, FMOD_MODE mode)
{
	if (numSounds < maxNumSounds)
	{
		std::string filePath = "Assets/" + audioFilePath + filename;

		FMOD::Sound* sound;

		FMOD_Assert( system->createSound( filePath.c_str(), mode, nullptr, &sound) );

		soundList[numSounds] = sound;

		++numSounds;
	}
}

//handle FMOD function result
void SoundManager::FMOD_Assert(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
	}
}
