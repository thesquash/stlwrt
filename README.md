# STLWRT

For anyone who does not know yet, STLWRT is *the* successor to GTK+ 2,
originally based almost entirely on the original code from GTK+ 2.  STLWRT
has deviated from the GTK+ 2 code, however, and continues to deviate.
Ultimately, the goal is to be able to run most GTK+ 2 and many GTK+ 3
applications using the same STLWRT library, while both types of applications
keep a conservative, traditional GTK+ 2-esque look and feel.

I emphasized the word "the" above because, as of right now, I do not know of
anyone else who has any actively maintained fork of any version of GTK.
If anyone who has such a fork happens to be reading this, please drop me
a line and let me know; I'd be interested in the design decisions you are
making and have made, and I hope we may be able to collaborate somehow.

## What's new?

Well, it's been a long time since I tried to compile STLWRT, so I tried to
compile it again.  The last time I tried to compile it, I got over 100 MB
of warnings dumped into my error log file.  I try-compile it using the
command:

        gcc -pipe -o libstlwrt.so -fPIC -shared -I include \
         -DSTLWRT_COMPILATION `pkg-config --cflags --libs \
         glib-2.0 pango cairo atk gdk-pixbuf-2.0` src/*.c \
         2>error_dump.log

(Note:  To do this yourself, you need to create the files "gdkconfig.h" and
"config.h" in the `include` directory in the source.  I just make them
empty files for now; I have too much to worry about right now to bother
making a real configuration file for either.)  This time when I try-compiled
STLWRT, after making assorted modifications, the error log is down around
57 MB!  *I'm* certainly optimistic about the figure.

If that figure still sounds daunting to you, consider that it's pretty much
the same old error messages repeated over and over again.  (They mostly crop
up in the header files, which get used thousands of times.)  Not to mention
that modern versions of GCC produce very informative, but very verbose error
messages, as opposed to the single-line or double-line error messages of the
past.  Here is a sample error message actually extracted from my error dump
which, though apparently multiple errors, is actually just one:

        In file included from src/gdkapplaunchcontext.c:25:
        include/stlwrt.h:400:11: error: redefinition of 'struct _GdkScreenProps'
          400 |    struct _##TN##Props \
              |           ^
        include/gdkscreen.h:45:1: note: in expansion of macro 'STLWRT_DECLARE_VTYPE_FPARENT'
           45 | STLWRT_DECLARE_VTYPE_FPARENT(GdkScreen, gdk_screen, GObject,
              | ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
        include/stlwrt.h:400:11: note: originally defined here
          400 |    struct _##TN##Props \
              |           ^
        include/gdkscreen.h:45:1: note: in expansion of macro 'STLWRT_DECLARE_VTYPE_FPARENT'
           45 | STLWRT_DECLARE_VTYPE_FPARENT(GdkScreen, gdk_screen, GObject,
              | ^~~~~~~~~~~~~~~~~~~~~~~~~~~~

That one error takes up 651 bytes.  *If* all of the errors were that size
(which they are not -- some are slightly larger, most are smaller), then
there would be very roughly 87,000 errors in STLWRT that the compiler
detected.  Still not peaches and cream, but not 57 million errors, certainly.
Furthermore, the same header files keep getting included over and over again,
and some of the header files contain multiple errors which keep showing up.
An example:

        include/gdkwindow.h:737:53: error: unknown type name 'GdkWindow'; did you mean 'GdkWindowEdge'?
          737 | void       SF(gdk_offscreen_window_set_embedder)   (GdkWindow     *window,
              |                                                     ^~~~~~~~~
              |                                                     GdkWindowEdge

This error appears literally a few hundred times in a row -- *each time*
`gdkwindow.h` is included!  The following command limits the message content
to only the kinds of messages that, say, GCC 4 would have produced:

        sort -u error_dump.log | grep -E '^[A-Za-z_/]' | grep -E '(warning|error):' | wc -l

This somewhat more accurate appraisal removes duplicate error reports and
returns 8,307 errors and warnings.  Again, it's a rough approximation.
Again, 8,307 errors and warnings is not going to be a picnic!  But did
anybody expect it would be?

Either way, I'm still optimistic.  My apologies to those who were worried
about my long absence.

**UPDATE:**  A case in point:  Take the time to look at the commit history.
I just made minor modifications to three files and reduced the log file
size to 48.6 MB!
