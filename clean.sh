#!/bin/sh
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done

rm serial.log
rm -rf sysroot
rm -rf isodir
rm -rf myos.iso
