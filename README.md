# SolarRadiometer

The aim of this project is to design, calibrate, and validate a simple low-cost sun radiometer.
We follow the method proposed by Forrest Mims [1] which consists of using a light-emitting
diode (LED) as a low-cost detector and optical filter. The LED acts as a photovoltaic (PV)
detector whose short-circuit current (photocurrent) is proportional to the Sun’s irradiance.
This current and, thus, the voltage it generates are amplified by a transimpedace amplifier as shown below [2]:

Fig. 1.
<img src=https://github.com/GiorgioMorales/SolarRadiometer/blob/master/Images/circuit.jpg alt="alt text" width=350 height=150>

## Device Specifications

The architecture of the proposed system was designed in Solidworks and consists of two
degrees of freedom, as shown in Fig. 2. The tube of this system is designed to house an
LED diode that is used as our sensor. The length of the tube allows the sensor to have
a narrow aperture angle of 3° approximately in order to measure direct radiation only and
avoid the interference of diffused radiation [3]. The LED acts as a PV detector and its short-
circuit current is amplified by an operational amplifier (LM324) using a feedback resistor
Rf = 4M Ω. The amplified signal (voltage) is automatically read by an Arduino UNO. The
amplifier circuit uses a voltage source of ±9V while the servomotors are powered with a
separate voltage source of 5V . The implemented device is shown in Fig. 3. Note that Fig. 3
shows that a red LED was used for initial experiments; however, a green LED (475 nm) was
used to collect the data used in this report.

Fig. 2.
<img src=https://github.com/GiorgioMorales/SolarRadiometer/blob/master/Images/solid.jpg alt="alt text" width=300 height=250>

Fig. 3.
<img src=https://github.com/GiorgioMorales/SolarRadiometer/blob/master/Images/device.jpg alt="alt text" width=320 height=150>

Fig. 4.
<img src=https://github.com/GiorgioMorales/SolarRadiometer/blob/master/Images/Afterhours.jpg alt="alt text" width=450 height=250>

The position of the tube is controlled by two servomotors, whose angles are calculated by
the Arduino. More specifically, the purpose of the Arduino program is to align the tube
with the position of the Sun thorough the day; that is, it calculates the Sun’s azimuth and
zenith angles constantly. 

## Results

An example of the data collected is shown in Fig. 5. From this, we obtained the Langley plot shown in Fig. 6.

Fig. 5.
<img src=https://github.com/GiorgioMorales/SolarRadiometer/blob/master/Images/VoltagevsHourMarch23warrows.png alt="alt text" width=450 height=250>


<img src=https://github.com/GiorgioMorales/SolarRadiometer/blob/master/Images/LangleyPlots.jpg alt="alt text" width=550 height=250>
Fig. 6. Langley plots fitted using data from (a) March 22 and (b) March 23




## Usage

This repository contains the following folders and scripts:

* `Read_Solar.py`: Python script that reads the collected data (.CSV files from the `data` folder) and generates the Langley plots (Fig. 5 and 6).        
* `utils.py`: Python script that contains the functions used to calculate the zenith angle, distance from Sun to Earth, air mass correction, etc.
* `Arduino\SolarRadiometer.ino`: Arduino program that controls the servomotors and stores the data automatically thorough the day.
* `Hardware_Solidworks\`: Folder that contains the Solidworks files of the structure of the device. The subfolder `Files ready for printing` contains the files that can be directly 3D-printed. The design of the moving base was adapted from https://github.com/BrownDogGadgets/SolarTracker





## References
[1] Forrest M. Mims. Sun photometer with light-emitting diodes as spectrally selective de-
tectors. Appl. Opt., 31(33):6965–6967, Nov 1992

[2] Joseph Shaw. Lecture notes in remote sensing systems, EELE 583 - Spring 2022, Montana
State University.

[3] Glenn E. Shaw. Sun photometry. Bulletin of the American Meteorological Society, 64(1):4
– 10, 1983
