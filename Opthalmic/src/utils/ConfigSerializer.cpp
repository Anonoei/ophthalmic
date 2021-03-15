#include "ConfigSerializer.h"

#include <glm/glm.hpp>


namespace YAML
{
	template<>
	struct convert<glm::vec4>	//	VEC4
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}	//	END namespace YAML

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)	//	VEC4
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

void ConfigSerializer::SerializePreferences(YAML::Emitter& out)
{
	out << YAML::BeginMap;	//	Visual
	out << YAML::Key << "Library Panel" << YAML::Value << GetLibraryPanel();
}

void ConfigSerializer::Serialize(const std::string& filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Opthalmic";
	out << YAML::Key << "Preferences" << YAML::Value << YAML::BeginSeq;
	SerializePreferences(out);
}
