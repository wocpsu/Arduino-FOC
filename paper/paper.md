---
title: 'SimpleFOC: A Field Oriented Control (FOC) Library for Controlling Brushless Direct Current (BLDC) and Stepper Motors'
tags: 
  - Motor Control
  - Dynamical Robot Control
  - Applied Control Theory
  - cpp
authors:
  - name: Antun Skuric^[co-first author] # note this makes a footnote saying 'co-first author'
    orcid: 0000-0002-3323-4482
    affiliation: "1,2" # (Multiple affiliations must be quoted)
  - name: Hasan Sinan Bank^[co-first author] # note this makes a footnote saying 'co-first author'
    orcid: 0000-0002-0626-2664
    affiliation: "3,4"
  - name: Richard Unger
    affiliation: 5
  - name: Owen Williams
    affiliation: 6
  - name: David Gonzalez
    affiliation: 7
affiliations:
 - name: Institut national de recherche en informatique et en automatique (INRIA), AUCTUS team
   index: 1
 - name: Université de Bordeaux
   index: 2
 - name: California State University, Chico
   index: 3
 - name:  Craftnetics Inc.
   index: 4
 - name: Institution Name
   index: 5
 - name: Institution Name
   index: 6
 - name: Institution Name
   index: 7
date: 07 December 2021
bibliography: paper.bib
---

# Summary

Field Oriented Control (FOC) is a well-known strategy for controlling different types of electrically commutated motors (e.g., Alternative Current based, Brushless DC, etc.) via phase-commutation. As opposed to the widely used brushed DC motors, which are by design mechanically commutated, the brushless motors (e.g., BLDC, stepper, AC, etc.) rely on the control algorithms and electronics to create the appropriate magnetic field in the motor to ensure its desired motion. Many techniques (Karman 2009, Bida 2018) have been developed over the years, from which FOC is arguably one of the most efficient ones. However, the FOC approach has several disadvantages, including complex control architecture, limited computational performance, and highly specific interfacing between different hardware components. Therefore today, even though the FOC algorithm has a very generic structure, most of its implementations are very specific, in many cases even proprietary (Belhamel 2020, Cheles 2008), to specific microcontrollers, motor drivers, current and position sensors, and motors (Castiglia 2018, Carey 2019, Reddy 2016)

SimpleFOC has been developed in an effort to provide a more generic and easy-to-use implementation of the FOC method to bolster the rapid development of highly dynamic cyber-physical systems (e.g., control theory experimental setup, dynamic robotic systems, etc.). 

SimpleFOC implements the FOC algorithm routines, generic hardware interfaces, and various configuration parameters encapsulated in an object-oriented C++ library. It provides the users with an intuitive way to develop their motion control application and the possibility to change all the hardware components (motor, sensors, drivers, microcontrollers) with relatively minor code modifications. SimpleFOC supports various RISC-based microcontroller architectures such as Atmega328/2560, MK20 series, STM32 series, SAMD series, RP2040 series, and Xtensa LX6 different platforms such as Arduino UNO, Nucleo, ESP32, Teensy, Portenta, to name a few.  Like many other open-source platforms \cite{chitta2017ros_control}, SimpleFOC takes advantage of using existing software toolchains to facilitate the interoperability of the hardware to use the FOC approach with different microcontrollers. 

As shown in Fig. 1, SimpleFOC provides a modular implementation of the FOC control architecture divided into generic blocks representing classes and hardware as motor, driver, position sensor, current sensing, user interface, and finally, the microcontroller specifics. 
The position sensor blocks are based on the Sensor class, and SimpleFOC provides initial support for hall sensors (HallSensor), magnetic sensors (MagneticSensor), and quadrature encoders (Encoder). Two motor driver classes are supported: BLDC drivers (BLDCDriver3PWM, BLDCDriver6PWM) and Stepper drivers (StepperDriver4PWM, StepperDriver2PWM). The current sensing blocks are based on the CurrentSense class and provide the initial support for the strategies such as inline current sensing (InlineCurrentSense)  and low-side current sensing (LowSideCurrentSense). The motion control classes have been implemented in the BLDC motor (BLDCMotor) and stepper motor (StepperMotor) classes. The motion control classes implement numerous strategies of motion and torque control. SimpleFOC additionally provides several user interfaces such as highly customizable commander (Commander), motor variable monitoring functionality, and the step-direction interface (StepDirListener).

Each block in Fig.1.b represents the operational necessities of the FOC (Fig.1.a) as well as all the initialization, calibration, communication functionalities specific to the hardware. This approach enables the several motors, sensor, driver, and microcontroller combinations. \footnote{The list of supported hardware can be found https://docs.simplefoc.com/supported_hardware}


# Statement of need

`Gala` is an Astropy-affiliated Python package for galactic dynamics. Python
enables wrapping low-level languages (e.g., C) for speed without losing
flexibility or ease-of-use in the user-interface. The API for `Gala` was
designed to provide a class-based and user-friendly interface to fast (C or
Cython-optimized) implementations of common operations such as gravitational
potential and force evaluation, orbit integration, dynamical transformations,
and chaos indicators for nonlinear dynamics. `Gala` also relies heavily on and
interfaces well with the implementations of physical units and astronomical
coordinate systems in the `Astropy` package [@astropy] (`astropy.units` and
`astropy.coordinates`).

`Gala` was designed to be used by both astronomical researchers and by
students in courses on gravitational dynamics or astronomy. It has already been
used in a number of scientific publications [@Pearson:2017] and has also been
used in graduate courses on Galactic dynamics to, e.g., provide interactive
visualizations of textbook material [@Binney:2008]. The combination of speed,
design, and support for Astropy functionality in `Gala` will enable exciting
scientific explorations of forthcoming data releases from the *Gaia* mission
[@gaia] by students and experts alike.

# Mathematics

Single dollars ($) are required for inline mathematics e.g. $f(x) = e^{\pi/x}$

Double dollars make self-standing equations:

$$\Theta(x) = \left\{\begin{array}{l}
0\textrm{ if } x < 0\cr
1\textrm{ else}
\end{array}\right.$$

You can also use plain \LaTeX for equations
\begin{equation}\label{eq:fourier}
\hat f(\omega) = \int_{-\infty}^{\infty} f(x) e^{i\omega x} dx
\end{equation}
and refer to \autoref{eq:fourier} from text.

# Citations

Citations to entries in paper.bib should be in
[rMarkdown](http://rmarkdown.rstudio.com/authoring_bibliographies_and_citations.html)
format.

If you want to cite a software repository URL (e.g. something on GitHub without a preferred
citation) then you can do it with the example BibTeX entry below for @fidgit.

For a quick reference, the following citation commands can be used:
- `@author:2001`  ->  "Author et al. (2001)"
- `[@author:2001]` -> "(Author et al., 2001)"
- `[@author1:2001; @author2:2001]` -> "(Author1 et al., 2001; Author2 et al., 2002)"

# Figures

Figures can be included like this:
![Caption for example figure.\label{fig:example}](figure.png)
and referenced from text using \autoref{fig:example}.

Figure sizes can be customized by adding an optional second parameter:
![Caption for example figure.](figure.png){ width=20% }

# Acknowledgements

We acknowledge contributions from Brigitta Sipocz, Syrtis Major, and Semyeong
Oh, and support from Kathryn Johnston during the genesis of this project.

# References