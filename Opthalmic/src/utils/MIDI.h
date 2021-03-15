#pragma once

#include <fstream>
#include <array>
#include <vector>
#include <list>
#include <string>

extern std::vector <std::string> dHistory;


namespace rMIDI
{
	struct MidiEvent
	{
		enum class Type
		{
			NoteOff,
			NoteOn,
			Other
		} event;

		uint8_t nKey = 0;
		uint8_t nVelocity = 0;
		uint32_t nDeltaTick = 0;
	};


	struct MidiNote
	{
		uint8_t nKey = 0;
		uint8_t nVelocity = 0;
		uint32_t nStartTime = 0;
		uint32_t nDuration = 0;
	};

	struct MidiTrack
	{
		std::string sName;
		std::string sInstrument;
		std::vector<MidiEvent> vecEvents;
		std::vector<MidiNote> vecNotes;
		uint8_t nMaxNote = 64;
		uint8_t nMinNote = 64;
	};


	class MidiFile
	{
	public:
		enum EventName : uint8_t
		{
			VoiceNoteOff = 0x80,
			VoiceNoteOn = 0x90,
			VoiceAftertouch = 0xA0,
			VoiceControlChange = 0xB0,
			VoiceProgramChange = 0xC0,
			VoiceChannelPressure = 0xD0,
			VoicePitchBend = 0xE0,
			SystemExclusive = 0xF0,
		};

		enum MetaEventName : uint8_t	//	Read file Metadata
		{
			MetaSequence = 0x00,
			MetaText = 0x01,
			MetaCopyright = 0x02,
			MetaTrackName = 0x03,
			MetaInstrumentName = 0x04,
			MetaLyrics = 0x05,
			MetaMarker = 0x06,
			MetaCuePoint = 0x07,
			MetaChannelPrefix = 0x20,
			MetaEndOfTrack = 0x2F,
			MetaSetTempo = 0x51,
			MetaSMPTEOffset = 0x54,
			MetaTimeSignature = 0x58,
			MetaKeySignature = 0x59,
			MetaSequencerSpecific = 0x7F,
		};

	public:
		MidiFile()
		{
		}

		MidiFile(const std::string& sFileName)
		{
			ParseFile(sFileName);
		}

		void Clear()
		{

		}

		bool ParseFile(const std::string& sFileName)
		{
			// Open the MIDI File as a stream
			std::ifstream ifs;
			ifs.open(sFileName, std::fstream::in | std::ios::binary);
			if (!ifs.is_open())
				return false;

			// Swaps byte order of 32-bit integer
			auto Swap32 = [](uint32_t n)
			{
				return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) | ((n << 24) & 0xff000000));
			};

			// Swaps byte order of 16-bit integer
			auto Swap16 = [](uint16_t n)
			{
				return ((n >> 8) | (n << 8));
			};

			// Reads nLength bytes form file stream, and constructs a text string
			auto ReadString = [&ifs](uint32_t nLength)
			{
				std::string s;
				for (uint32_t i = 0; i < nLength; i++) s += ifs.get();
				return s;
			};

			// Reads a compressed MIDI value. This can be up to 32 bits long. Essentially if the first byte, first
			// bit is set to 1, that indicates that the next byte is required to construct the full word. Only
			// the bottom 7 bits of each byte are used to construct the final word value. Each successive byte 
			// that has MSB set, indicates a further byte needs to be read.
			auto ReadValue = [&ifs]()
			{
				uint32_t nValue = 0;
				uint8_t nByte = 0;

				// Read byte
				nValue = ifs.get();

				// Check MSB, if set, more bytes need reading
				if (nValue & 0x80)
				{
					// Extract bottom 7 bits of read byte
					nValue &= 0x7F;
					do
					{
						// Read next byte
						nByte = ifs.get();

						// Construct value by setting bottom 7 bits, then shifting 7 bits
						nValue = (nValue << 7) | (nByte & 0x7F);
					} while (nByte & 0x80); // Loop whilst read byte MSB is 1
				}
				// Return final construction (always 32-bit unsigned integer internally)
				return nValue;
			};

