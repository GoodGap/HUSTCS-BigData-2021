#!/bin/sh

EXE=`basename $0`
DIR=`dirname $0`

if [ "$EXE" = "cc1" ]; then
  TOP="../../../.."
elif [ "$EXE" = "cc1plus" ]; then
  TOP="../../../.."
elif [ "$EXE" = "ld" ]; then
  TOP="../.."
elif [ "$EXE" = "ld.bfd" ]; then
  TOP="../.."
elif [ "$EXE" = "riscv64-unknown-elf-ld" ]; then
  TOP=".."
elif [ "$EXE" = "riscv64-unknown-elf-ld.bfd" ]; then
  TOP=".."
else
  echo "unknow $EXE"
  exit 1
fi

TOP=`cd "$DIR/$TOP"; pwd`
if [ ! -d "$TOP/1804" ]; then
  echo "not found 1804"
  exit 1
fi

#echo "------ wrap $EXE ------"

export LD_LIBRARY_PATH="$TOP/1804"
"$TOP/1804/ld-linux-x86-64.so.2" "$DIR/$EXE-orig" $@

