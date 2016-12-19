#!/bin/bash

if [ ! -f $1 ]; then
    echo "File not found: $1"
    exit 1
fi

HEXFILE="${1}.hex"
xxd -p -c 8 "$1" > "${HEXFILE}"
vi "${HEXFILE}"
xxd -r -ps "${HEXFILE}" "$1"
rm -f "${HEXFILE}"

