# Cougs in Space Software Library
This is a collection of classes that are common to all subsystems. They are organized into the following folders:

* **Components** - a class that represents a real component of the subsystem. It may contain one or more drivers. For example: the thermistor class reads an ADC to get a voltage then does math to return a temperature. The class abstracts the ADC away to a single readTemperature function.
* **Drivers** - a class that interfaces with a hardware circuit. Each type of circuit has a separate interface class that allows multiple implementations of circuits share a common interface. For example: the ADC interface class has configureChannel and readChannel functions but may interface with the hardware ADC over I²C or SPI.
* **File Types** - a class that takes data input and translates it to a file type. For example: the image file class takes a array of pixels and writes a file with a header and that pixel array
* **Tools** - These are utility classes including debug printing, error defines, CRC computation, etc.