#include "UI.hpp"

UI::UI()
{
	
}

void UI::CreateText(std::string name, sf::Color colorText, std::string text, int characterSize, sf::Vector2f position, sf::Font& font)
{
	buttonModel.setFont(font);
	buttonModel.setFillColor(colorText);
	buttonModel.setString(text);
	buttonModel.setCharacterSize(characterSize);
	buttonModel.setOrigin(buttonModel.getGlobalBounds().width / 2, buttonModel.getGlobalBounds().height / 2);
	buttonModel.setPosition(position);

	button.push_back(buttonModel);
	nameButton.push_back(name);
}

sf::Text& UI::Text(std::string name)
{
	for (int i = 0; i < nameButton.size(); i++)
	{
		if (nameButton[i] == name)
		{
			return button.at(i);
		}
	}
}

sf::Text& UI::TextBox(std::string name)
{
	for (int i = 0; i < nameTextBox.size(); i++)
	{
		if (nameTextBox[i] == name)
		{
			return text.at(i);
		}
	}
	
}



sf::RectangleShape& UI::Zone(std::string name)
{
	for (int i = 0; i < nameTextBox.size(); i++)
	{
		if (nameTextBox[i] == name)
		{
			return zone.at(i);
		}
	}
}

void UI::CreateTextBox(std::string name, sf::Font& font, int maxCharac, sf::Vector2f position)
{
	zoneTextModel.setFillColor(sf::Color(100, 100, 100));
	zoneTextModel.setSize(sf::Vector2f(((maxCharac+1)*30), 100));
	zoneTextModel.setOrigin(zoneTextModel.getGlobalBounds().width / 2, zoneTextModel.getGlobalBounds().height / 2);
	zoneTextModel.setPosition(position);

	textModel.setFont(font);
	textModel.setFillColor(sf::Color::Black);
	textModel.setString("");
	textModel.setCharacterSize(76);
	textModel.setOrigin(zoneTextModel.getGlobalBounds().width / 2, zoneTextModel.getGlobalBounds().height / 2);
	textModel.setPosition(zoneTextModel.getPosition().x, zoneTextModel.getPosition().y);
	
	zone.push_back(zoneTextModel);
	text.push_back(textModel);
	nameTextBox.push_back(name);


}

void UI::RenderTextBox(std::string name, sf::RenderTarget* renderTarget)
{
	renderTarget->draw(Zone(name));
	renderTarget->draw(TextBox(name));
}



bool UI::InteractionButton(std::string name,sf::Vector2i mouseposition,bool _isclicked)
{
	if (Text(name).getGlobalBounds().contains(mouseposition.x,mouseposition.y))
	{
		Text(name).setColor(sf::Color::Red);
		if (_isclicked) return true;
		else return false;
	}
	else
	{
		Text(name).setColor(sf::Color::White);
		return false;
	}
	
}

bool UI::InteractionTextBox(std::string name, sf::Vector2i mouseposition, bool _isclicked)
{
	if (Zone(name).getGlobalBounds().contains(mouseposition.x, mouseposition.y))
	{
		if (_isclicked) 
		{
			Zone(name).setFillColor(sf::Color::White);
			return true;
		}
		else return false;
	}
	else
	{
		if (_isclicked)
		{
			Zone(name).setFillColor(sf::Color(100,100,100));
		}
		return false;
	}
}

