# SigBlocks

The Signal Blocks is a c++ framework to implement blocks of
logic which can be tied together like digital signal processing
(DSP) blocks (as in Matlab).

> (c) 2014 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>
> see LICENSE for license

## Motivation

I was always facinated by Matlab power and ease of use while in
college and wanted to implement something similar but in a language
which can be compiled to native format for blazingly fast performance.
This project is an attempt to give a basic frameowork which will (soon)
encapsulate many of the open source software to construct a building
block for such requirement.

## Blocks

    (inspired from matlab-simulink)
    blocks:

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
    serialize (convert buffer to serial data, or send one-by-one to the connected peer)
    downsample (filter data to down-sample input stream of data)
    upsample (filter data to up-sample the input stream and insert 0's) - TODO
