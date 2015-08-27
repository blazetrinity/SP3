#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using std::vector;

class Text
{
public:
	Text(void);
	~Text(void);

	void Init(std::string filename);
	std::string getText();
	void setText(std::string Text);
	int getStringsize(int list_position);
	char getTextCharacter(int list_position,int string_position);
	void Readfrom(std::string filename);
	void Writeinto(std::string filename, int size);
protected:
	vector<std::string> m_texts;
};

#endif