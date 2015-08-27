#include "Text.h"

Text::Text()
{
}

Text::~Text()
{
}


void Text::Writeinto(std::string filename,int size)
{
	int i = 0;
	std::ofstream myFile(filename);
	if(myFile.is_open())
	{
		while(i < m_texts.size())
		{
			myFile << m_texts[i] << std::endl;
			i++;
		}
		myFile.close();
	}
}
void Text::Readfrom(std::string filename)
{
	int i = 0;
	std::ifstream myFile(filename);
	std::string temp;
	if (myFile.is_open())
	{ 
		while(!myFile.eof())
		{
			getline(myFile,temp);
			if(temp == "~")
				break;
			else
			{
				m_texts.push_back(temp);
				i++;
			}
		}
		myFile.close();
	}
}
void Text::Init(std::string filename)
{
	Text::Readfrom(filename);
}

int Text::getStringsize(int list_position)
{
	std::string temp;
	return m_texts[list_position].size();
}

char Text::getTextCharacter(int list_position,int string_position)
{
	return m_texts[list_position][string_position];	
}