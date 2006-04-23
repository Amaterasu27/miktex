dnl @synopsis AX_SPLIT_VERSION
dnl
dnl Splits a version number in the format MAJOR.MINOR.POINT into it's
dnl separeate components.
dnl
dnl Sets the variables.
dnl
dnl @category Automake
dnl @author Tom Howard <tomhoward@users.sf.net>
dnl @version 2005-01-14
dnl @license AllPermissive

dnl Modified for MiKTeX by Christian Schenk

AC_DEFUN([AX_SPLIT_VERSION],[
    MIKTEX_MAJOR_VERSION=`echo "$VERSION" | sed 's/\([[0-9]][[0-9]]*\)\.\([[0-9]][[0-9]]*\)\.\([[0-9]][[0-9]]*\).*/\1/'`
    MIKTEX_MINOR_VERSION=`echo "$VERSION" | sed 's/\([[0-9]][[0-9]]*\)\.\([[0-9]][[0-9]]*\)\.\([[0-9]][[0-9]]*\).*/\2/'`
    MIKTEX_J2000_VERSION=`echo "$VERSION" | sed 's/\([[0-9]][[0-9]]*\)\.\([[0-9]][[0-9]]*\)\.\([[0-9]][[0-9]]*\).*/\3/'`
    case "$VERSION" in
      *-rc-*)
        MIKTEX_RELEASE_STATE=1; MIKTEX_RELEASE_NUMBER=`echo "$VERSION" | sed 's/.*-rc-\([[0-9]][[0-9]]*\).*/\1/'` ;;
      *-beta-*)
        MIKTEX_RELEASE_STATE=2; MIKTEX_RELEASE_NUMBER=`echo "$VERSION" | sed 's/.*-beta-\([[0-9]][[0-9]]*\).*/\1/'` ;;
      *-snapshot)
        MIKTEX_RELEASE_STATE=4; MIKTEX_RELEASE_NUMBER=0 ;;
      *)
        MIKTEX_RELEASE_STATE=0; MIKTEX_RELEASE_NUMBER=0 ;;
    esac
])

