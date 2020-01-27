#!/bin/bash
rm -f  Makefile.gz
gzip -k Makefile
./src/gz-file-stream-test Makefile.gz /tmp/blabla > /tmp/Makefile.expand
tail /tmp/Makefile.expand

