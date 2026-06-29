#pragma once
#include <JuceHeader.h>

class ToggleSwitchLookAndFeel : public juce::LookAndFeel_V4 
{
    public:
         void drawToggleButton (Graphics& g, ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override 
        {
            auto bounds = button.getLocalBounds().toFloat();
            bool isOn = button.getToggleState();
            float margine = 3.0f;
            float diametro = bounds.getHeight() - margine * 2.0f;
            float circleY = bounds.getY() + margine;

            g.setColour(Colour(96,96,96));
            g.fillRoundedRectangle(bounds, bounds.getHeight() * 0.5f);

            float circleX;
            if (!isOn)
                circleX = bounds.getX() + margine;
            else
                circleX = bounds.getRight() - diametro - margine;

            g.setColour(Colour(192,192,192));
            g.fillEllipse(circleX, circleY, diametro, diametro); 
        }
    private:
};