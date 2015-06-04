#PDF to Image

##Release Note
###v0.2
- Open password protected PDF.
- New High (600dpi) quality.

###v0.1
- Convert PDF to PNG images.
- Web (72dpi) and Print (300dpi) quality.
- Select pages.

##Build
###Linux (g++)
- Create Development Directory
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

###Windows (vs2013)
- Make sure FLTK and MuPDF are compiled with same type of run time library,
both /MD or /MT.
- Create a new solution include all .cpp and .hpp file.
- Add FLTK and MuPDF include path to compiler.
- Add FLTK and MuPDF library to linker.
