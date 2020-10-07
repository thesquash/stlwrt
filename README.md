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

In my last update, I explained that the compiler errors and warnings totalled
up to about 49 MB.  Now they total up to 1.7 MB -- not bad.  When compiled
today, GTK+ 2 displays over 100 KB of warnings.  Considering that STLWRT is
unstable software, that's really not too bad.

Even when you compile it with '-Wall', the log file is still only 1.8 MB --
that's great, considering all the warnings that '-Wall' turns on!  (Though
modern versions of GCC are a lot more verbose by default than they used to
be.)

Compiling (or attempt-compiling) STLWRT is quick compared to GTK+ 2.  On
recent hardware (I briefly tried it on borrowed 6-year-old hardware), it
takes less than a minute to (fail to) build!  For fun, you can try it too:

        gcc -Wall -pipe -o libstlwrt.so -fPIC -shared -I include -I \
         include/x11 -DSTLWRT_COMPILATION -DGETTEXT_PACKAGE='"stlwrt"' \
         `pkg-config --cflags --libs glib-2.0 pango cairo atk \
          gdk-pixbuf-2.0` src/*.c 2>error_dump.log

I'm getting close to real results, considering that the error log was 49 MB
just a few days ago.  Have fun!
