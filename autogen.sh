#!/bin/bash

libtoolize
aclocal
autoheader
automake --include-deps --add-missing --copy
autoconf
