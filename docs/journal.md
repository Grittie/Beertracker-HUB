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
<img src="../assets/images/learningjournal/cadLCD.png" alt="Cadding of the enclosement" width="500" />


### Feedback
**🧰 System Requirements** (solved)

**Reviewer:** Koen Lammers

**Feedback:** It looks like a realy good page for your system requirement, realy detailed and covers all the parts of your system. Be sure to check them to yes when you finish one and also add the description with code on how you solved the problem!

**Takeaway:** I should often revisit this page and see whether I've finished a requirement and properly document it when I do.

<br>


## Week 2
Week two started with writing out my learning journal and figuring out how to tackle the feedback and also writing out a project plan with weekly goals and aspirations. This week was a lot of work building the website and making the required steps to achieve it. I started with the database where I used UML to figure out what data was required of this project to be in a database and sketched out an ERD. When that looked good I started making the EERD in *MySQL Database* from which I could generate my database.

<img src="../../assets/images/web/database/EER.png" alt="EER of website" width="250" />


With the database up and running it was time to start working on the website, this had to be done with the PHP, JS and HTML + CSS for styling. Using PHP I could make query's and calls to my database and encode it withing JSON which JS can fetch and work out it's information with it. I created a connection check that checks if the website has connection to the database and created a table which prints out all the users and their associated pass UID. 

<img src="../../assets/images/learningjournal/website.png" alt="Screenshot of the website" width="500" />

To secure the website and database I didn't want to have my credentials hard coded into the websites code so I had to learn how to expand my docker to install composer from which I could then install php packages like DOTENV which made it possible for me to store my database credentials in an only local file so other people could not have access to it.

Having created a solid basis for the website I expanded my embedded part by adding a new output, the lcd screen. Getting it to work was a lot of trial and error. I used the i2c backpack which I had to solder unto the lcd screen itself. Using this I significantly lowered the amount of pins required to get the LCD display to work. My biggest struggle was to get the lcd display sufficiently powered since it requires 5v and the esp32 only has 3.3v output pins. This required me to use the 9v battery included in the IoT kit but not wanting to overpower the display I had to come to a solution to get the power from 9v to 5v for which I used a mini DCDC converter which gave me an output of 5v. 

<img src="add-image-of-dcdc-setup" alt="setup" width="500" />

Lastly I have gotten my esp32 connected to the wifi using a wifi manager which makes it so I again don't have any potential important info saved in the code. I sketched out some more designs for my device and thought of fun additions and created the basis for my API. Which will handle the data calls from device to device.

<img src="../../assets/images/learningjournal/sketch2.png" alt="second sketch of the device" width="500" />

### Feedback
**🧰 Technical Documentations** (unsolved)

**Reviewer:** Melvin Moes

**Feedback:** Looks good maybe add the prices to the component list. This way you could show a total price of making your system.

**Takeaway:** I should add a column with prices, even though Melvin did not mention it I should change the BOM to places where I can purchase one from instead of where I got it. I should remove the HvA as supplier for example.

<br>

**🧰 Technical Documentations** (unsolved)

**Reviewer:** Koen Lammers

**Feedback:** Be sure to add the url to the store pages! the rest looks clean and good!

**Takeaway:** I should make sure to add links to the suppliers in the BOM to redirect them to the product.

<br>

**🌐 Database** (unsolved)

**Reviewer:** Melvin Moes

**Feedback:** Looks good except for some information is redundant. The datatypes should be excluded and the identifier should be underlined. (ERD)

**Takeaway:** I should change the ERD and remove datatypes and underline identifiers, these steps should be implemented in the EER instead only.

<br>

**🌐 Technical Documentation** (unsolved)

**Reviewer:** Nick Schokker

**Feedback:** I like the structure of your technical documentation but i would say you can improve if you give a few visual example so its better to understand with the text

**Takeaway:**  I should add short and concise code snippet to the technical doc aswell to make it more readable.

<br>

## Week 3