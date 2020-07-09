# STLWRT

This is the pre-alpha version of STLWRT, the graphical toolkit based on [GTK+](
https://www.gtk.org) 2 which aims to emulate GTK+ 3 and later versions as
necessary.  The user is presented with a traditional interface; the application
is presented with whatever application programming or binary interface it
prefers.  I have released this in hopes that I can obtain help from the
community and to prove to others who have challenged this project in the past
that this project is not "vaporware".

However, due to time constraints I am not able to release the full glory of this
document right now; I am only able to give pointers to where to find things,
what STLWRT is missing right now, et cetera.  Please do not expect much from
STLWRT now, as it's a complex project and honestly, I've only just begun this
journey.  Please do not attempt to even compile STLWRT right now; it's got a lot
of work to do before it can compile.  I've just been pressured to release this
stuff before it's really ready, and I encourage frank feedback about this
software (for example: "You really could be doing it better if you put the GTK
emulation in...", or "There's a better way to do..." -- not "This stinks!" or
":lol:", please.)

## The directory layout

 * `icons` is where the PNG-format stock icons for STLWRT are stored.  This is
   worthy of note by itself:  GTK+ 2 had built-in icons; for a time GTK+ 3 did
   too, but no longer.  I personally think icons are an integral part of GTK and
   find that GTK+ 3 without icons is dismal, so I kept the icons from GTK+ 2.
   In fact, I actually intend on carrying over the default icons from GTK+ 2.10,
   because they fit in better with the default theme (formerly Raleigh; in
   STLWRT it's just called Default) than the newer default icons do, in my
   opinion.  You can already see quite a few of the old-style icons in STLWRT.
   This is something I'd like input from the community on: old versus new icons.
 
 * `include`, like in many projects, is where the C header files for STLWRT are
   stored.  Now, *STLWRT is not GTK*.  This is an important point.  STLWRT can
   emulate many versions of GTK, and they are not the same.  We need to put a
   facade in front of the applications and make the applications think they're
   using GTK even though they're using STLWRT.  So the header files that are
   actually used inside STLWRT are under `include/private`.  These should never
   get installed with STLWRT, only the header files under `include/public`
   should get installed and used by applications.  `include/private` is
   subdivided into `stlwrt` and `backends`, the latter of which contains three
   more subdirectories:  `x11`, `directfb` (the DirectFB backend came from GTK+
   2 and I anticipate using it as a template for Wayland), and `common` (stuff
   that could potentially be used between backends, but is specific to backends
   themselves, not the rest of STLWRT).  The main difference between the public
   and private header files right now is the names of some functions, as
   described below.
 
 * `m4`:  Various m4 macros used by the configuration process of STLWRT.  The
   whole build process of GTK+ 2 was messed up, and I plan on converting to
   Meson and Ninja instead of GNU Autotools, which I note have been very
   sluggishly maintained lately.  It seems the whole GNU Project is starting
   to fall apart and get replaced by independent, (relatively) uncoordinated
   open-source programmers.  That's not derogatory, that's my unbiased
   perspective.
 
 * `modules`:  This holds the code for various STLWRT modules, some of which
   probably don't need to be here.  This includes the Pixbuf-based theme engine
   and several print backends, all copied directly from GTK+ 2.  GAIL is
   missing but I'm not really sure what good it did anybody after all; if
   anybody knows, please tell me.
 
 * `po` and `po-properties` contain translation stuff.  Which brings me to
   another point:  I'm going to need translators for STLWRT, because there will
   be a full configuration tool with lots of options, and a few of the old GTK
   messages made no sense as they were, so those need to be fixed too.
 
 * `public-libs`:  This is the code for the wrapper libraries, the libraries
   that actually emulate one version of GTK or another.  These libraries should
   eventually contain code to "wrap around" functions in STLWRT -- with
   functions named after the original GTK functions which call the appropriate
   function in STLWRT.  Why the indirection?  In GTK 4, there are plans to
   change the `gtk_init()` function so that it doesn't accept any parameters
   anymore.  I anticipate more functions will be changed like this in the
   future, so I'm building in support to call different functions in the STLWRT
   library depending on what version of GTK needs to be emulated.  See the next
   bullet point for more information about this stuff.
 
 * `source`:  This is the main STLWRT library itself.  Publicly-accessible
   functions (functions intended for use by applications) names should take the
   following form:  They should be preceded by version information enclosed in
   underscores.  If the function is to be used no matter what version of GTK is
   to be emulated, then the function should begin simply with two underscores.
   So the `gtk_main()` function should be named in the STLWRT library
   `__gtk_main()`.  But the `gtk_init()` function, whose prototype changed in
   GTK 4, would be split into two functions:  One, the older form, would be
   called `_T3_gtk_init`, to show that it is the one to use in code compiled for
   GTK+ 3 and earlier; the other, new form, would be called `_4T_gtk_init`, to
   show it's the form used by GTK 4 and later applications.  The wrapper library
   to emulate GTK+ 2 would contain a function which simply calls `_T3_gtk_init`;
   the wrapper library for GTK+ 3 would also call `_T3_gtk_init`; and the GTK 4
   wrapper function for `gtk_init` would call `_4T_gtk_init`.  A 'T' indicates
   the function is used for GTK wrappers emulating GTK versions (left operand)
   *Through* (right operand).  If the left operand is omitted, zero is assumed.
   If the right operand is omitted, infinity is assumed.  If a change is
   reverted, for example the GTK developers get some sense and return to the old
   style of `gtk_init()` calls in GTK 5, the GTK 4 version will be called
   `_4T4_gtk_init` (because it only applies to GTK 4) and the old form will be
   called `_T3N5T_gtk_init`, since this is the form used by GTK 0 *Through*
   GTK 3 *aNd* GTK 5 onwards.  The format may look awkward, but it's supposed
   to be easily machine-readable so that a program can *generate* part of the
   GTK wrapper code -- eventually -- given the STLWRT library itself.
   
 * `themes`:  Visual and keyboard layout themes.  This includes the default
   theme Default (formerly Raleigh), and some keyboard themes like Default
   (the default keyboard theme), Mac (the keyboard theme for Mac keyboards),
   and Emacs (the keyboard theme which emulates Emacs -- don't ask me what that
   means).  These are copied straight from GTK+ 2.
 
 * `ui`:  This is the GTK Builder / Glade user interface files for various
   complex widgets which have alternative makeovers, such as the file chooser.
   One user requested a Windows 2000-style file chooser.  Like everything else,
   this directory is not complete yet.
   
 * `utilities`:  A bunch of utilities, useful or not, which relate to STLWRT or
   GTK in some way or another.
 
Peruse the code for five minutes and you'll see STLWRT has a long, long way to
go before it's production-ready.  Maybe July 10th isnt realistic, after all...
I actually had it working somewhat a while ago, but I decided to dive into
fixing everything all at once, which is why STLWRT is where it is.  But the
fact is GTK+ 2 was kind of poorly structured; it worked, but if you actually
want to change any code, that's tough.  STLWRT I hope will be more maintainable
thanks to its wildly different source code layout.

## The future

Currently the biigest problem for STLWRT is that:  In GTK, GTK objects are
structures which contain their parent object as a member of the structure.  In
GTK+ 2, it was not uncommon to insert several properties of the object in the
structure itself.  In GTK+ 3 and later, putting properties into the structure
became a no-no, so the structures got smaller.  GTK+ 3 applications and modules
expect the structures to be the smaller size; GTK+ 2 applications and modules
(including theme engines) often access the public members of the structures
and would crash if the public members weren't present.  My current idea is:  GTK
objects can have a special "private area" to themselves which goes along with
the instance of an object.  My idea is to create an all-encompassing
StlwrtObject which all STLWRT objects, widget or not, inherit from.
StlwrtObject's contructor would supersede the default constructor for an object
and would actually allocate *two* instances of an object:  the "fat" GTK+ 2
version and the "skinny" GTK+ 3 version.  Then add a private area to both
instances whose declaration looks like this:

        struct StlwrtObjectPrivate
        {
          gpointer *fat_instance;
          gpointer *skinny_instance;
        };

The private areas would be identical between the fat instance and the skinny
instance so that, no matter which size of object you get, you can always get to
the one you prefer by using:

        StlwrtObjectPrivate *private = G_TYPE_INSTANCE_GET_PRIVATE(widget,
                                           STLWRT_TYPE_OBJECT,
                                           StlwrtObjectPrivate);
        
        /*
         * Now use private->fat_instance or private->skinny_instance, depending
         * on what version you need to pass to another function.
         */

The appropriate version of the object would then be passed off to the
application depending on what version of GTK the application wants to pretend
it's using.  The same goes for modules:  If a module was intended for use with
GTK+ 2, then pass the "fat" versions of objects to the module; otherwise, pass
the "skinny" versions of objects to the module.

## A closing note

You may think I'm crazy, but stop psychoanalyzing me and just read the code and
comment!  ;-)

Gordon N. Squash
June 15th, 2020
