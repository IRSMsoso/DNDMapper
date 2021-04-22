#pragma once
#include <string>

class Resource {
public:

	virtual ~Resource() {};

	void setResourceID(std::string resourceID) { m_resourceID = resourceID; }
	std::string getResourceID() { return m_resourceID; }


private:
	std::string m_resourceID;
};

