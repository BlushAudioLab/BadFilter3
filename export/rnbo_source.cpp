/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2022 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing@cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define floor(x) ((long)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

rnbomatic()
{
}

~rnbomatic()
{
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo();
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
    if (!stringCompare(paramid, "Cutoff")) {
        return 0;
    }

    if (!stringCompare(paramid, "Resonance")) {
        return 1;
    }

    return INVALID_INDEX;
}

Index getNumMidiInputPorts() const {
    return 0;
}

void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    SampleValue ** inputs,
    Index numInputs,
    SampleValue ** outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * Cutoff = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
    SampleValue * Resonance = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    this->paramtilde_01_perform(Cutoff, this->signals[0], n);
    this->paramtilde_02_perform(Resonance, this->signals[1], n);
    this->lores_01_perform(this->signals[0], this->signals[1], in1, out1, n);
    this->lores_02_perform(this->signals[0], this->signals[1], in2, out2, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 2; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->paramtilde_01_sigbuf = resizeSignal(this->paramtilde_01_sigbuf, this->maxvs, maxBlockSize);
        this->paramtilde_02_sigbuf = resizeSignal(this->paramtilde_02_sigbuf, this->maxvs, maxBlockSize);
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->paramtilde_01_dspsetup(forceDSPSetup);
    this->paramtilde_02_dspsetup(forceDSPSetup);
    this->lores_01_dspsetup(forceDSPSetup);
    this->lores_02_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        this->setProbingIndex(-1);
        break;
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    default:
        return nullptr;
    }
}

DataRefIndex getNumDataRefs() const {
    return 0;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}

void initialize() {
    this->assign_defaults();
    this->setState();
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
}

void setPreset(MillisecondTime , PatcherStateInterface& ) {}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(tempo, false))
        {}
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(state, false))
        {}
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(beattime, false))
        {}
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(numerator, denominator, false))
        {}
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        this->paramtilde_01_value_set(v);
        break;
    case 1:
        this->paramtilde_02_value_set(v);
        break;
    case 2:
        // namedAudioIn: Cutoff
        break;
    case 3:
        // namedAudioIn: Resonance
        break;
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        return this->paramtilde_01_value;
    case 1:
        return this->paramtilde_02_value;
    case 2:
        // namedAudioIn: Cutoff
        return 0;
    case 3:
        // namedAudioIn: Resonance
        return 0;
    default:
        return 0;
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 2;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 4;
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "Cutoff";
    case 1:
        return "Resonance";
    case 2:
        return "Cutoff";
    case 3:
        return "Resonance";
    default:
        return "bogus";
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "Cutoff";
    case 1:
        return "Resonance";
    case 2:
        return "/signals/Cutoff";
    case 3:
        return "/signals/Resonance";
    default:
        return "bogus";
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            info->type = ParameterTypeNumber;
            info->initialValue = 700;
            info->min = 10;
            info->max = 12000;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 1:
            info->type = ParameterTypeNumber;
            info->initialValue = 0.2;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 2:
            info->type = ParameterTypeSignal;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = false;
            info->transmittable = false;
            info->initialized = false;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeInput;
            info->signalIndex = 2;
            break;
        case 3:
            info->type = ParameterTypeSignal;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = false;
            info->transmittable = false;
            info->initialized = false;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeInput;
            info->signalIndex = 3;
            break;
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 1:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
    case 0:
        {
            value = (value < 10 ? 10 : (value > 12000 ? 12000 : value));
            ParameterValue normalizedValue = (value - 10) / (12000 - 10);
            return normalizedValue;
        }
    default:
        return value;
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 1:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (1 - 0);
            }
        }
    case 0:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 10 + value * (12000 - 10);
            }
        }
    default:
        return value;
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    default:
        return value;
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}

void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}

void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {

    }

    return "";
}

MessageIndex getNumMessages() const {
    return 0;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {

    }

    return NullMessageInfo;
}

protected:

void paramtilde_01_value_set(number v) {
    this->paramtilde_01_value_setter(v);
    v = this->paramtilde_01_value;
    this->sendParameter(0, false);
    SampleIndex k = (SampleIndex)(this->sampleOffsetIntoNextAudioBuffer);

    if ((bool)(this->paramtilde_01_sigbuf)) {
        for (SampleIndex i = (SampleIndex)(this->paramtilde_01_lastIndex); i < k; i++) {
            this->paramtilde_01_sigbuf[(Index)i] = v;
            this->paramtilde_01_lastIndex = k;
        }
    }
}

