In this folder you'll find a VisualBasic.NET wrapper for VS CAN API library. Following files and folders are provided:

* vs_can_api_wrapper.vb - VisualBasic.NET wrapper class
* api_doc.shfbproj - Sandcastle[1] configuration file to produce CHM documentation for VisualBasic.NET wrapper class
* example - folder containing TestDrive.vb file with a simple API usage

To use the class just create a new project with Visual Studio and add vs_can_api_wrapper.vb to this project. The same can be done with the TestDrive.vb - create a project and add both vs_can_api_wrapper.vb and TestDrive.vb. The TestDrive.cs is configured to open a first found device and using self reception mode, so if for example USB-CAN is installed properly and there is a functional connection between USB-CAN and your CAN bus (termination), the example program should run as it is.

[1] Sandcastle Help File Builder project: http://shfb.codeplex.com/
