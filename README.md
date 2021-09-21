# ATmega_One_Wire

This repository defines a package gathering software elements allowing to manage a 1-Wire bus
through a digital pin of an ATmega chip.

The package is designed following
[this meta-model](https://github.com/HomeMadeBots/Embedded_Software_Meta_Model) and implemented
according to [these C language
patterns](https://github.com/HomeMadeBots/C-language-patterns-for-Embedded-Software-Meta-Model).

## Content

The DS18B20_Ambient_Air_Temperature_Sensor package gathers :
* Component_Types :
  * ATmega_One_Wire_Interaction_Layer

## Overview

![Overview](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.github.com/HomeMadeBots/ATmega_One_Wire/master/doc/overview.puml)

## Dependencies

![Packages dependencies](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.github.com/HomeMadeBots/ATmega_One_Wire/master/doc/dependencies.puml)

The following repositories shall be retrieved :
* [One_Wire_Interfaces](https://github.com/HomeMadeBots/One_Wire_Interfaces)
* [Bits_Management](https://github.com/HomeMadeBots/Bits_Management)
* [Embedded_C_Framework](https://github.com/HomeMadeBots/Embedded_C_Framework)

## Use

### With the Arduino IDE

This repository shall be clone within the _libraries_ folder of the _Arduino sketchbook folder_.