#!/bin/bash
rm -f  Makefile.gz
gzip -k Makefile
./src/decompress-plain-db Makefile.gz /tmp/blabla > /tmp/Makefile.expand
tail /tmp/Makefile.expand

