#pragma once

#include "ofMain.h"

#include <iostream>
#include "essentia/algorithmfactory.h"
#include "essentia/essentiamath.h" // for the isSilent function
#include "essentia/pool.h"

using namespace std;
using namespace essentia;
using namespace standard;

#include "ofxAudioAnalyzerAlgorithms.h"

//----------------------------
//vars use in setup function

#define MELBANDS_BANDS_NUM 24
#define DCT_COEFF_NUM 10

#define HPCP_SIZE 12
#define HPCP_MIN_FREQ 40.0//hz
#define HPCP_MAX_FREQ 5000.0//hz

#define PEAKS_MAXPEAKS_NUM 10000
#define PEAKS_MIN_FREQ 40.0//hz
#define PEAKS_MAX_FREQ 5000.0//hz

//-----------------------------
#define MFCC_MAX_ESTIMATED_VALUE 300.0 //??? donde se usa

#define DB_MIN -6
#define DB_MAX 0

//----------------------------------
enum OnsetsTimeTresholdMode{
    TIME_BASED,
    BUFFER_NUM_BASED
};

//----------------------------------

class ofxAudioAnalyzerUnit
{

public:
    
    ofxAudioAnalyzerUnit(int sampleRate, int bufferSize){
        setup(sampleRate, bufferSize);
    }
    ~ofxAudioAnalyzerUnit(){
        exit();
    }
    void setup(int sampleRate, int bufferSize);
    void exit();

    void analyze(const vector<float> &  inBuffer);

    void resetOnsets();

    
    //Value getters -------------------
    
    float getRms(float smooth=0.0);
    float getEnergy(float smooth=0.0);
    float getPower(float smooth=0.0);

    float getPitchFreq(float smooth=0.0);
    int   getPitchFreqAsMidiNote(float smooth=0.0);
    string getPitchFreqAsNoteName(float smooth=0.0);
    float getPitchConfidence(float smooth=0.0);
    float getMelodySalience(float smooth=0.0);

    float getTuningFreq();
    float getTuningCents();
    
    float getInharmonicity(float smooth=0.0);
    float getHfc(float smooth=0.0);
    float getSpectralComplex(float smooth=0.0);
    float getCentroid(float smooth=0.0);

    float getHfcNormalized(float smooth=0.0);
    float getSpectralComplexNormalized(float smooth=0.0);
    float getCentroidNormalized(float smooth=0.0);
    
    ///test:
    float getDissonance(float smooth=0.0);
    vector<float>& getKlapuriMultiPitchesRef();
    vector<SalienceFunctionPeak>& getPitchSaliencePeaksRef();
    
    
    
    bool getIsOnset();

    int getSpectrumBinsNum();
    int getMelBandsBinsNum();
    int getMfccBinsNum();
    int getHpcpBinsNum();
    
    vector<float>& getSpectrumRef(float smooth=0.0);
    vector<float>& getMelBandsRef(float smooth=0.0);
    vector<float>& getDctRef(float smooth=0.0);
    vector<float>& getHpcpRef(float smooth=0.0);

    float getOnsetSilenceTreshold(){return silenceTreshold;}
    float getOnsetTimeTreshold(){return timeTreshold;}
    float getOnsetAlpha(){return alpha;}

    
    //Algorithms activation -------------------
    void setActiveRms(bool state);
    void setActiveEnergy(bool state);
    void setActivePower(bool state);
    void setActivePitch(bool state);
    void setActiveMelodySalience(bool state);
    
    void setActiveTuning(bool state);
    
    void setActiveInharmonicity(bool state);
    void setActiveHfc(bool state);
    void setActiveSpectralComplex(bool state);
    void setActiveCentroid(bool state);
    void setActiveMelbandsAndMfcc(bool state);
    void setActiveHpcp(bool state);

    void setActiveOnsets(bool state);
    
    ///test:
    void setActiveDissonance(bool state);
    void setActivePitchSalienceFunctionPeaks(bool state);
    void setActiveKlapuriMultiPitch(bool state);
    
    //Onsets configuration -------------------
    void setOnsetSilenceTreshold(float val);
    void setOnsetAlpha(float val);
    void setOnsetTimeTreshold(float ms);
    void setOnsetBufferNumTreshold(int buffersNum);
    void setUseTimeTreshold(bool doUse){useTimeTreshold = doUse;}
    
    //Max estimated values -------------------
    void setMaxEnergyEstimatedValue(float val){maxEnergyEstimatedValue = val;}
    void setMaxHfcEstimatedValue(float val){maxHfcEstimatedValue = val;}
    void setMaxSpecCompEstimatedValue(float val){maxSpecCompEstimatedValue = val;}
    void setMaxCentroidEstimatedValue(float val){maxCentroidEstimatedValue = val;}

private:
    
    vector<Real> audioBuffer;

    //algorithms with return value func
    ofxAABaseAlgorithm rms;
    ofxAABaseAlgorithm energy;
    ofxAABaseAlgorithm power;
    ofxAAPitchDetectAlgorithm pitchDetect;
    ofxAABaseAlgorithm pitchSalience;
    ofxAATuningFrequencyAlgorithm tuning;
    ofxAABaseAlgorithm inharmonicity;
    ofxAABaseAlgorithm hfc;
    ofxAABaseAlgorithm centroid;
    ofxAABaseAlgorithm spectralComplex;
    
    ofxAAOneVectorOutputAlgorithm spectrum;
    ofxAAOneVectorOutputAlgorithm melBands;
    ofxAAOneVectorOutputAlgorithm dct;//MFCC
    ofxAAOneVectorOutputAlgorithm hpcp;
    
    
    ///test adding algorithms:
    ofxAABaseAlgorithm dissonance;
    ofxAAOneVectorOutputAlgorithm pitchSalienceFunction;
    ofxAAPitchSalienceFunctionPeaksAlgorithm pitchSalienceFunctionPeaks;
    ofxAAMultiPitchKlapuriAlgorithm multiPitchKlapuri;

    //algorithms for internal functionality:
    ofxAAOneVectorOutputAlgorithm dcremoval;
    ofxAAOneVectorOutputAlgorithm window;
    ofxAAFftAlgorithm fft;
    ofxAACartToPolAlgorithm cartesian2polar;
    ofxAAPeaksAlgorithm spectralPeaks;
    ofxAAPeaksAlgorithm harmonicPeaks;
    ofxAABaseAlgorithm onsetHfc;
    ofxAABaseAlgorithm onsetComplex;
    ofxAABaseAlgorithm onsetFlux;

    //Onset detection------------
    bool onsetEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux);
    bool onsetTimeTresholdEvaluation();
    bool onsetBufferNumTresholdEvaluation();//framebased treshold eval.
    bool isOnset;
    Real silenceTreshold, alpha;
    bool addHfc, addComplex, addFlux;
    bool doOnsets;
    bool useTimeTreshold;
    float timeTreshold;
    float lastOnsetTime;
    int bufferNumTreshold;
    int lastOnsetBufferNum;
    
    OnsetsTimeTresholdMode onsetsMode;

    int framesize;
    int hopsize;
    int sr;
    int zeropadding;
    Real framerate;

    int detecBufferSize;
    vector<vector<Real> > detections;
    vector<Real> detection_sum;
    Real hfc_max, complex_max, flux_max;

    //Utils
    int pitchToMidi(float pitch);
    string midiToNoteName(int midiNote);

    float maxEnergyEstimatedValue;
    float maxHfcEstimatedValue;
    float maxSpecCompEstimatedValue;
    float maxCentroidEstimatedValue;
    
    int bufferCounter;
    
    //klapuri test:
   
    
};

