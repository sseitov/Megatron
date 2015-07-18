In this folder you'll find a C# wrapper for VS CAN API library. Following files and folders are provided:

* vs_can_api_wrapper.cs - C# wrapper class
* Doxyfile.doxyfile - Doxygen[1] configuration file to produce HTML documentation for C# wrapper class
* example - folder containing TestDrive.cs file with a simple API usage

To use the class just create a new project with Visual Studio or MonoDevelop[2] and add vs_can_api_wrapper.cs to this project. The same can be done with the TestDrive.cs - create a project and add both vs_can_api_wrapper.cs and TestDrive.cs. The TestDrive.cs is configured to open a first found device and using self reception mode, so if for example USB-CAN is installed properly and there is a functional connection between USB-CAN and your CAN bus (termination), the example program should run as it is.

[1] Doxygen project: http://www.stack.nl/~dimitri/doxygen/
[2] MonoDevelop project: http://monodevelop.com/