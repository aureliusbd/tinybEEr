// If your target is limited in memory remove this macro to save 10K RAM
#define EIDSP_QUANTIZE_FILTERBANK   0

/* Includes ---------------------------------------------------------------- */
#include <PDM.h>
#include <BeerModel_inferencing.h>
#include <Wire.h> 
// #include <LiquidCrystal_I2C.h>

// pH sensor
#define phPin A0
#define turbPin A1
float turbV;

// lcd display
// LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

float GetPhValue()
{
  static int buf[10];

  for(int index = 0; index < 10; ++index)
  { 
    buf[index] = analogRead(phPin);
    delay(10);
  }

  for(int i = 0; i < 9; ++i)
  {
    for(int j = i + 1; j < 10; ++j)
    {
      if(buf[i] > buf[j])
      {
        int temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  unsigned long int avgVal = 0;
  for(int i = 2; i < 8; ++i)
  {
    avgVal += buf[i];
  }
    
  float phValue = (float)avgVal * 3.3/1024/6;

  return phValue;  
}

float GetTurbValue()
{
  static float turbV = 0.0;
  for (int i = 0; i < 800; i++) {
    turbV += ((float)analogRead(turbPin)/1023)*3.3;
  }
  turbV = turbV/800; 

  return turbV;
}

/* ------------------------------ Start of model code --------------------------------- */

/** Audio buffers, pointers and selectors */
// typedef struct {
//     int16_t *buffer;
//     uint8_t buf_ready;
//     uint32_t buf_count;
//     uint32_t n_samples;
// } inference_t;

// static inference_t inference;
// static signed short sampleBuffer[2048];

/**
 * @brief      Arduino setup function
 */
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    // lcd.init(); 
    // lcd.backlight();
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");

    // summary of inferencing settings (from model_metadata.h)
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
    /* Sample Collection and Inference Section */
    Serial.println("Collecting next sample in 2 seconds...");
    delay(2000);

    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
    buffer[0] = GetPhValue();
    buffer[1] = GetTurbValue();

    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0)
    {
      Serial.print("Failed to create signal from buffer ");
      Serial.print(err);
      Serial.println();
      return;
    }

    /* Classifier Section */
    ei_impulse_result_t result = { 0 };

    Serial.println("Running Beer Classifier");

    EI_IMPULSE_ERROR r = run_classifier(&signal, &result, false);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }

    // print the predictions
    Serial.println("Predictions :");
    // ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
    //     result.timing.dsp, result.timing.classification, result.timing.anomaly);
    // ei_printf(": \n");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ++ix) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }

    Serial.println("pH:");
    Serial.println(GetPhValue());
}