void paramtilde_02_value_set(number v) {
    this->paramtilde_02_value_setter(v);
    v = this->paramtilde_02_value;
    this->sendParameter(1, false);
    SampleIndex k = (SampleIndex)(this->sampleOffsetIntoNextAudioBuffer);

    if ((bool)(this->paramtilde_02_sigbuf)) {
        for (SampleIndex i = (SampleIndex)(this->paramtilde_02_lastIndex); i < k; i++) {
            this->paramtilde_02_sigbuf[(Index)i] = v;
            this->paramtilde_02_lastIndex = k;
        }
    }
}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 2;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {}

void initializeObjects() {}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
    this->processParamInitEvents();
}

void paramtilde_01_perform(const Sample * Cutoff, Sample * out, Index n) {
    auto __paramtilde_01_sigbuf = this->paramtilde_01_sigbuf;
    auto __paramtilde_01_value = this->paramtilde_01_value;
    auto __paramtilde_01_lastIndex = this->paramtilde_01_lastIndex;

    for (Index i = 0; i < n; i++) {
        if (i >= __paramtilde_01_lastIndex) {
            out[(Index)i] = Cutoff[(Index)i] + __paramtilde_01_value;
        } else {
            out[(Index)i] = Cutoff[(Index)i] + __paramtilde_01_sigbuf[(Index)i];
        }
    }

    __paramtilde_01_lastIndex = 0;
    this->paramtilde_01_lastIndex = __paramtilde_01_lastIndex;
}

void paramtilde_02_perform(const Sample * Resonance, Sample * out, Index n) {
    auto __paramtilde_02_sigbuf = this->paramtilde_02_sigbuf;
    auto __paramtilde_02_value = this->paramtilde_02_value;
    auto __paramtilde_02_lastIndex = this->paramtilde_02_lastIndex;

    for (Index i = 0; i < n; i++) {
        if (i >= __paramtilde_02_lastIndex) {
            out[(Index)i] = Resonance[(Index)i] + __paramtilde_02_value;
        } else {
            out[(Index)i] = Resonance[(Index)i] + __paramtilde_02_sigbuf[(Index)i];
        }
    }

    __paramtilde_02_lastIndex = 0;
    this->paramtilde_02_lastIndex = __paramtilde_02_lastIndex;
}

void lores_01_perform(
    const Sample * cutoff,
    const Sample * resonance,
    const Sample * input,
    Sample * out,
    Index n
) {
    auto __lores_01_y_2 = this->lores_01_y_2;
    auto __lores_01_y_1 = this->lores_01_y_1;
    auto __lores_01_x_0 = this->lores_01_x_0;
    auto __lores_01_b_0 = this->lores_01_b_0;
    auto __lores_01_a_1 = this->lores_01_a_1;
    auto __lores_01_a_2 = this->lores_01_a_2;
    auto __lores_01_last_freq_calc = this->lores_01_last_freq_calc;
    auto __lores_01_last_res_calc = this->lores_01_last_res_calc;
    auto __lores_01_last_frequency = this->lores_01_last_frequency;
    auto __lores_01_last_resonance = this->lores_01_last_resonance;

    for (Index i = 0; i < n; i++) {
        if (resonance[(Index)i] != __lores_01_last_resonance || cutoff[(Index)i] != __lores_01_last_frequency) {
            __lores_01_last_res_calc = rnbo_exp(resonance[(Index)i] * 0.125) * 0.882497;
            __lores_01_last_freq_calc = rnbo_cos(2.0 * pi01() / this->sr * cutoff[(Index)i]);
            __lores_01_last_resonance = resonance[(Index)i];
            __lores_01_last_frequency = cutoff[(Index)i];
        }

        number res_temp = __lores_01_last_res_calc;
        number freq_temp = __lores_01_last_freq_calc;
        __lores_01_a_2 = res_temp * res_temp;
        __lores_01_a_1 = -2.0 * res_temp * freq_temp;
        __lores_01_b_0 = 1.0 + __lores_01_a_1 + __lores_01_a_2;
        __lores_01_x_0 = input[(Index)i];
        number y_0 = __lores_01_x_0 * __lores_01_b_0 - __lores_01_y_1 * __lores_01_a_1 - __lores_01_y_2 * __lores_01_a_2;
        __lores_01_y_2 = __lores_01_y_1;
        __lores_01_y_1 = y_0;
        out[(Index)i] = y_0;
    }

    this->lores_01_last_resonance = __lores_01_last_resonance;
    this->lores_01_last_frequency = __lores_01_last_frequency;
    this->lores_01_last_res_calc = __lores_01_last_res_calc;
    this->lores_01_last_freq_calc = __lores_01_last_freq_calc;
    this->lores_01_a_2 = __lores_01_a_2;
    this->lores_01_a_1 = __lores_01_a_1;
    this->lores_01_b_0 = __lores_01_b_0;
    this->lores_01_x_0 = __lores_01_x_0;
    this->lores_01_y_1 = __lores_01_y_1;
    this->lores_01_y_2 = __lores_01_y_2;
}

