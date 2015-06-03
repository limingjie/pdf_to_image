#PDF to Image

##Build
###Create Development Directory
```console
$ cd
$ mkdir Development
$ cd Development
```

###Build and Install FLTK
```console
$ curl -O http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz
$ tar zxvf fltk-1.3.3-source.tar.gz
$ cd fltk-1.3.3-source
$ make
$ make install
```

###Build and Install MuPDF
```console
$ curl -O http://mupdf.com/downloads/mupdf-1.7a-source.tar.gz
$ tar zxvf mupdf-1.7a-source.tar.gz
$ cd mupdf-1.7a-source
$ make build=release
$ make build=release install
```

###Build PDF to Image
```console
$ git clone --depth=1 https://github.com/limingjie/pdf_to_image
$ cd pdf_to_image
$ make
```
