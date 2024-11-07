# Research to market

### Introduction

Imagine a group of friends enjoying a beer night together. One of them, Alex, is visually impaired. While his friends use the Beertracker HUB effortlessly to track their beer consumption, Alex faces significant challenges in interacting with the system. Current designs of Beertracker HUB do not account for individuals with visual impairments, which leaves people like Alex excluded from this shared experience. With accessibility becoming an increasingly important aspect of product design, addressing these challenges not only helps improve inclusion but also broadens the user base. Enhancing accessibility for visually impaired individuals is a meaningful pursuit, both from an ethical and market perspective, as it empowers a wider audience to fully engage with products. This research focuses on enhancing the accessibility of Beertracker HUB for blind users by proposing and evaluating practical improvements to the interface and interaction model.

### Research Question and Sub-Questions

**Main Research Question:** How can the Beertracker HUB be redesigned to make it more accessible for visually impaired users?

**Sub-Questions:**

1. What are the specific challenges faced by visually impaired users when interacting with the current Beertracker HUB interface?
2. What are the best practices for designing accessible interfaces for blind users?
3. How can audio feedback, tactile components, a braille display, and voice activation be integrated into Beertracker HUB to enhance usability?
4. What technologies are available to support accessibility in embedded and physical systems?

### Research Methodology

To answer the research question and sub-questions, a mixed-method approach was used:

1. **User Interviews**: Interviews were conducted with visually impaired individuals to identify the main challenges they face while using Beertracker HUB and similar products. These interviews aimed to gain qualitative insights into user needs, preferences, and pain points.
2. **Accessibility Testing**: The current Beertracker HUB was tested by both visually impaired and sighted users. Accessibility experts were also consulted to evaluate the system and provide detailed feedback on usability issues.
3. **Literature Review**: A review of existing literature on accessibility design, specifically for embedded systems and smart hubs, was performed. This helped identify best practices and successful design principles.
4. **Prototype Testing**: A prototype with proposed modifications was developed and tested by users to evaluate the effectiveness of these changes. The prototype included audio feedback, a braille display, and voice activation features to assist navigation.

### Findings and Evidence

The research revealed several key challenges:

- **Interface Complexity**: The current system heavily relies on visual cues, making it challenging for visually impaired users to interact with.
- **Lack of Feedback**: The Beertracker HUB lacks sufficient auditory or tactile feedback, which limits its accessibility for users who cannot rely on visual interaction.
- **User-Centered Improvements**: From user interviews, it became clear that blind users preferred clear, immediate auditory feedback when interacting with the system. Some users also suggested adding a braille display for tactile guidance and voice activation for hands-free interaction.

To address these challenges, a prototype incorporating the following features was developed:

- **Audio Feedback System**: Audio prompts were added to guide users through the interaction process, such as confirming actions like beer logging or navigating through menu options.
- **Tactile Buttons and Braille Display**: Physical buttons with braille labels were added, along with a braille display to provide real-time information, enabling users to perform basic functions without needing to see the screen.
- **Voice Activation**: Voice commands were integrated into the system, allowing users to navigate menus and log beers hands-free, providing an additional layer of accessibility.

User testing of the improved prototype showed a significant increase in satisfaction and ease of use among visually impaired participants. The inclusion of auditory feedback, tactile components, a braille display, and voice activation made the system much more user-friendly and intuitive.

### Reactions to Different Buzzer Sounds

As part of the accessibility improvements, different buzzer sounds were tested to determine user reactions and preferences. The findings showed that the tone and type of buzzer sound could convey different types of feedback effectively:

- **High-Pitched Tone for Positive Actions**: A high-pitched, short buzzer sound was used to indicate positive actions, such as successfully logging a beer. Users responded positively to this sound, as it provided an immediate and clear indication of success.
- **Low-Pitched Tone for Errors**: A low-pitched, longer buzzer sound was used to indicate errors or invalid actions. This type of feedback helped users quickly understand when something had gone wrong, allowing them to adjust their actions accordingly.
- **Rising Tone for System Ready**: A rising tone sequence was used to indicate that the system was ready for input. This sound was found to be effective in signaling users to proceed with their actions.
- **Falling Tone for Session End**: A falling tone sequence was used to indicate the end of a session or when the user had completed their interaction. This provided a clear and satisfying conclusion to the interaction.

The use of different buzzer sounds, combined with other feedback mechanisms, greatly enhanced the overall user experience by providing non-visual cues that were easy to interpret. This multi-modal approach helped ensure that blind users received clear and actionable feedback during their interaction with the Beertracker HUB.

### Proposed Improvements and Implementation

Based on the findings, the following improvements were proposed and documented on the portfolio website:

- **Audio Feedback Integration**: A module providing voice feedback for key actions, enabling blind users to understand what action has been performed.
- **Braille-Enhanced Physical Interface and Display**: Incorporating braille labels for all key buttons and a braille display for real-time system information, making the system more accessible.
- **Voice Activation Feature**: Adding voice commands to enable users to navigate the interface and log actions without the need for physical input.
- **Simplified Navigation**: Reducing the complexity of the menu to ensure that visually impaired users can quickly learn and use the system effectively.
- **Buzzer Feedback System**: Incorporating different buzzer tones to indicate positive actions, errors, readiness, and session end, providing users with immediate auditory cues that enhance interaction.

The implementation of these features is documented with code explanations, images of the modified interface, and video demonstrations showing blind users interacting with the improved prototype.

### Conclusion

This research addressed the main research question by demonstrating how the Beertracker HUB could be made more accessible for visually impaired users. The introduction of audio feedback, tactile components, a braille display, voice activation, and differentiated buzzer sounds significantly improved the usability of the system. Key findings showed that providing multiple forms of feedback—auditory, tactile, and voice—enabled a much more inclusive experience for blind users.

**Recommendations**:

- Future improvements could include developing a mobile app that interfaces with Beertracker HUB, providing personalized voice control and further enhancing accessibility.
- Additional research could explore the use of machine learning to predict user actions and provide anticipatory assistance, further reducing barriers for visually impaired users.

**References**: The research included a mix of up-to-date articles and books on accessibility design, case studies on accessible embedded systems, and guidelines from trusted organizations like the World Wide Web Consortium (W3C). Referencing was done consistently in APA style throughout the documentation.