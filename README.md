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

I've been tied up with other issues for the past few months, so (humiliatingly)
I haven't been keeping this repo up-to-date very much, and I haven't put in too
much development time into STLWRT.  However, now I'm finally returning to a
coding spree here.

The latest is I've been reducing the warning and error count -- as usual.  I've
done this most recently by removing old code for GObject per-instance private
data, and have (mostly) implemented STLWRT so that it uses the new private data
mechanism provided by GObject.  I trimmed the compiler error list from 1.7 MB
to 1.2 MB, which isn't bad.  Yes, it would be better if it **just compiled now**
and you all *didn't have to keep waiting*, but life gets in the way...

If you want to take a shot at compiling it yourself, clone this Git repo, then
`cd` into the parent directory of the repo:

```
~/stlwrt $ git clone https://github.com/thesquash/stlwrt
~/stlwrt $ ls
stlwrt
~/stlwrt $
```

In this case, you're already in the parent, so there's no need to `cd`.

Now, create a directory called `include`, and create two empty, stub files
underneath the new `include` directory (as follows).  These files will
eventually be filled with useful values when I get the build system working.

```
~/stlwrt $ mkdir include
~/stlwrt $ touch include/config.h
~/stlwrt $ touch include/gdkconfig.h
```

Let's compile it now.  This is the command I use to compile STLWRT so that I
know how many errors and warnings I need to deal with:

```
~/stlwrt $ cc -Wall -pipe -o libstlwrt.so -fPIC -shared -Iinclude -Istlwrt/include -Istlwrt/include/x11 $(pkg-config --cflags --libs glib-2.0 pango cairo atk gdk-pixbuf-2.0) -DSTLWRT_COMPILATION -DGETTEXT_PACKAGE='"stlwrt"' stlwrt/src/*.c 2>error_dump.log
```

You can now view the error dump under the file `~/stlwrt/error_dump.log`.