			uint32_t n32 = 0;
			uint16_t n16 = 0;

			// Read MIDI Header (Fixed Size)
			ifs.read((char*)&n32, sizeof(uint32_t));	//	Read first four bytes (It's a MIDI file)
			uint32_t nFileID = Swap32(n32);
			ifs.read((char*)&n32, sizeof(uint32_t));	//	Read next four bytes (length of header)
			uint32_t nHeaderLength = Swap32(n32);
			ifs.read((char*)&n16, sizeof(uint16_t));	//	Read next two bytes (format of MIDI file)
			uint16_t nFormat = Swap16(n16);
			ifs.read((char*)&n16, sizeof(uint16_t));	//	Read next two bytes (number of track chunks) ******
			uint16_t nTrackChunks = Swap16(n16);
			ifs.read((char*)&n16, sizeof(uint16_t));	//	Read next two bytes
			uint16_t nDivision = Swap16(n16);

			for (uint16_t nChunk = 0; nChunk < nTrackChunks; nChunk++)
			{
				dHistory.push_back("===== NEW TRACK =====" );
				// Read Track Header
				ifs.read((char*)&n32, sizeof(uint32_t));	//	Read track ID
				uint32_t nTrackID = Swap32(n32);
				ifs.read((char*)&n32, sizeof(uint32_t));	//	Read how many bytes are in the track (lenth)
				uint32_t nTrackLength = Swap32(n32);

				bool bEndOfTrack = false;

				vecTracks.push_back(MidiTrack());

				uint32_t nWallTime = 0;

				uint8_t nPreviousStatus = 0;

				while (!ifs.eof() && !bEndOfTrack)
				{
					// Fundamentally all MIDI Events contain a timecode, and a status byte*
					uint32_t nStatusTimeDelta = 0;
					uint8_t nStatus = 0;


					// Read Timecode from MIDI stream. This could be variable in length
					// and is the delta in "ticks" from the previous event. Of course this value
					// could be 0 if two events happen simultaneously.
					nStatusTimeDelta = ReadValue(); // Read Timecode

					// Read first byte of message, this could be the status byte, or it could not...
					nStatus = ifs.get();

					// All MIDI Status events have the MSB set. The data within a standard MIDI event
					// does not. A crude yet utilised form of compression is to omit sending status
					// bytes if the following sequence of events all refer to the same MIDI Status.
					// This is called MIDI Running Status, and is essential to succesful decoding of
					// MIDI streams and files.
					//
					// If the MSB of the read byte was not set, and on the whole we were expecting a
					// status byte, then Running Status is in effect, so we refer to the previous 
					// confirmed status byte.
					if (nStatus < 0x80)
					{
						// MIDI Running Status is happening, so refer to previous valid MIDI Status byte
						nStatus = nPreviousStatus;

						// We had to read the byte to assess if MIDI Running Status is in effect. But!
						// that read removed the byte form the stream, and that will desync all of the 
						// following code because normally we would have read a status byte, but instead
						// we have read the data contained within a MIDI message. The simple solution is 
						// to put the byte back :P
						ifs.seekg(-1, std::ios_base::cur);
					}



					if ((nStatus & 0xF0) == EventName::VoiceNoteOff)
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;
						uint8_t nNoteID = ifs.get();
						uint8_t nNoteVelocity = ifs.get();
						vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::NoteOff, nNoteID, nNoteVelocity, nStatusTimeDelta });
					}

					else if ((nStatus & 0xF0) == EventName::VoiceNoteOn)	//	Note has been pressed
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;	//	Which channel
						uint8_t nNoteID = ifs.get();		//	Which note has been pressed
						uint8_t nNoteVelocity = ifs.get();	//	What is the note velocity
						if (nNoteVelocity == 0)
							vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::NoteOff, nNoteID, nNoteVelocity, nStatusTimeDelta });
						else
							vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::NoteOn, nNoteID, nNoteVelocity, nStatusTimeDelta });
					}

					else if ((nStatus & 0xF0) == EventName::VoiceAftertouch)
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;
						uint8_t nNoteID = ifs.get();
						uint8_t nNoteVelocity = ifs.get();
						vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
					}

					else if ((nStatus & 0xF0) == EventName::VoiceControlChange)
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;
						uint8_t nControlID = ifs.get();
						uint8_t nControlValue = ifs.get();
						vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
					}

					else if ((nStatus & 0xF0) == EventName::VoiceProgramChange)
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;
						uint8_t nProgramID = ifs.get();
						vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
					}

					else if ((nStatus & 0xF0) == EventName::VoiceChannelPressure)
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;
						uint8_t nChannelPressure = ifs.get();
						vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
					}

					else if ((nStatus & 0xF0) == EventName::VoicePitchBend)
					{
						nPreviousStatus = nStatus;
						uint8_t nChannel = nStatus & 0x0F;
						uint8_t nLS7B = ifs.get();
						uint8_t nMS7B = ifs.get();
						vecTracks[nChunk].vecEvents.push_back({ MidiEvent::Type::Other });
					}

					else if ((nStatus & 0xF0) == EventName::SystemExclusive)
					{
						nPreviousStatus = 0;

						if (nStatus == 0xFF)	//	Midi event is a meta message
						{
							uint8_t nType = ifs.get();
							uint8_t nLength = ReadValue();

							switch (nType)
							{
							case MetaSequence:
								dHistory.push_back("Sequence Number: " + ifs.get() + ifs.get());
								break;
							case MetaText:
								dHistory.push_back("Text: " + ReadString(nLength));
								break;
							case MetaCopyright:
								dHistory.push_back("Copyright: " + ReadString(nLength));
								break;
							case MetaTrackName:
								vecTracks[nChunk].sName = ReadString(nLength);
								dHistory.push_back("Track Name: " + vecTracks[nChunk].sName);
								break;
							case MetaInstrumentName:
								vecTracks[nChunk].sInstrument = ReadString(nLength);
								dHistory.push_back("Instrument Name: " + vecTracks[nChunk].sInstrument);
								break;
							case MetaLyrics:
								dHistory.push_back("Lyrics: " + ReadString(nLength));
								break;
							case MetaMarker:
								dHistory.push_back("Marker: " + ReadString(nLength));
								break;
							case MetaCuePoint:
								dHistory.push_back("Cue: " + ReadString(nLength));
								break;
							case MetaChannelPrefix:
								dHistory.push_back("Prefix: " + ifs.get());
								break;
							case MetaEndOfTrack:
								bEndOfTrack = true;
								break;
							case MetaSetTempo:
								// Tempo is in microseconds per quarter note	
								if (m_nTempo == 0)
								{
									(m_nTempo |= (ifs.get() << 16));
									(m_nTempo |= (ifs.get() << 8));
									(m_nTempo |= (ifs.get() << 0));
									m_nBPM = (60000000 / m_nTempo);
									dHistory.push_back("Tempo: " + std::to_string(m_nTempo) + " (" + std::to_string(m_nBPM) + "bpm)" );
								}
								break;
							case MetaSMPTEOffset:
								dHistory.push_back("SMPTE: H:" + std::to_string(ifs.get()) + " M:" + std::to_string(ifs.get()) + " S:" + std::to_string(ifs.get()) + " FR:" + std::to_string(ifs.get()) + " FF:" + std::to_string(ifs.get()) );
								break;
							case MetaTimeSignature:
								dHistory.push_back("Time Signature: " + std::to_string(ifs.get()) + " / " + std::to_string((2 << ifs.get())) );
								dHistory.push_back("ClocksPerTick: " + ifs.get() );

								// A MIDI "Beat" is 24 ticks, so specify how many 32nd notes constitute a beat
								dHistory.push_back("32per24Clocks: " + ifs.get() );
								break;
							case MetaKeySignature:
								dHistory.push_back("Key Signature: " + ifs.get() );
								dHistory.push_back("Minor Key: " + ifs.get() );
								break;
							case MetaSequencerSpecific:
								dHistory.push_back("Sequencer Specific: " + ReadString(nLength) );
								break;
							default:
								dHistory.push_back("Unrecognised MetaEvent: " + nType );
							}
						}

						if (nStatus == 0xF0)
						{
							// System Exclusive Message Begin
							dHistory.push_back("System Exclusive Begin: " + ReadString(ReadValue()) );
						}

						if (nStatus == 0xF7)
						{
							// System Exclusive Message Begin
							dHistory.push_back("System Exclusive End: " + ReadString(ReadValue()) );
						}
					}
					else
					{
						dHistory.push_back("Unrecognised Status Byte: " + nStatus );
					}
				}
			}


			// Convert Time Events to Notes
			for (auto& track : vecTracks)
			{
				uint32_t nWallTime = 0;

				std::list<MidiNote> listNotesBeingProcessed;

				for (auto& event : track.vecEvents)
				{
					nWallTime += event.nDeltaTick;

					if (event.event == MidiEvent::Type::NoteOn)
					{
						// New Note
						listNotesBeingProcessed.push_back({ event.nKey, event.nVelocity, nWallTime, 0 });
					}

					if (event.event == MidiEvent::Type::NoteOff)
					{
						auto note = std::find_if(listNotesBeingProcessed.begin(), listNotesBeingProcessed.end(), [&](const MidiNote& n) { return n.nKey == event.nKey; });
						if (note != listNotesBeingProcessed.end())
						{
							note->nDuration = nWallTime - note->nStartTime;
							track.vecNotes.push_back(*note);
							track.nMinNote = std::min(track.nMinNote, note->nKey);
							track.nMaxNote = std::max(track.nMaxNote, note->nKey);
							listNotesBeingProcessed.erase(note);
						}
					}
				}
			}

			return true;
		}

	public:
		std::vector<MidiTrack> vecTracks;
		uint32_t m_nTempo = 0;
		uint32_t m_nBPM = 0;

	};

	class MIDIViewer
	{

	};

	/*
	class olcMIDIViewer : public olc::PixelGameEngine
	{
	public:
		olcMIDIViewer()
		{
			sAppName = "MIDI File Viewer";
		}

		MidiFile midi;

		//HMIDIOUT hInstrument;
		size_t nCurrentNote[16]{ 0 };

		double dSongTime = 0.0;
		double dRunTime = 0.0;
		uint32_t nMidiClock = 0;


	public:
		bool OnUserCreate() override
		{

			midi.ParseFile("ff7_battle.mid");

			/*
			int nMidiDevices = midiOutGetNumDevs();
			if (nMidiDevices > 0)
			{
				if (midiOutOpen(&hInstrument, 2, NULL, 0, NULL) == MMSYSERR_NOERROR)
				{
					dHistory.push_back("Opened midi" );
				}
			}


	return true;
		}

		float nTrackOffset = 1000;

		bool OnUserUpdate(float fElapsedTime) override
		{
			Clear(olc::BLACK);
			uint32_t nTimePerColumn = 50;
			uint32_t nNoteHeight = 2;
			uint32_t nOffsetY = 0;

			if (GetKey(olc::Key::LEFT).bHeld) nTrackOffset -= 10000.0f * fElapsedTime;
			if (GetKey(olc::Key::RIGHT).bHeld) nTrackOffset += 10000.0f * fElapsedTime;


			for (auto& track : midi.vecTracks)
			{
				if (!track.vecNotes.empty())
				{
					uint32_t nNoteRange = track.nMaxNote - track.nMinNote;

					FillRect(0, nOffsetY, ScreenWidth(), (nNoteRange + 1) * nNoteHeight, olc::DARK_GREY);
					DrawString(1, nOffsetY + 1, track.sName);

					for (auto& note : track.vecNotes)
					{
						FillRect((note.nStartTime - nTrackOffset) / nTimePerColumn, (nNoteRange - (note.nKey - track.nMinNote)) * nNoteHeight + nOffsetY, note.nDuration / nTimePerColumn, nNoteHeight, olc::WHITE);
					}

					nOffsetY += (nNoteRange + 1) * nNoteHeight + 4;
				}
			}
			return true;
		}


	};

	int main()
	{
		olcMIDIViewer demo;
		if (demo.Construct(1280, 960, 1, 1))
			demo.Start();
		return 0;
	}

	*/
}