let m_levelMeter = document.getElementById("inputLevelMeter");
//
let m_levelProgress = document.getElementById("inputLevelProgress");
//
let m_inputSlider = document.getElementById("inputSlider");
//
let m_inputSliderLabel = document.getElementById("inputSliderLabel");
//
function SetMeterFeedback(newValue) {
    m_levelMeter.value = newValue;
}
//
function SetProgressBarFeedback(newValue) {
    m_levelProgress.value = newValue;
}
//
function OnButtonClick() {
    m_inputSliderLabel.textContent = "button clicked!"
    m_levelMeter.value = 0.5;
    m_levelProgress.value = 0.5;
}
//
function OnSliderValueChanged(newValue) {
    m_inputSliderLabel.textContent = newValue;
    SetMeterFeedback(newValue);
    SetProgressBarFeedback(newValue);
    //OnSliderValueChangedNative(this.value);
    if(window.webkit)
    {
        //m_inputSliderLabel.innerText = "WebKit";
        window.webkit.messageHandlers.sliderMessageHandler.postMessage(this.value);
    }
}
//m_inputSlider.addEventListener("input", OnSliderValueChanged);
//
function OnColorPickerChanged(newValue) {
    document.body.style.backgroundColor = newValue;
}