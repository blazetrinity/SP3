#include "Text.h"

Text::Text(): TexT("")
{
}

Text::~Text()
{
}


void Text::Writeinto(std::string filename,Text list[],int size)
{
	int i = 0;
	std::ofstream myFile(filename);
	if(myFile.is_open())
	{
		while(i < size)
		{
			myFile << list[i].getText() << std::endl;
			i++;
		}
		myFile.close();
	}
}
void Text::Readfrom(std::string filename,Text list[])
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
				list[i].setText(temp);
				i++;
			}
		}
		myFile.close();
	}
	
}
void Text::Init(std::string filename,Text list[])
{
	Text::Readfrom(filename,list);
}

int Text::getStringsize(Text list[],int list_position)
{
	std::string temp;
	temp = list[list_position].getText();
	return temp.size();
}

void Text::setText(std::string Text)
{
	this->TexT = Text;
}
char Text::getTextCharacter(Text list[],int list_position,int string_position)
{
	std::string temp;
	temp = list[list_position].getText();
	return temp[string_position];
}

std::string Text::getText()
{
	return this->TexT;
}