# tinybEEr
Beer Classification TinyML Model run on Arduino Nano 33 BLE.

# BeerClassifier\BeerClassifier.ino
Main sketch containing deployed classification model and sensor readings to generate prediction ratings for the supported beer types. These beers are Ale, Stout, Hazy IPA, Lager, and IPA.

# sensor_readings_final\sensor_readings_final.ino
Sketch solely for collecting pH and turbidity readings. This was used to collect our beer data.

# ei-beermodel-arduino-1.0.3.zip
Deployed arduino library from Edge Impulse containing model code and supporting libraries. To run the BeerClassifier.ino file you must extract this .zip file into your local arduino library folder.

# ei-beermodel-classifier.zip
Raw classifier model code exported from Edge Impulse for manual editing if desired.

# tinybEEr_flatbEEr_dataSet - Sheet1.csv
Dataset of flat beer used to train final model.

# ----------To Run----------
Extract ei-beermodel-arduino-1.0.3.zip to local Arduino library folder, then open BeerClassifier.ino in Arduino IDE. Compile sketch and load to Arduino Nano 33 BLE Sense board.
