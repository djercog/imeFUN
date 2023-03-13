# 1. Loading Imetronic output directly on Matlab

I wrote this matlab script to retrieve a dictionary of events logged by Imetronic (exercise lines executions, infra red beam-breaks). 
This works for the setups used in Herry Lab, but if you want to implement it on your setups: double check if it works!
The important files to save are the "exercises" (.xls) and the "data" files (.dat). 

Briebly, the .dat files logs the timestamps and what happened in the setup. Whenever one exercise line is executed, the number of the line of the exercise is logged on the .dat file. Then it is impotant to add a label to the event you want to retrieve, and to do that
you add a label, like for example here we log the onset of a new trial using "_NEW":

![image](https://user-images.githubusercontent.com/28762337/224672649-e4ecca8e-3c8a-4344-b878-c2c86f5b1e24.png)

# 2. Controlling external Reward ports and internal shock intensity of Imetronic.

Here I document the things you need to do in order to use Imetronic to interface with reward delivery ports. 

We customized the shuttleboxes adding water-ports (Sanworks, https://sanworks.io/shop/viewproduct?productID=1009) into the short walls. Use a 18 mm drill bit to drill through the plexiglass and fix the port inside.

To control the ports we use an Arduino Mega controlling an 8-channel mechanical relay board. The reward port requires 5v (leds) and 12v (valve) power supplies. 
Connection map as follows:

![image](https://user-images.githubusercontent.com/28762337/224666493-960b5899-537d-441e-8f04-bc3a97f3d3b6.png)


