#!/bin/sh

#
# Check the arguments.
#
if test $# -lt 2 ; then 
    echo "I need two arguments - the skeleton, and the type" 
    exit 1
fi

#
# Check the skeleton
#
skel="$1"
shift
if test ! -f $skel ; then 
    echo "No such file: $skel"
    exit 2
fi

#
# Check the type
#
type="$1" 
shift
low=_`echo $type | tr ' ' '_'`
sub="$type"
name=_`echo $type | tr ' ' '_'`
complex=0

#
# Check the normal argument from given type
#
extract_normal() {
    foo="$1"
    case "$foo" in 
	"long double")             bar=_long_double   ;;
        "unsigned long")           bar=_ulong         ;;
	"unsigned int")            bar=_uint          ;;
	"unsigned short")          bar=_ushort        ;;
	"unsigned char")           bar=_uchar         ;;
	"double")                  bar=               ;;
	"float")                   bar=_$foo          ;;
	"long")                    bar=_$foo          ;;
	"int")                     bar=_$foo          ;;
	"short")                   bar=_$foo          ;;
	"char")                    bar=_$foo          ;;
	*) 
	    echo "Unknown low-level type: $foo"
	    exit 1
	    ;;
    esac
    echo "$bar"
}

extract_blas1() 
{
    foo="$1"
    case $foo in 
	"double")                  bar=d          ;;
	"float")                   bar=s          ;;
	"complex<float>")	   bar=c	  ;;
	"complex<double>")	   bar=z	  ;;
	*)			   bar=		  ;;
    esac
    echo "$bar"
}
extract_blas2() 
{
    foo="$1"
    case $foo in 
	"complex<float>")	   bar=s	  ;;
	"complex<double>")	   bar=d	  ;;
	*)			   bar=		  ;;
    esac
    echo "$bar"
}


blas1=`extract_blas1 $type`
blas2=`extract_blas2 $type`
have_blas=0
if test "x$blas1" != "x" ; then 
    have_blas=1
fi
#
# Check if we need complex specialisation
#
double=0
case "$type" in
    complex*)
	complex=1
	sub=`echo $type | sed -e 's/complex *< *//' -e 's/ *>//'`
	if test "x$sub" = "xdouble" ; then 
	    double=1
	fi
	low=_complex`extract_normal "$sub"`
	name=_complex_`echo ${sub} | tr ' ' '_'`
	cname=complex_`echo ${sub} | tr ' ' '_'`
	;;
    *)  low=`extract_normal "$type"`
	case "$type" in 
	    "long double")             baz=$type		;;
	    "unsigned long")           baz="double"		;;
	    "unsigned int")            baz="float"		;;
	    "unsigned short")          baz="float"		;;
	    "unsigned char")           baz="float"		;;
	    "double")                  baz=$type ; double=1	;;
	    "float")                   baz=$type		;;
	    "long")                    baz="double"		;;
	    "int")                     baz="float"		;;
	    "short")                   baz="float"		;;
	    "char")                    baz="float"		;;
	    *) 	echo "Unknown low-level type: $type" > /dev/stderr ; 	exit 1 	;;
	esac
	;;
esac


#
# Figure out the file name to output to
#
# file=`echo $skel | sed -e "s/_skel.hh/$name.hh/"`

# echo "Type :     $type" 
# echo "Low-level: $low"
# echo "Sub-type:  $sub"
# echo "Name:      $name"
# echo "File:      $file"
progname=`basename $0`

sed -e "s|@name@|$name|g" 		\
    -e "s|@cname@|$cname|g" 		\
    -e "s|@type@|$type |g" 		\
    -e "s|@sub@|$sub|g"   		\
    -e "s|@complex@|$complex|g" 	\
    -e "s|@double@|$double|g" 		\
    -e "s|@low@|$low|g"   		\
    -e "s|@ftype@|$baz|g"   		\
    -e "s|@btype@|$blas1|g"   		\
    -e "s|@btype1@|$blas2|g"   		\
    -e "s|@have_blas@|$have_blas|g"   	\
    -e "s|@progname@|$progname|g" 	\
    < $skel 
    
#
# EOF
#