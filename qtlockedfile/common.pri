exists(config.pri):infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtlockedfile-uselib

TEMPLATE += fakelib
greaterThan(QT_MAJOR_VERSION, 5)|\
  if(equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 4))|\
  if(equals(QT_MAJOR_VERSION, 5):equals(QT_MINOR_VERSION, 4):greaterThan(QT_PATCH_VERSION, 1)) {
    QTLOCKEDFILE_LIBNAME = $$qt5LibraryTarget(QtSolutions_LockedFile-head)
} else {
    QTLOCKEDFILE_LIBNAME = $$qtLibraryTarget(QtSolutions_LockedFile-head)
}
TEMPLATE -= fakelib

QTLOCKEDFILE_LIBDIR = $$PWD/lib
unix:qtlockedfile-uselib:!qtlockedfile-buildlib:QMAKE_RPATHDIR += $$QTLOCKEDFILE_LIBDIR
