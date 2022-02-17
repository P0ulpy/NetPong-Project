#pragma once

#include <mutex>
#include "SFML/Graphics.hpp"

class UI
{
public:
	UI();
	void CreateText(std::string name, sf::Color colorText, std::string text, int characterSize, sf::Vector2f position, sf::Font& font);
	sf::Text& TextBox(const std::string& name);
	sf::Text& Text(const std::string& name);
	sf::RectangleShape& Zone(const std::string& name);
	void CreateTextBox(const std::string& name, sf::Font& font, int maxCharac, sf::Vector2f position, const std::string& textContent = "");
	bool InteractionButton(std::string name, sf::Vector2i mouseposition, bool _isclicked );
	bool InteractionTextBox(std::string name, sf::Vector2i mouseposition, bool _isclicked );
	void RenderTextBox(std::string name, sf::RenderTarget* renderTarget);

private:

    std::mutex _mutex;

    std::vector<sf::String> nameButton;
	std::vector<sf::Text> button;
	std::vector<sf::Text> text;

	sf::Text buttonModel;
	sf::Text textModel;
	sf::RectangleShape zoneTextModel;
	
	std::vector<sf::String> nameTextBox;
	std::vector<sf::RectangleShape> zone;
};