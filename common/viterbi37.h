/* The three generator polynomials for the NASA Standard K=7 rate 1/3 code */
#define	POLYA	0x4f
#define POLYB	0x57
#define	POLYC	0x6d


/* The basic Viterbi decoder operation, called a "butterfly"
 * operation because of the way it looks on a trellis diagram. Each
 * butterfly involves an Add-Compare-Select (ACS) operation on the two nodes
 * where the 0 and 1 paths from the current node merge at the next step of
 * the trellis.
 *
 * The code polynomials are assumed to have 1's on both ends. Given a
 * function encode_state() that returns the two symbols for a given
 * encoder state in the low two bits, such a code will have the following
 * identities for even 'n' < 64:
 *
 * 	encode_state(n) = encode_state(n+65)
 *	encode_state(n+1) = encode_state(n+64) = (3 ^ encode_state(n))
 *
 * Any convolutional code you would actually want to use will have
 * these properties, so these assumptions aren't too limiting.
 *
 * Doing this as a macro lets the compiler evaluate at compile time the
 * many expressions that depend on the loop index and encoder state and
 * emit them as immediate arguments.
 * This makes an enormous difference on register-starved machines such
 * as the Intel x86 family where evaluating these expressions at runtime
 * would spill over into memory.
 *
 * Two versions of the butterfly are defined. The first reads cmetric[]
 * and writes nmetric[], while the other does the reverse. This allows the
 * main decoding loop to be unrolled to two bits per loop, avoiding the
 * need to reference the metrics through pointers that are swapped at the
 * end of each bit. This was another performance win on the register-starved
 * Intel CPU architecture.
 */

#define	BUTTERFLY(i,sym) { \
	register int m0,m1;\
	/* ACS for 0 branch */\
	m0 = cmetric[i] + mets[sym];	/* 2*i */\
	m1 = cmetric[i+32] + mets[7^sym];	/* 2*i + 64 */\
	nmetric[2*i] = m0;\
	if(m1 > m0){\
		nmetric[2*i] = m1;\
		dec |= 1 << ((2*i) & 31);\
	}\
	/* ACS for 1 branch */\
	m0 -= (mets[sym] - mets[7^sym]);\
	m1 += (mets[sym] - mets[7^sym]);\
	nmetric[2*i+1] = m0;\
	if(m1 > m0){\
		nmetric[2*i+1] = m1;\
		dec |= 1 << ((2*i+1) & 31);\
	}\
}
#define	BUTTERFLY2(i,sym) { \
	register int m0,m1;\
	/* ACS for 0 branch */\
	m0 = nmetric[i] + mets[sym];	/* 2*i */\
	m1 = nmetric[i+32] + mets[7^sym]; /* 2*i + 64 */\
	cmetric[2*i] = m0;\
	if(m1 > m0){\
		cmetric[2*i] = m1;\
		dec |= 1 << ((2*i) & 31);\
	}\
	/* ACS for 1 branch */\
	m0 -= (mets[sym] - mets[7^sym]);\
	m1 += (mets[sym] - mets[7^sym]);\
	cmetric[2*i+1] = m0;\
	if(m1 > m0){\
		cmetric[2*i+1] = m1;\
		dec |= 1 << ((2*i+1) & 31);\
	}\
}
