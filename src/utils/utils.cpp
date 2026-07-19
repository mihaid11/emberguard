#include "utils.h"
#include <sstream>

std::vector<sf::String> wrapText(const sf::String& text, const sf::Font& font, unsigned int characterSize, float boxWidth) {
    std::vector<sf::String> lines;
    sf::String currentLine;
    std::istringstream wordStream(text.toAnsiString());
    std::string word;

    while (wordStream >> word) {
        // Measure the size of the current line with the new word appended
        sf::Text tempText(currentLine + (currentLine.isEmpty() ? "" : " ") + word, font, characterSize);
        float lineWidth = tempText.getLocalBounds().width;

        if (lineWidth > boxWidth) {
            // If the current line with the new word exceeds the box width, store the current line and start a new one
            lines.push_back(currentLine);
            currentLine = word;
        }
        else {
            if (!currentLine.isEmpty()) {
                currentLine += " ";
            }
            currentLine += word;
        }
    }

    // Add the last line to the result
    if (!currentLine.isEmpty()) {
        lines.push_back(currentLine);
    }

    return lines;
}
