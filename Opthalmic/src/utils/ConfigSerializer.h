#pragma once

#include "DAGGer/Core/Base.h"
#include <string>

#include <yaml-cpp/yaml.h>

class ConfigSerializer
{
public:
	ConfigSerializer() = default;

	void Serialize(const std::string& filepath);

	void Deserialize(const std::string& filepath);
private:
	static void SerializePreferences(YAML::Emitter& out);

	//	User Preferences
	bool m_ShowLibraryPanel = true;
	bool m_ShowPlaybackPanel = true;
	bool m_ShowKeyboard = true;
	int m_FirstKeyShown = 0;
	int m_LastKeyShown = 127;
	bool m_EnableVSync = true;

	//	Note colors
	std::array<DrVec4, 16> Colors =
	{
		DrColor(0.85f,  0.07f, 0.23f, 1.0f),	//	One.......Dark Red
		DrColor(1.0f,   0.5f,  0.31f, 1.0f),	//	Two.......Coral
		DrColor(1.0f,   0.26f, 0.0f,  1.0f),	//	Three.....Orange red
		DrColor(1.0f,   0.54f, 0.0f,  1.0f),	//	Four......Orange
		DrColor(0.85f,  0.52f, 0.12f, 1.0f),	//	Five......Golden rod
		DrColor(0.5f,   0.5f,  0.0f,  1.0f),	//	Six.......Olive
		DrColor(0.41f,  0.55f, 0.13f, 1.0f),	//	Seven.....Olive drab
		DrColor(0.13f,  0.54f, 0.13f, 1.0f),	//	Eight.....Forest Green
		DrColor(0.0f,   1.0f,  0.5f,  1.0f),	//	Nine......Spring Green
		DrColor(0.19f,  0.54f, 0.33f, 1.0f),	//	Ten.......Sea green
		DrColor(0.125f, 0.69f, 0.66f, 1.0f),	//	Eleven....Light sea green
		DrColor(0.0f,   0.5f,  0.5f,  1.0f),	//	Twelve....Teal
		DrColor(0.25f,  0.87f, 0.81f, 1.0f),	//	Thirteen..Turqoise
		DrColor(0.27f,  0.50f, 0.70f, 1.0f),	//	Fourteen..Steel blue
		DrColor(0.57f,  0.0f,  0.82f, 1.0f),	//	Fifteen...Dark violet
		DrColor(0.82f,  0.39f, 0.11f, 1.0f),	//	Sixteen...Chocolate
	};

	//	User Folders / files
	std::vector<std::string> m_Folders;
	std::vector<std::string> m_Files;
public:
	//	Getters for user settings
	bool GetLibraryPanel() const { return m_ShowLibraryPanel; }
	bool GetPlaybackPanel() const { return m_ShowPlaybackPanel; }
	bool GetKeyboard() const { return m_ShowKeyboard; }
	int GetFirstKey() const { return m_FirstKeyShown; }
	int GetLastKey() const { return m_LastKeyShown; }
	bool GetVSync() const { return m_EnableVSync; }

	//	Setters for user settings
	void SetLibraryPanel(bool enable) { m_ShowLibraryPanel = enable; }
	void SetPlaybackPanel(bool enable) { m_ShowPlaybackPanel = enable; }
	void SetKeyboard(bool enable) { m_ShowKeyboard = enable; }
	void SetFirstKey(int firstKey) { m_FirstKeyShown = firstKey; }
	void SetLastKey(int lastKey) { m_LastKeyShown = lastKey; }
	void SetVSync(bool enable) { m_EnableVSync = enable; }
};