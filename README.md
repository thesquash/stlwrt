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

Progress has been slow since the mid-to-late fall, as anybody who reads the
commit logs can see.  The stuff I've been doing lately is STLWRT-ization of
the roughly 260 source files.  Specifically, all STLWRT objects need to stop
using object instance structures directly and need to go through at least one
generated function.  As an example, the following hypothetical function
which packs a widget inside of a GtkDialog:

        void
        gtk_dialog_really_stupid_function (GtkDialog *dialog, GtkWidget *child)
        {
          g_return_if_fail (GTK_IS_DIALOG (dialog));
          g_return_if_fail (GTK_IS_WIDGET (child));
        
          gtk_container_add (GTK_CONTAINER (dialog->vbox), child);
        }

...needs to become:

        void
        gtk_dialog_really_stupid_function (GtkDialog *dialog, GtkWidget *child)
        {
          GtkDialogProps *dialog_props;
        
          g_return_if_fail (GTK_IS_DIALOG (dialog));
          g_return_if_fail (GTK_IS_WIDGET (child));
        
          dialog_props = gtk_dialog_get_props (dialog);
        
          gtk_container_add (GTK_CONTAINER (dialog_props->vbox), child);
        }

Of course, many functions are a lot longer than this, so the `dialog_props` can
be used multiple times in the same function, or maybe even shared between local
functions which have no need for ABI compatibility.  `src/gtkbbox.c` is an
example of a coding nightmare, something which was allowed to collect a lot of
bad coding practices over time.  A cursory look at GTK+ 1.0.0 source code
reveals that GtkButtonBox existed even back then in 1998, when GTK worked much
differently than it does now.  So in short, there are some files which are hard
to convert because they're full of inconsistencies, such as inconsistent
variable naming schemes.
