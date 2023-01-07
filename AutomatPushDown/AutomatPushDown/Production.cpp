#include "Production.h"

Production::Production(std::string leftMember, std::string rightMember)
	:m_leftMember(leftMember), m_rightMember(rightMember)
{
	m_production = std::make_pair(m_leftMember, m_rightMember);
}

std::string Production::GetLeftMember()
{
	return m_leftMember;
}

std::string Production::GetRightMember()
{
	return m_rightMember;
}

std::pair<std::string, std::string> Production::GetProduction()
{
	return m_production;
}
