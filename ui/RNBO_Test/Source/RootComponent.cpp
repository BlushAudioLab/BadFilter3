/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "RootComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
RootComponent::RootComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    juce__slider.reset (new juce::Slider ("cutoff slider"));
    addAndMakeVisible (juce__slider.get());
    juce__slider->setRange (0, 1200, 0);
    juce__slider->setSliderStyle (juce::Slider::LinearHorizontal);
    juce__slider->setTextBoxStyle (juce::Slider::TextBoxLeft, true, 80, 20);
    juce__slider->setColour (juce::Slider::thumbColourId, juce::Colour (0xfff25e5e));
    juce__slider->addListener (this);

    juce__slider->setBounds (24, 56, 150, 24);

    juce__slider2.reset (new juce::Slider ("resonance slider"));
    addAndMakeVisible (juce__slider2.get());
    juce__slider2->setRange (0, 1, 0);
    juce__slider2->setSliderStyle (juce::Slider::LinearHorizontal);
    juce__slider2->setTextBoxStyle (juce::Slider::TextBoxLeft, true, 80, 20);
    juce__slider2->setColour (juce::Slider::thumbColourId, juce::Colour (0x5efff25e));
    juce__slider2->addListener (this);

    juce__slider2->setBounds (24, 88, 150, 24);

    juce__label.reset (new juce::Label ("Bad Filter Label",
                                        TRANS("Bad Filter")));
    addAndMakeVisible (juce__label.get());
    juce__label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label->setJustificationType (juce::Justification::centredLeft);
    juce__label->setEditable (false, false, false);
    juce__label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label->setBounds (16, 24, 150, 24);

    juce__label2.reset (new juce::Label ("new label",
                                         TRANS("Cutoff")));
    addAndMakeVisible (juce__label2.get());
    juce__label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label2->setJustificationType (juce::Justification::centredLeft);
    juce__label2->setEditable (false, false, false);
    juce__label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label2->setBounds (184, 56, 56, 24);

    juce__label3.reset (new juce::Label ("new label",
                                         TRANS("Resonance")));
    addAndMakeVisible (juce__label3.get());
    juce__label3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    juce__label3->setJustificationType (juce::Justification::centredLeft);
    juce__label3->setEditable (false, false, false);
    juce__label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    juce__label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    juce__label3->setBounds (184, 88, 80, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 150);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

RootComponent::~RootComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    juce__slider = nullptr;
    juce__slider2 = nullptr;
    juce__label = nullptr;
    juce__label2 = nullptr;
    juce__label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RootComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff163f53));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RootComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RootComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == juce__slider.get())
    {
        //[UserSliderCode_juce__slider] -- add your slider handling code here..
        //[/UserSliderCode_juce__slider]
    }
    else if (sliderThatWasMoved == juce__slider2.get())
    {
        //[UserSliderCode_juce__slider2] -- add your slider handling code here..
        //[/UserSliderCode_juce__slider2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RootComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="150">
  <BACKGROUND backgroundColour="ff163f53"/>
  <SLIDER name="cutoff slider" id="2286839aaff3e835" memberName="juce__slider"
          virtualName="" explicitFocusOrder="0" pos="24 56 150 24" thumbcol="fff25e5e"
          min="0.0" max="1200.0" int="0.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="resonance slider" id="1914824324c81579" memberName="juce__slider2"
          virtualName="" explicitFocusOrder="0" pos="24 88 150 24" thumbcol="5efff25e"
          min="0.0" max="1.0" int="0.0" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Bad Filter Label" id="2a6a6124871b1756" memberName="juce__label"
         virtualName="" explicitFocusOrder="0" pos="16 24 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Bad Filter" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="cddea11894d591e2" memberName="juce__label2"
         virtualName="" explicitFocusOrder="0" pos="184 56 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="1db4a3bc772937df" memberName="juce__label3"
         virtualName="" explicitFocusOrder="0" pos="184 88 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Resonance" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

