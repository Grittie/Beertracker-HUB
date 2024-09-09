# Learning journal

## Week 1
In week one I tried to tackle a bunch of different aspects of the project. I choose the blueprint **Smart Time Registration.** and started planning out what I needed to create for this project. I have been keeping track of my tasks and ideas in Trello. 

<img src="../assets/images/learningjournal/trello.png" alt="Trello Board with tasks" width="500" />

I setup the project using Docker, created a venv for python so I could run my portfolio website locally and started working on the *Embedded* part of the project first.

I started creating my circuit in Fritzing and using the WeMOS which I later switched over to the ESP32-S3 after seeing that for the additional sensors and outputs I wanted to add still I would not have the available pins required on the WeMOS so I moved over to the ESP32.

<img src="../assets/images/learningjournal/wemostoesp.png" alt="Fritzing diagrams moving from WeMOS to ESP32" width="500" />

I then built the circuit on my breadboard and wrote the code for it, and I was able to read the UID of the HvA passes.

<img src="../assets/images/learningjournal/uidtermoutput.png" alt="Output of the serial monitor" width="500" />

I then moved my focus to writing the required documentation for the work I've done and shifted my focus on two other aspects of the project. The website and the User Experience Design. For the website I wanted to start with looking into how to make the database required for project and started making an ERD. This ERD still has to be worked on further and implemented. For User Experience Design I wanted to start learning how to use CAD and how to adapt CAD Designs to existing components.

I took an LCD to start learning this, I looked up the datasheet where I was able to find the dimensions of the device and started sketching that out in OnShape where I could then design an enclosement around it.

<img src="../assets/images/learningjournal/datasheetlcd.png" alt="Datasheet of the LCD" width="200" />
<img src="../assets/images/learningjournal/cadlcd.png" alt="Cadding of the enclosement" width="500" />


### Feedback
**🧰 System Requirements** 

**Reviewer:** Koen Lammers

**Feedback:** It looks like a realy good page for your system requirement, realy detailed and covers all the parts of your system. Be sure to check them to yes when you finish one and also add the description with code on how you solved the problem!

**Takeaway:** I should often revisit this page and see whether I've finished a requirement and properly document it when I do.

<br>


## Week 2
### Feedback
**🧰 Technical Documentations** 

**Reviewer:** Koen Lammers

**Feedback:** 

**Takeaway:** 

<br>

**🌐 Database** 

**Reviewer:** Melvin Moes

**Feedback:** Looks good except for some information is redundant. The datatypes should be excluded and the identifier should be underlined. (ERD)

**Takeaway:** I should change the ERD and remove datatypes and underline identifiers, these steps should be implemented in the EER instead only.