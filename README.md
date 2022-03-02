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

## Please donate

Keep reading, but if STLWRT sounds good to you, I would appreciate it
unimaginably if you'd even *consider* donating.

STLWRT is no simple project, nor is it easy to continue writing code for it.
I continue to push forward (albeit at a sluggardly pace) because I know it's
going to be good.

I was formerly opposed to asking for donations toward this project because I
figured it was unethical to ask for contributions before producing a significant
tangible product.  However, that was many, many months ago.  Now, I am faced
with real-life struggles, and sadly, it is now essential that I ask for this.
If I were to continue like I have been doing, developing slowly without any
compensation what-so-ever, I'm afraid I'd have to call it quits and end the
project.

***This message should not be interpreted as doom and gloom!***  I have plenty
of energy to spare to continue working on STLWRT, and I greatly appreciate any
and all assistance I have received as of yet regarding the code and technical
aspects of the project.  But this projects requires heavy lifting -- a lot of
it.  And if, and only if, I begin receiving donations, I can put aside my
ordinary "day jobs" (which cut far into the night too) and progress with STLWRT
at an unprecedented speed -- with enough force to **get it working finally**.

So, I'll say it one more time:  Any contribution, big or small, that you can
make, I will be unimaginably appreciative.  There are not words to describe it.
And I hope the same will be true for you, when STLWRT comes to fruition finally.
I hope I can bring STLWRT out of the status of absurdity and into the realm of
reality.

Right now I accept donations in Bitcoin:

<a href="bitcoin:34gs6QF1ZnszcaQmikSVTwWbPZ7zsMP6G1"><img src="data:image/png;base64,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" alt="34gs6QF1ZnszcaQmikSVTwWbPZ7zsMP6G1"/></a>

**34gs6QF1ZnszcaQmikSVTwWbPZ7zsMP6G1**

Once more, thank you for your extremely generous support!
