#! /bin/bash
$EXTRACTRC *.ui *.kcfg >> rc.cpp
$XGETTEXT *.cpp *.h -o $podir/pothumbnail.pot
rm -f rc.cpp
