# Random utilities

This is my collection of random command-line utilities that really should have
been included with the system. At some point in the future, I may submit some
of these as patches to some of the more popular shells.

All of these programs are in the public domain and may be used freely.

## `commentize`

`commentize` converts a text file, or standard input if none is specified,
into one of several styles of block comment, with line breaks and formatting.
Its intended purpose is generating the licence comments which commonly appear
at the top of source code files. The styles of block comment which `commentize`
supports are as follows:

### Traditional C/C++/Java style (`c`)
```c
/*
 * Copyright (C) [year] [copyright holder]. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software") ...
 */
```

### Alternate C/C++/Java style (`c-alt`)
```c
// Copyright (C) [year] [copyright holder]. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software") ...
```

### Python/Shell style (`python`)
```python
###
# Copyright (C) [year] [copyright holder]. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software") ...
###
```

### Usage

`commentize [input file] [options]`

If no input file is given, `commentize` will default to reading from standard
input. Please note that if invoked with no arguments whatsoever, the program
will fail with a usage message, for the sake of new users. If you wish to read
from standard input, write to standard output, and use all the default options,
please pass the single character `-` as an argument.

#### Options
  * `-o, --output` File to output to. If not given, defaults to stdout.
  * `-s, --style` Block comment style. See above for accepted values. Defaults
    to Traditional C/C++/Java Style (`c`).
  * `-n, --line-length` Line length, including formatting, at which to wrap
    the block comment. Defaults to 80 characters.
