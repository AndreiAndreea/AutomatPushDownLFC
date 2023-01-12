#pragma once
#include <string>
#include <utility>
class Production
{
public:
	Production(std::string leftMember, std::string rightMember);

	void SetRightMember(std::string rightMember);

	std::string GetLeftMember();
	std::string GetRightMember();
	std::pair<std::string, std::string> GetProduction();
	
	bool operator==(const Production& other) const
	{
		return (m_production.first == other.m_production.first) && (m_production.second == other.m_production.second);
	}

private:
	std::string m_leftMember;
	std::string m_rightMember;
	std::pair<std::string, std::string> m_production;
};