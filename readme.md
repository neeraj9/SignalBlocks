# SigBlocks

The SignalBlocks is a c++11 framework to implement blocks of
logic which can be tied together like digital signal processing
(DSP) blocks (as in Matlab). This toolset aims to be fast, adaptable
and free (GPLv3 license) for the student/research community.

Please note that this is a humble attempt (no way complete) to
give a framework which is (probably) useful to the scientific
community.

The c++ can appear daunting for newbies but I strive to make those
as simple possible in the future. Additionally, there are so many
opensource libraries available which I intend to reuse for much faster
development.

    I know that the sample code is a lot cryptic at present,
    but this will improve over time.


> (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
> see LICENSE for license

## Roadmap


1. c++ blocks for a fast yet easy to use software modules for a
   generic processing. The idea is to have a basic framework
   which will use a lot of opensource libariries to present
   a blockset for mathematical (and probably more).
2. graphical based approach where users will use blocks as
   drag an drop, while the c++ engine is running in the
   background (more like Matlab Simulink).
3. Have some more thoughts but none mature enough to document.


## Motivation

I was always facinated by Matlab's power and ease of use while in
college and wanted to implement something similar, but in a language
which can be compiled to native format for blazingly fast performance.
This project is an attempt to give a basic frameowork which will (soon)
encapsulate many of the open source software to construct a building
block for such requirement.

## Blocks

    Blocks:

    csvfilesource - source of csvfile and generates a vector output of
                    string data type.
    constant (source of constant number)
    demux (one to many)
    summation (add values received so far)
    gain (multiply input with a constant factor)
    logical AND (output is boolean type)
    mux (many to one)
    product (multiply x and y giving z as output)
    relational operation (ex. <= on inputs x,y and give boolean output)
    saturation (with upper and lower limit)
    sum (x+y = z)
    switch (pass ip1 when ip2 satisfies some criteria (>= threshold) else pass ip3 to op1)
    terminator (dummy output port)
    delay (with some initial condition which is the initial output till delay is reached for first output)
    quantizer (quantization interval (ex. 0.5) is fixed at start and input is divided by that to get integer output)
    wrap to zero (if the input > threshold then output = 0, else output = input)
    difference (have fixed initial condition, y(n) = x(n) - x(n-1)
    buffer (convert serial data to a buffer of given size)
    nbuffer (convert serial data to a buffer of length N where N is within a given set and repeats over and over) - TODO
    downsample (filter data to down-sample input stream of data)
    upsample (filter data to up-sample the input stream and insert 0s) - TODO
    duplicator - duplicate the input into multiple outputs
    splitter - split a N dimmention vector to scalar which goes out to
               N output blocks.
    ordinalconverter - learn automatically and convert from string to integer,
                       where the id are automatically generated based on
                       inputs and continuously increases as new text strings
                       are encountered. It takes simple c++ string as input and
                       unsigned long as output (so its a MixedPort).


## Design Choices


### Flexible Input and Output Data

Each of the block is written such that it can receive either scalar, vector
or matrix data and mostly (unless there is a strong reason against it)
it accepts it. Additionally, the same block instance can receive different
type of data in its lifetime as well. There is no hard requirements against
it and not configuration option is provided to enforce it as well. This design
choice is made to allow complex scenarios (where changing dimension is required)
and avoid configuration. Instead this design lets the system learn the data
dimension automatically. The downside of this approach is that technically
the block can receive data of different dimensions (scalar, vector or matrix)
without any validation and needs to accept as it receives it. In case a
specific block cannot work in this manner then appropriate errors or assertions
should be enforced (but that is on a case-by-case basis).

At present there is no restriction on the dimensions of data passed each time
to the same block, while this is a lot flexible but can be an issue for an
incorrectly written block. (hint: see Buffer block)


## TODO

* Include a bulk of Gnu Scientific Library vector operations
* Test all socket operations tcp, udp and udplite
* Enhance bulk of functionality with unit tests

## Thanks

I have a lot of strategies to make this library even more useful to
students/researchers, while this is one humble attempt to go
one step closer towards the final goal.

Please write comments / send me an email since a lot needs to be
achieved and this is just the beginning.
