#pragma once
#include <string>
#include <utility>
class Production
{
public:
	Production(std::string leftMember, std::string rightMember);

	std::string GetLeftMember();
	std::string GetRightMember();
	std::pair<std::string, std::string> GetProduction();

private:
	std::string m_leftMember;
	std::string m_rightMember;
	std::pair<std::string, std::string> m_production;
};