void lores_02_perform(
    const Sample * cutoff,
    const Sample * resonance,
    const Sample * input,
    Sample * out,
    Index n
) {
    auto __lores_02_y_2 = this->lores_02_y_2;
    auto __lores_02_y_1 = this->lores_02_y_1;
    auto __lores_02_x_0 = this->lores_02_x_0;
    auto __lores_02_b_0 = this->lores_02_b_0;
    auto __lores_02_a_1 = this->lores_02_a_1;
    auto __lores_02_a_2 = this->lores_02_a_2;
    auto __lores_02_last_freq_calc = this->lores_02_last_freq_calc;
    auto __lores_02_last_res_calc = this->lores_02_last_res_calc;
    auto __lores_02_last_frequency = this->lores_02_last_frequency;
    auto __lores_02_last_resonance = this->lores_02_last_resonance;

    for (Index i = 0; i < n; i++) {
        if (resonance[(Index)i] != __lores_02_last_resonance || cutoff[(Index)i] != __lores_02_last_frequency) {
            __lores_02_last_res_calc = rnbo_exp(resonance[(Index)i] * 0.125) * 0.882497;
            __lores_02_last_freq_calc = rnbo_cos(2.0 * pi01() / this->sr * cutoff[(Index)i]);
            __lores_02_last_resonance = resonance[(Index)i];
            __lores_02_last_frequency = cutoff[(Index)i];
        }

        number res_temp = __lores_02_last_res_calc;
        number freq_temp = __lores_02_last_freq_calc;
        __lores_02_a_2 = res_temp * res_temp;
        __lores_02_a_1 = -2.0 * res_temp * freq_temp;
        __lores_02_b_0 = 1.0 + __lores_02_a_1 + __lores_02_a_2;
        __lores_02_x_0 = input[(Index)i];
        number y_0 = __lores_02_x_0 * __lores_02_b_0 - __lores_02_y_1 * __lores_02_a_1 - __lores_02_y_2 * __lores_02_a_2;
        __lores_02_y_2 = __lores_02_y_1;
        __lores_02_y_1 = y_0;
        out[(Index)i] = y_0;
    }

    this->lores_02_last_resonance = __lores_02_last_resonance;
    this->lores_02_last_frequency = __lores_02_last_frequency;
    this->lores_02_last_res_calc = __lores_02_last_res_calc;
    this->lores_02_last_freq_calc = __lores_02_last_freq_calc;
    this->lores_02_a_2 = __lores_02_a_2;
    this->lores_02_a_1 = __lores_02_a_1;
    this->lores_02_b_0 = __lores_02_b_0;
    this->lores_02_x_0 = __lores_02_x_0;
    this->lores_02_y_1 = __lores_02_y_1;
    this->lores_02_y_2 = __lores_02_y_2;
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void lores_01_resonance_setter(number v) {
    if (v < 0.0) {
        v = 0.0;
    } else if (v >= 1.0) {
        v = 1.0 - 1E-20;
    }

    this->lores_01_resonance = v;
}

void lores_02_resonance_setter(number v) {
    if (v < 0.0) {
        v = 0.0;
    } else if (v >= 1.0) {
        v = 1.0 - 1E-20;
    }

    this->lores_02_resonance = v;
}

void paramtilde_01_value_setter(number v) {
    if ((bool)(!(bool)(isNaN(12000))) && v >= 12000) {
        v = 12000;
    }

    if ((bool)(!(bool)(isNaN(10))) && v <= 10) {
        v = 10;
    }

    this->paramtilde_01_value = v;
}

void paramtilde_02_value_setter(number v) {
    if ((bool)(!(bool)(isNaN(1))) && v >= 1) {
        v = 1;
    }

    if ((bool)(!(bool)(isNaN(0))) && v <= 0) {
        v = 0;
    }

    this->paramtilde_02_value = v;
}

void lores_01_reset() {
    this->lores_01_y_1 = 0;
    this->lores_01_y_2 = 0;
    this->lores_01_last_resonance = 0;
    this->lores_01_last_frequency = 0;
    this->lores_01_last_res_calc = 0;
    this->lores_01_last_freq_calc = 0;
}

void lores_01_dspsetup(bool force) {
    if ((bool)(this->lores_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->lores_01_reset();
    this->lores_01_setupDone = true;
}

void lores_02_reset() {
    this->lores_02_y_1 = 0;
    this->lores_02_y_2 = 0;
    this->lores_02_last_resonance = 0;
    this->lores_02_last_frequency = 0;
    this->lores_02_last_res_calc = 0;
    this->lores_02_last_freq_calc = 0;
}

void lores_02_dspsetup(bool force) {
    if ((bool)(this->lores_02_setupDone) && (bool)(!(bool)(force)))
        return;

    this->lores_02_reset();
    this->lores_02_setupDone = true;
}

void paramtilde_01_dspsetup(bool force) {
    if ((bool)(this->paramtilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->paramtilde_01_lastIndex = 0;
    this->paramtilde_01_setupDone = true;
}

void paramtilde_02_dspsetup(bool force) {
    if ((bool)(this->paramtilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    this->paramtilde_02_lastIndex = 0;
    this->paramtilde_02_setupDone = true;
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getTempo() {
    return this->globaltransport_getTempoAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState() {
    return this->globaltransport_getStateAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getBeatTimeAtMsTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState() == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(this->currenttime(), tempo);
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getTempo() != tempo) {
        MillisecondTime ct = this->currenttime();
        this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTimeAtMsTime(ct));
        this->globaltransport_beatTimeChanges->push(ct);

        fillSignal(
            this->globaltransport_tempo,
            this->vs,
            tempo,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastTempo = tempo;
        this->globaltransport_tempoNeedsReset = true;
        return true;
    }

    return false;
}

number globaltransport_getBeatTime() {
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime());
}

bool globaltransport_setState(number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(this->currenttime(), TransportState(state));
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getState() != state) {
        fillSignal(
            this->globaltransport_state,
            this->vs,
            state,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastState = TransportState(state);
        this->globaltransport_stateNeedsReset = true;

        if (state == 0) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime());
            this->globaltransport_beatTimeChanges->push(this->currenttime());
        }

        return true;
    }

    return false;
}

bool globaltransport_setBeatTime(number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(this->currenttime(), beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime());
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(this->currenttime());
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignatureAtMsTime(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignature() {
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime());
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime() + msOffset);
}

bool globaltransport_setTimeSignature(number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(this->currenttime(), (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature();

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(this->currenttime());
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    lores_01_cutoff = 0;
    lores_01_resonance = 0;
    lores_01_resonance_setter(lores_01_resonance);
    lores_02_cutoff = 0;
    lores_02_resonance = 0;
    lores_02_resonance_setter(lores_02_resonance);
    paramtilde_01_value = 700;
    paramtilde_01_value_setter(paramtilde_01_value);
    paramtilde_02_value = 0.2;
    paramtilde_02_value_setter(paramtilde_02_value);
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    lores_01_setupDone = false;
    lores_02_setupDone = false;
    paramtilde_01_lastIndex = 0;
    paramtilde_01_sigbuf = nullptr;
    paramtilde_01_setupDone = false;
    paramtilde_02_lastIndex = 0;
    paramtilde_02_sigbuf = nullptr;
    paramtilde_02_setupDone = false;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number lores_01_cutoff;
    number lores_01_resonance;
    number lores_02_cutoff;
    number lores_02_resonance;
    number paramtilde_01_value;
    number paramtilde_02_value;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[2];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number lores_01_b_0;
    number lores_01_a_1;
    number lores_01_a_2;
    number lores_01_x_0;
    number lores_01_y_1;
    number lores_01_y_2;
    number lores_01_last_resonance;
    number lores_01_last_frequency;
    number lores_01_last_res_calc;
    number lores_01_last_freq_calc;
    bool lores_01_setupDone;
    number lores_02_b_0;
    number lores_02_a_1;
    number lores_02_a_2;
    number lores_02_x_0;
    number lores_02_y_1;
    number lores_02_y_2;
    number lores_02_last_resonance;
    number lores_02_last_frequency;
    number lores_02_last_res_calc;
    number lores_02_last_freq_calc;
    bool lores_02_setupDone;
    SampleIndex paramtilde_01_lastIndex;
    signal paramtilde_01_sigbuf;
    bool paramtilde_01_setupDone;
    SampleIndex paramtilde_02_lastIndex;
    signal paramtilde_02_sigbuf;
    bool paramtilde_02_setupDone;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

