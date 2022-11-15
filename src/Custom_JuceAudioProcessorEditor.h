/*
 ==============================================================================

 RNBO_JuceAudioProcessorEditor.h
 Created: 21 Sep 2015 11:50:17am
 Author:  stb

 ==============================================================================
 */

#ifndef RNBO_JUCEAUDIOPROCESSOREDITOR_H_INCLUDED
#define RNBO_JUCEAUDIOPROCESSOREDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "RNBO.h"
#include "RootComponent.h"

namespace RNBO {

	//==============================================================================
	/**

		this is mostly a copy of a Juce GenericAudioProcessorEditor, with the important
		difference that it can be refreshed via a ParameterInterfacHandle

	 */
	class CustomAudioProcessorEditor : public AudioProcessorEditor, public AsyncUpdater, public RNBO::EventHandler
	{
	public:
		//==============================================================================
		CustomAudioProcessorEditor (AudioProcessor* owner, CoreObject& rnboObject);
		~CustomAudioProcessorEditor() override;

		//==============================================================================
		void paint (Graphics&) override;
		void resized() override;

		void handleAsyncUpdate() override;

		void eventsAvailable() override;
		void handleParameterEvent(const RNBO::ParameterEvent& event) override;
		void handlePresetEvent(const RNBO::PresetEvent& event) override;
		
	private:

		void updateAllParams();

		//==============================================================================
		CoreObject&								_rnboObject;
		ParameterEventInterfaceUniquePtr		_parameterInterface;

		RootComponent	_rootComponent;
	};

} // namespace RNBO

#endif  // RNBO_JUCEAUDIOPROCESSOREDITOR_H_INCLUDED
