#PDF to Image
To easily and quickly convert PDF document to images.

##Snapshots
- Windows 7

  ![Windows](/screenshots/windows.png)

- Archlinux (Xfce4)

  ![Archlinux](/screenshots/archlinux.png)

##Release Note

###Download
- **[GitHub](https://github.com/limingjie/pdf_to_image/releases)**
- **[Baidu Cloud](http://pan.baidu.com/s/1ntrDD7z)**

###v0.2.1pre
- [Issue #2] Specify export directory.

###v0.2
- Open password protected PDF.
- New Large (600dpi) quality.
- [Windows] Static link to CRT, remove dll dependency.
- [Issue #1] Not valid Win32 application on Windows XP.

###v0.1
- Convert PDF to PNG images.
- Web (72dpi) and Print (300dpi) quality.
- Select pages.

##Manual Build
###Linux (g++)
- Create Development Directory (For linking MuPDF 3rd party libraries)
  ```console
  $ cd
  $ mkdir Development
  $ cd Development
  ```

- Build and Install FLTK
  ```console
  $ curl -O http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz
  $ tar zxvf fltk-1.3.3-source.tar.gz
  $ cd fltk-1.3.3-source
  $ make
  $ make install
  ```

- Build and Install MuPDF
  ```console
  $ curl -O http://mupdf.com/downloads/mupdf-1.7a-source.tar.gz
  $ tar zxvf mupdf-1.7a-source.tar.gz
  $ cd mupdf-1.7a-source
  $ make build=release
  $ make build=release install
  ```

- Build PDF to Image
  ```console
  $ git clone --depth=1 https://github.com/limingjie/pdf_to_image
  $ cd pdf_to_image
  $ make
  ```

###Windows (Visual Studio 2013)
- Build FLTK (Optional, pre-built libraries provided.)
  - Download and extract http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz.
  - Install CMake, generate Visual Studio solution and project files.
  - Open Visual Studio solution.
  - In Properties, change Configuration -> C/C++ -> Code Generation ->
  Runtime Library to **Multi-threaded (/MT)** for following projects.
    - fltk
    - fltk_forms
    - fltk_gl
    - fltk_images
    - fltk_jpeg
    - fltk_png
    - fltk_z
  - Build these projects.

- Build MuPDF (Optional, pre-built libraries provided.)
  - Download and extract http://mupdf.com/downloads/mupdf-1.7a-source.tar.gz.
  - Open mupdf-1.7a-source\platform\win32\mupdf.sln.
  - In Properties, change Configuration -> C/C++ -> Code Generation ->
  Runtime Library to **Multi-threaded (/MT)** for all projects.
  - Build the solution.

- Build PDF to Image
  - Open vs2013\pdf_to_image.sln
  - Build the solution.

##Author
- [Mingjie Li](mailto:limingjie@outlook.com)
- [GitHub](https://github.com/limingjie)
