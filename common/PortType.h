// (c) 2016 Neeraj Sharma <neeraj.sharma@alumni.iitg.ernet.in>.
// see LICENSE for license

#ifndef SIGBLOCKS_PORTTYPE_H
#define SIGBLOCKS_PORTTYPE_H

namespace sigblocks {
    enum PortType {
        PORT_TYPE_SCALAR = 0,
        PORT_TYPE_VECTOR,
        PORT_TYPE_MATRIX
    };
}
#endif //SIGBLOCKS_PORTTYPE_H
