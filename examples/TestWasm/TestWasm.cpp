#include "JuceHeader.h"

int main() {
    juce::ScopedJuceInitialiser_GUI init;
    juce::String editFilePath = "./test.pro";
    const File editFile (editFilePath);
    tracktion_engine::Engine engine("Test");
    auto  edit = tracktion_engine::createEmptyEdit (engine, editFile);
    edit->ensureNumberOfAudioTracks(tracktion_engine::getAudioTracks(*edit).size() + 1);
    auto track = tracktion_engine::getAudioTracks(*edit).getLast();
    track->setName("Piano");
    auto clipTrack = static_cast<tracktion_engine::ClipTrack*>(track);
    auto clip = clipTrack->insertMIDIClip({},nullptr);
    edit->getTransport().play(true);
    juce::Thread::sleep(1000);
    return 1;
}