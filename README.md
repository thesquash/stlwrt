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

For the last few weeks I've been working on cleaning up the STLWRT code,
since the GTK+ 2 code it was based on was haphazardly written over a
period of over 20 years.  My latest achievement was STLWRT-izing all header
files (adding fat and thin versions of objects).  While I converted the
header files, I would notice a boatload of "forward declarations" --
instances of code duplication in which certain type declarations had to be
present before the header file in question could declare any new types,
but the prerequisite type declarations were in another header file which
depended upon the header file in question, creating a circular dependency.
To mitigate this spaghetti code, I plan on setting up all header files to
have distinct sections for each of preprocessor macros, type declatations,
structure declarations, and function declarations.  Each of these sections
will be surrounded by special preprocessor conditionals, so that a header
file can include *just the type declarations* and *nothing else*, for
instance.

An example:  Suppose the `gtkwidget.h` header requires only the type
declarations from `gtkwindow.h`, and wants nothing else from the header
file yet.  (This is a practical example, actually.  The `gtkwidget.h`
header includes several functions which accept parameters of type
`GtkWindow`, so currently a forward declaration is used.)  `gtkwidget.h`
can eliminate the forward declarations and instead use:

        /* A whole bunch of other stuff goes here, specifically GtkWidget
         * type definitions -- GtkWindow inherits from GtkWidget, so we
         * must declare GtkWidget before we import declarations and
         * definitions of GtkWindow. */
        
        #define __GTK_WINDOW_H_IMPORT_TYPES_ONLY__
        #include <gtkwindow.h>
        #undef  __GTK_WINDOW_H_IMPORT_TYPES_ONLY__
        
        /* Now comes the functions, some of which rely on GtkWindow */

Much easier to read and more portable to boot, since type declarations
only need to be defined once now.

And after we're done with that, it'll be time to convert the source code
to use special functions and macros to *define* the object types and to
read the object instance properties.

But after that's done, it'll be time to run a build of the whole thing
and see how many megabytes of warnings and errors we get.  The last time
I ran a build, I got over 100 MB of errors and warnings!
