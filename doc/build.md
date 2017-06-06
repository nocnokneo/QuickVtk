QuickVtk
===
Build Instructions
----

QuickVtk can be built for Mac OS X and Windows via **[CMake](https://cmake.org)**. Building the project takes three steps:

### Step 1 - Install the Qt binaries

Download and launch the installer from the **[Qt download section](https://www.qt.io/download/)**.

### Step 2 - Build VTK

**[VTK](http://www.vtk.org)** can be built as a separate project from the **libs** folder located in the project directory. Launch **[CMake](https://cmake.org)** and set the paths to src and bin:

- **src**: QuickVtk/libs/src
- **bin**: QuickVtk/libs/bin

<center>
	<img src="img/cmake2.png"/>
</center>
You can specify a git tag to build a certain version of **[VTK](http://www.vtk.org)**. Configure, generate and build the project.

Alternatively, you can build **[VTK](http://www.vtk.org)** manually or use already built binaries from your system if available. Just make sure to use  **OpenGL2** for the **VTK\_RENDERING\_BACKEND** option and enable the following options:

- Module_vtkRenderingExternal
- Module_vtkIOGeometry
- Module_vtkIOPLY
- Module_vtkFiltersModeling
- Module_vtkFiltersTexture
- Module_vtkImagingGeneral
- Module_vtkInteractionWidgets

### Step 3 - Build QuickVtk
After successfully installing **[Qt](http://www.qt.io)** and building **[VTK](http://www.vtk.org)**, launch **[CMake](https://cmake.org)** and set the paths to src and bin:

- **src**: QuickVtk
- **bin**: QuickVtk/bin

<center>
	<img src="img/cmake1.png"/>
</center>

Specify the version and path attributes for **[Qt](http://www.qt.io)** and **[VTK](http://www.vtk.org)** and finally configure, generate and build the project. 

From a command line using the [Ninja](https://ninja-build.org/) generator this looks like:

```sh
cmake ~/git/QuickVtk \
  -GNinja \
  -DQUICKVTK_QT_VERSION=5.9.0 \
  -DQUICKVTK_VTK_VERSION=7.1.0 \
  -DQt5_DIR:PATH=~/Qt/5.9/gcc_64/lib/cmake/Qt5 \
  -DVTK_DIR:PATH=~/VTK-build
```

Tested project generators are:

- Unix Makefile (recommended for building VTK on Mac OS X)
- XCode (recommended for building QuickVtk on Mac OS X)
- Microsoft Visual Studio 2015 (for Windows)
- Ninja (tested on Linux)

**Note when building with Visual Studio:**
Make sure to set the environment in the IDE. Add the paths for **[Qt](http://www.qt.io)** and **[VTK](http://www.vtk.org)** to the *Environment* field found in the project properties view under *Debugging*:

<center>
	<img src="img/vs1.png"/>
</center>

Copy-Paste-Helpers:

path-to-vtk: **C:\Dev\QuickVtk\libs\bin\Vtk\bin\bin\Debug**   
path-to-qt: **C:\Qt\5.7\msvc2015_64\bin**

Those paths vary depending on the project location, Qt installation, build type, MSVC version, compiler type, etc. In the end, the value for *Environment* should look like this: 

**PATH=**\<path-to-vtk\>**;**\<path-to-qt\>**;%PATH%;**

