# 1. Loading Imetronic output directly on Matlab

Because of exporting the Imetronic output data is an authentic pain, I wrote this matlab script (<b>loadIMetronicData.m</b>) to retrieve a dictionary of events logged by Imetronic (exercise lines executions, infra red beam-breaks). This works for the setups used in Herry Lab, but if you want to us it on other Imetronic setups: double check if it works!
The important files to save are the "exercises" (.xls) and the "data" files (.dat). 

To call the Matlab function you need to give:
- the path to the Imetronic exercise used.
- the path to the imetronic data files you want to analyze.
- the prefix of the saved filnames (like mouse ids; multiple files with same ids are ordered on the output according to file date).
- the dictionary of labels to retrieve (see below).

An example call:

\>\> outVar=loadIMetronicData('D:\Daniel\imeData\Exercises', 'D:\Daniel\imeData\electro\g15', {'397','398'}, {'_CSM','_CSP'});

The output outVar is a structure containing the extracted data for each .dat file processed. The trajetory data is reconstructed from infrared beam-breaks.

Note about the labels: the .dat files logs the timestamps and what happened in the setup. Whenever one exercise line is executed, the number of the line of the exercise is logged on the .dat file. Then it is impotant to add a label to the event you want to retrieve, and to do that
you add a label, like for example here we log the onset of a new trial using "_NEW":

![image](https://user-images.githubusercontent.com/28762337/224672649-e4ecca8e-3c8a-4344-b878-c2c86f5b1e24.png)

If you didn't have the labels added to the exercise at the time when you run it, you can add them before calling the function and it will work. 


# 2. Controlling external Reward ports and internal shock intensity of Imetronic.

Here I document the things you need to do in order to use Imetronic to interface with reward delivery ports. 

We customized the shuttleboxes adding water-ports (Sanworks, https://sanworks.io/shop/viewproduct?productID=1009) into the short walls. Use a 18 mm drill bit to drill through the plexiglass and fix the port inside.

To control the ports we use an Arduino Mega controlling an 8-channel mechanical relay module (AZDelivery). The reward port requires 5v (leds and nose-poke photogate) and 12v (valve) power supplies (AZDelivery). Connection map is as follows:

![image](https://user-images.githubusercontent.com/28762337/224666493-960b5899-537d-441e-8f04-bc3a97f3d3b6.png)

The 4 relays from below control the led and valve (x2) respectively. We also control the intensity of the shock using the relays from the top.
Load the arduino code (.ino) in the Mega. The arduino is listening whats happening in terms of the input TTLs to determine the ongoing trial type and launching different routines (for example, reward delivery or setting the shock intensity) and photogates crossing to detect nose-pokes. To log the nose-pokes you need to run the python "listener" of the serial port, that is also included in this repository. 





