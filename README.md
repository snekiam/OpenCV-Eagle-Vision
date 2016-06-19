# OpenCV-Eagle-Vision
This is work in progress vision code for FRC3322 Eagle Imperium.
The goal is to check the conour similarity of the goal in the folder to one pulled from an image.  If they are withing a threshold,
calculate the azimuth (horizontal angle) to the center of the goal.  Hardware includes a raspberry pi running mjpg-streamer @ 432*240,
a logitech C615 webcam, and a laptop to compile the code.  Currently only outputs to command line through cout, but will eventually post
to networktables.
