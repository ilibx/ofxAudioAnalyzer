# pragma once


#include "ofMain.h"

#include "essentia/algorithmfactory.h"
#include "essentia/essentiamath.h" // for the isSilent function
#include "essentia/pool.h"

using namespace std;
using namespace essentia;
using namespace standard;

//??? set clamp fixed and madatory??

class ofxAABaseAlgorithm{
    
public:
    
    
    void init();
    
    float getValue();
    float getValueDb();
    
    float getValueNormalized(float min, float max, bool doClamp=TRUE);
    float getValueDbNormalized(float min, float max, bool doClamp=TRUE);
    float getSmoothedValue(float smthAmnt);
    
    
    float getSmoothedValueNormalized(float smthAmnt, float min, float max, bool doClamp=TRUE);
    float getSmoothedValueDbNormalized(float smthAmnt, float min, float max, bool doClamp=TRUE);
    
    bool getIsActive();
    
    void setActive(bool state);
    void setValueZero();
    
    void compute();
    
    void castValueToFloat();
    
    void deleteAlgorithm();
    
    
    Algorithm* algorithm;
    Real realValue;
    
    
protected:
    
    bool isActivated;
    float floatValue;
    float smoothedFloatValue;
    float smoothedNormFloatValue;
    float smoothedNormFloatValueDb;
    
};
//---------------------------------------------------------------------
class ofxAAOneVectorOutputAlgorithm : public ofxAABaseAlgorithm{

public:
    
    void init();
    
    void initAndAssignSize(int size, int initValues);
    
    void assignFloatValuesSize(int size, int val);
    
    void castValuesToFloat(bool logarithmic);
    
    void updateLogRealValues();
    
    int getBinsNum();
    vector<float>& getValues();
    vector<float>& getSmoothedValues(float smthAmnt);
    
    vector<Real> realValues;
    vector<Real> logRealValues;
    
private:
    
    vector<float> floatValues;
    vector<float> smoothedFloatValues;
    
};

//---------------------------------------------------------------------
class ofxAACartToPolAlgorithm : public ofxAABaseAlgorithm{
public:
    vector<Real> magnitudes;
    vector<Real> phases;
    
};
//---------------------------------------------------------------------
class ofxAAPeaksAlgorithm : public ofxAABaseAlgorithm{
public:
    vector<Real> frequencies;
    vector<Real> magnitudes;
    
};

//---------------------------------------------------------------------
class ofxAAFftAlgorithm : public ofxAAOneVectorOutputAlgorithm{
    public:
    vector<complex<Real> > fftRealValues;

};
//---------------------------------------------------------------------
class ofxAAPitchDetectAlgorithm : public ofxAABaseAlgorithm{

public:
    
    void castValuesToFloat();
    
    float getPitchValue();
    float getConfidenceValue();
    float getSmoothedPitchValue(float smthAmnt);
    float getSmoothedConfidenceValue(float smthAmnt);
 
    Real pitchRealVal, confidenceRealVal;

private:
    float pitchFloatVal, confidenceFloatVal;
    float smoothedPitchFloatValue = 0.0;
    float smoothedConfidenceFloatValue = 0.0;

};
//---------------------------------------------------------------------
class ofxAATuningFrequencyAlgorithm : public ofxAABaseAlgorithm{
    
public:
    
    void castValuesToFloat();
    
    float getFreqValue();
    float getCentsValue();
    
    Real freqRealVal,   centsRealVal;
    
    private:
    float freqFloatVal, centsFloatVal;
    
};