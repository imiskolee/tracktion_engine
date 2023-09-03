#include "JuceHeader.h"
#include <emscripten.h>
#include <emscripten/html5.h>
#include "plugin.h"

std::unique_ptr<tracktion_engine::Engine> engine = nullptr;
std::unique_ptr<tracktion_engine::Edit> edit = nullptr;

class MyLogger : public juce::Logger {

protected:
    void logMessage(const String &message) override {
        emscripten_log(0,message.toRawUTF8());
    }
};

EM_BOOL OnCanvasClick(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
    juce::Logger::setCurrentLogger(new MyLogger());
    std::cout << "Starting Test" << std::endl;
    juce::String editFilePath = "./test.pro";
    const File editFile (editFilePath);
    engine = std::make_unique<tracktion_engine::Engine>("Test");
    edit = tracktion_engine::createEmptyEdit (*engine, editFile);
    edit->ensureNumberOfAudioTracks(tracktion_engine::getAudioTracks(*edit).size() + 1);
    auto track = tracktion_engine::getAudioTracks(*edit).getLast();
    engine->getPluginManager().createBuiltInType<CustomSynthPlugin>();
    auto plugin = edit->getPluginCache().createNewPlugin(CustomSynthPlugin::xmlTypeName,{});
    plugin->setEnabled(true);
    plugin->setProcessingEnabled(true);
    track->pluginList.insertPlugin(plugin,-1,nullptr);
    track->setName("Piano");
    auto clipTrack = static_cast<tracktion_engine::ClipTrack*>(track);
    tracktion::TimeRange range(tracktion::TimePosition::fromSeconds(0),tracktion::TimePosition::fromSeconds(10));
    auto clip = clipTrack->insertMIDIClip(range,nullptr);
    tracktion_engine::MidiClip* midiClip = static_cast<tracktion_engine::MidiClip*>(clip.get());
    clip->setMidiChannel(tracktion::MidiChannel(1));
    auto& seq = midiClip->getSequence();
    for(int i =0; i <30;i++) {
        seq.addNote(40 +i,tracktion::BeatPosition::fromBeats(i),tracktion::BeatDuration::fromBeats(1),100,0,nullptr);
    }
    edit->getTransport().play(false);
   // engine->getDeviceManager().deviceManager.playTestSound();
    return true;
}

static void mainLoop() {

}

int main() {
    juce::ScopedJuceInitialiser_GUI init;
    emscripten_set_click_callback("body", nullptr, 0, OnCanvasClick);
    emscripten_set_main_loop(mainLoop,30,1);
    emscripten_resume_main_loop();
    return 1;
}