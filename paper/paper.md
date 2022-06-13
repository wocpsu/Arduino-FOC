---
title: 'SimpleFOC: A Cross-platform Field Oriented Control (FOC) Library for Controlling Brushless Direct Current (BLDC) and Stepper Motors'
tags: 
  - Motor Control
  - Dynamical Robot Control
  - Applied Control Theory
  - cpp
authors:
  - name: Antun Skuric^[corresponding author] # note this makes a footnote saying 'co-first author'
    orcid: 0000-0002-3323-4482
    affiliation: 1 # (Multiple affiliations must be quoted)
  - name: Hasan Sinan Bank^[co-first author] # note this makes a footnote saying 'co-first author'
    orcid: 0000-0002-0626-2664
    affiliation: "2,3"
  - name: Richard Unger
    affiliation: 4
  - name: Owen Williams
    affiliation: 4
  - name: David González-Reyes
    orcid: 0000-0002-1535-3007
    affiliation: 4
affiliations:
 - name: INRIA Bordeaux, AUCTUS team, Université de Bordeaux
   index: 1
 - name: California State University, Chico
   index: 2
 - name:  Craftnetics Inc.
   index: 3
 - name: Community Member
   index: 4
date: 12 December 2021
bibliography: paper.bib
---

# Summary

Field Oriented Control (FOC) is a well-known strategy for controlling different types of electrically commutated motors (e.g., aternating current based, brushless DC, etc.) via phase-commutation. As opposed to the widely used brushed DC motors, which are by design mechanically commutated, brushless motors (e.g., BLDC, stepper, AC, etc.) rely on the control algorithms and electronics to create the appropriate magnetic fields and ensure motors' desired motion. Many techniques [@jalili2009investigation; @bida2018pmsm] have been developed over the years, of which FOC is arguably one of the most efficient ones. However, the FOC approach has relatively complex control architecture (see \autoref{fig:control}) requiring substantial computational performance. Since the motion control applications are implemented on embedded systems with limited capabilities, various optimizations and simplifications are usually necessary. This makes the FOC implementations almost exclusively specific to certain microcontroller architectures [@belhamel2020model; @cheles2008sensorless], motor drivers, current and position sensors, and motors [@castiglia2018high; @carey2019hybrid; @reddy2016soc].

SimpleFOC has been developed in an effort to provide a more generic and easy-to-use implementation of the FOC method to bolster the rapid development of highly dynamic cyber-physical systems (e.g., control theory experimental setup, dynamic robotic systems, etc.). 

![Overview of a typical motion control architecture based on the Field Oriented Control method. \label{fig:control}](control.png)

SimpleFOC implements the FOC algorithm routines, motion control strategies, generic hardware interfaces, and various configuration parameters encapsulated in an object-oriented C++ library. It provides the users with an intuitive way to develop their motion control application and the possibility to change all the hardware components (motor, sensors, drivers, microcontrollers) with relatively minor code modifications. SimpleFOC supports various RISC-based microcontroller architectures such as Atmega328/2560, MK20 series, STM32 series, SAMD series, RP2040 series and Xtensa LX6, as well as different platforms such as Arduino UNO, Nucleo, ESP32, Teensy, Portenta, to name a few.  Like many other open-source platforms [@chitta2017ros_control], SimpleFOC takes advantage of using existing software toolchains to facilitate the interoperability of the hardware to use the FOC approach with different microcontrollers. 



# Modular Architecture

As shown in \autoref{fig:scheme}, SimpleFOC provides a modular implementation of the FOC control architecture by intuitively dividing the code into the generic blocks representing different hardware and software components, such as motors, drivers, position sensors, current sensing, user interfaces, and finally the microcontroller specific code.  Each block in \autoref{fig:scheme} represents the operational necessities of the FOC and motion control ( see \autoref{fig:control} ), as well as all the initialization, calibration, communication functionalities specific to the hardware. For each of the generic blocks, SimpleFOC provides multiple cross-platform implementations based on different hardware specifications and in this way supports many motor, sensor, driver, and microcontroller combinations. 

