# Smart Time Registration


## What is a Time Registration Device?
The Smart Time Registration device is a tool used to track the ammount of time an employee has spent working, it can take on many forms. Traditionally, this was a a physical time clock where employees would 'punch in and out' their card, recording their hours manually.

Nowadays, time registration devices are often available as websites or mobile apps. These tools can not only track overall work hours but also allow users to log time spent on specific tasks or projects. This tracking is then used for billing purposes, project management, and ensuring accurate payroll processing.

## Project Requirements
<img src="assets/images/initial_sketch.jpg" alt="Alt text" width="500" />

(image will be changed  when better sketches are available)

### Embedded Device
For this project, the WEMOS, along with other devices, will serve as the core component of the embedded product. Various sensors and peripherals, including an RFID scanner for scanning students' HVA IDs, buttons, and displays for data visualization, will be connected to the WEMOS.

#### The 3 minimum requirement for Input & Ouput are:
**INPUT:**

1. RFID Scanner

2. Buttons for increment and decrement (plus & minus)

3. On / Off Switch

**Output:**

1. LED Indicators

2. LCD Screen

3. Passive Buzzer

### Website

The website will implement user registration using HVA IDs and passwords. Users will be listed with their clock-in and clock-out times for the day. Further developments and applications of this data are to be determined.

### User Experience Design (CAD)

The device will be housed in a compact enclosure designed using CAD, with the software OnShape.

## Our own flavor
Once the blueprint requirements are met, the device will be changed adding our own flavor and features. For example, a counter registration system will be integrated to track beverages, which will be added to a user database when they scan their pass. The device will also be integrated into a already running project "BeerTracker". 

## What is BeerTracker?

BeerTracker is a website designed, made and managed by Lars Grit for his class at the University of Applied Science of Amsterdam to keep a track of how many beers have been bought and drank to avoid the classic tradition of "tikkies". Using the BeerTracker the class is made aware of how much people are chipping in to keep the drinks even.

During the implementation of this device the website will be heavily changed and updated.


{{ mdocotion_header('https://cdn.prod.website-files.com/65730eadde5e46a394ae1451/65730eadde5e46a394ae14ca_ES%20-%20Image%20home%202.jpg') }}