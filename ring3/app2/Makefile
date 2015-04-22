M=../../make
include $M/header.mk

I   =.#
O   =$I/_tmp#
TYPE=exe#
NAME=a#
ARCH=linux#
B   =release#

SOURCES=\
				$I/x86/boot.S  \
				$I/x86/main.cc \
				$I/x86/except.cc \
				$I/apps/labs.cc \
				$I/util/debug.cc \
				$I/util/lib.cc \


CPPFLAGS    = -I$I #-DTEST_LABS
TARGET_MACH = -m32
TARGET_ARCH = -m32

CXXFLAGS.debug   = -g
ASFLAGS.debug    = -g
CXXFLAGS.release = -O3

NO_SIMD=-mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-sse4 -mno-avx -mno-3dnow

CXXFLAGS = -std=c++0x -ffreestanding -fno-rtti -fno-exceptions \
					 -fPIE -pie                                          \
					 -z max-page-size=0x0000040                          \


#options specific for except.cc
CXXFLAGS.x86/except.cc = $(NO_SIMD)
CXXFLAGS.util/lib.cc   = $(NO_SIMD) -fno-tree-loop-distribute-patterns


# simd
CXXFLAGS.x86/main.cc             = -msse -msse2 -mfpmath=sse
CXXFLAGS.apps/labs.cc            = -msse -msse2 -mfpmath=sse
CXXFLAGS.labs/shell.cc           = -msse -msse2 -mfpmath=sse
CXXFLAGS.labs/coroutine.cc       = -msse -msse2 -mfpmath=sse
CXXFLAGS.labs/fiber.cc           = -msse -msse2 -mfpmath=sse
CXXFLAGS.labs/fiber_schedular.cc = -msse -msse2 -mfpmath=sse




LDFLAGS  = -m32  -nostdlib                                     \
					 -fPIE -pie                                          \
					 -z max-page-size=0x0000040                          \
					 -flto                                               \
  #-lsupc++ -lgcc

include $M/footer.mk