The position sensor blocks are based on the Sensor class, and SimpleFOC provides initial support for hall sensors (*HallSensor*), magnetic sensors (*MagneticSensor*), and quadrature encoders (*Encoder*). Two motor driver classes are supported: BLDC drivers (*BLDCDriver3PWM, BLDCDriver6PWM*) and stepper drivers (*StepperDriver4PWM, StepperDriver2PWM*). The current sensing blocks are based on the CurrentSense class and provide the initial support for the strategies such as inline current sensing (*InlineCurrentSense*)  and low-side current sensing (*LowSideCurrentSense*). The motion control strategies have been implemented in the BLDC motor (*BLDCMotor*) and stepper motor (*StepperMotor*) classes. The motion control classes implement numerous strategies of motion and torque control, \autoref{fig:control} shows the implemented position control strategy. SimpleFOC additionally provides several user interfaces such as highly customizable commander (*Commander*), motor variable monitoring functionality, and the step-direction interface (*StepDirListener*).

The goal of this modular architecture is not only to facilitate the prototyping and design process for users, but also to allow for easier integration and support of new hardware components.

![ SimpleFOC modular architecture overview \label{fig:scheme}](architecture.png)

# The Documentation and a Sample Workflow
Similar to existing documentation frameworks (e.g., Diátaxis, @procida2021), SimpleFOC library is explained with tutorials, how-to guides, and additional references from the literature by using Jekyll-based Just-to-Docs. With that, the intention is to demystify FOC for solving motion control challenges for a wider community including the research and teaching audience. Many examples in the documentation ease the use of SimpleFOC and introduce the users to motion control principles of highly dynamic cyber-physical systems. A user can easily set up the SimpleFOC library to control the target hardware’s motion by following the workflow in \autoref{fig:setup}.

![Simplistic workflow to control BLDC motors with simpleFOC \label{fig:setup}](setup.png)

# Research Interest
SimpleFOC has been a valuable base for several research projects, such as motion control of a five-bar robot arm [@gonzalez2021metodologia], for synthesizing the head motion for a morphable robotic face [@lalitharatnesynthesizing] and for researching the feasibility of wireless embedded BLDC solutions [@rohman2021wireless]. Additionally, SimpleFOC has been used for investigating the floating-point precision influence on the control quality of AC Motors [@magnani2021impact]. Furthermore, SimpleFOC has been introduced in educational materials and books [@birglen2016mecatronique]. Therefore, the modularity and the configurability of SimpleFOC make it an interesting tool for different levels of educational projects while facilitating the testing and development of highly dynamic physical systems.

# Statement of Need
Building highly dynamic systems for research, development, or teaching purposes requires a specific type of actuator that is dynamic enough to respond to the changes of the system under interest [@seok2012actuator]. The most promising actuator type, capable of delivering very high torque density, minimizing the effect of the perturbations while working in a relatively wide bandwidth, are the FOC controlled BLDC motors. The actuators that can facilitate these requirements [@katz2018low] are either too arduous to set up and control for using research or teaching purposes or relatively expensive and complicated to improve due to their proprietary nature. On the other hand, open-source solutions provide limited control features, are often not well-documented and specific to certain hardware platforms. These limitations point to the need to develop a widely accessible, open-source solution to ease the building or development of dynamic systems for research and teaching. The presented library, SimpleFOC, facilitates a well-documented solution for various types of well-known and widely used microcontroller platforms, drivers, motors, and sensors. By the time of this writing, SimpleFOC has more than 250 active forks in GitHub, 600 members in its community platform with over 800 discussions, and 10000 posts exchanged between its members.

# Acknowledgments
The authors would like to thank the community for the continuous and generous support of the SimpleFOC project.

# References
