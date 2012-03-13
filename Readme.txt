This code was created from Ogre Basic Tutorial 1 and the Ogre Character sample demo. Walking was abstracted from Ogre Intermediate Tutorial 1.

Some of you may encounter a run-time error related to not finding Ogre dlls when you try to run the code. This is likely due to the error I talked about in class. While grading homework 1, I found that Visual Studio was not saving all of the settings. I have since discovered where the settings are saved and a work around. Among the files for this solution, you'll find a file: 

GameEngine.vcproj.DUG.Jan Allbeck.user 

Visual Studio seems to create a new version of this file for each machine and user that uses the code. (BTW, my laptop's name is DUG). This is a text file that holds the settings that are being a problem. To fix the issue, simply rename this file to reflex the computer and user that you are using. Then Visual Studio will read the file and use the need settings. 


