#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <string>
#include <fstream>

class Text
{
public:
	Text(void);
	~Text(void);

	void Init(std::string filename,Text list[]);
	std::string getText();
	void setText(std::string Text);
	int getStringsize(Text list[],int list_position);
	char getTextCharacter(Text list[],int list_position,int string_position);
	void Readfrom(std::string filename,Text list[]);
	void Writeinto(std::string filename,Text list[],int size);
protected:
	std::string TexT;
};

#endif