##
# Copyright 2017-2020 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
##

include $(DEV_ROOT_DIR)/build/NativeBuild.mk
include $(DEV_ROOT_DIR)/build/Dependencies.mk
include $(DEV_ROOT_DIR)/build/CodeCoverage.mk
include $(DEV_ROOT_DIR)/build/LinuxBuildConfig.mk
include $(DEV_ROOT_DIR)/build/LinuxIncrementalBuild.mk
include $(DEV_ROOT_DIR)/build/DeviceTree.mk

ifeq ($(origin CC), default)
    CC  = $(CROSS_COMPILE)gcc
endif

ifeq ($(origin CXX), default)
    CXX = $(CROSS_COMPILE)g++
endif

ifeq ($(origin AR), default)
    AR  = $(CROSS_COMPILE)ar
endif

CFLAGS += -Wall


# Application
ifdef MODULE_NAME
    EXEC = $(OBJDIR)/$(MODULE_NAME)
else
    EXEC =
endif

.PHONY: all clean allsub cleansub lib app
.DEFAULT_GOAL: all

# Static lib
ifdef STATIC_LIB_NAME
    LIB_LOCK = $(STATIC_LIB_NAME).lock
    ST_LIB = $(OBJDIR)/lib$(STATIC_LIB_NAME).a

all allsub: $(ST_LIB) | $(OBJDIR)

lib: all
app:

else
    ifdef SHARED_LIB_NAME
        LDFLAGS += -shared
        LIB_LOCK = $(SHARED_LIB_NAME).lock
        SH_LIB = $(OBJDIR)/lib$(SHARED_LIB_NAME).so

all allsub: $(OBJS) $(SH_LIB)
    else
    ST_LIB =

all allsub: $(EXEC) | $(OBJDIR)

lib:
app: all

    endif
endif

_OBJS     = $(SRCS:.c=.c.o)
_OBJS1    = $(_OBJS:.S=.S.o)
OBJS      = $(addprefix $(OBJDIR)/,$(_OBJS1:.cpp=.cpp.o))

COMMON_WARNING_FLAGS =          \
	-Wall                   \
	-Wextra                 \
	-Wlogical-op            \
	-Wdouble-promotion      \
	-Wshadow                \

CFLAGS += $(CDEFS)              \
	-pedantic               \
	-std=c99                \
	-Wjump-misses-init      \
	$(COMMON_WARNING_FLAGS) \

CXXFLAGS = $(CDEFS)             \
	-pedantic               \
	-std=gnu++0x            \
	$(COMMON_WARNING_FLAGS) \

ifeq ($(findstring .cpp,$(SRCS)),.cpp)
    LD=$(CXX)
else
    LD=$(CC)
endif

vpath %.c   $(VPATH)
vpath %.S   $(VPATH)
vpath %.cpp $(VPATH)

INCFLAGS = $(foreach tmp_var,$(INCDIR),-I$(tmp_var))
LIBFLAGS = $(foreach tmp_var,$(LIBDIR),-L$(tmp_var))

ifdef OAL_BUILD_NO_FPIC
    PIC_OPTION =
else
    PIC_OPTION ?= -fPIC
endif

$(OBJS): | $(OBJDIR)

$(EXEC): $(OBJS) $(_DEP_STATIC_LIBS) $(_INJECT_LIBRARY)
	@echo "	[LD]  $@ <= $^"
	@$(LD) -o $@ $^ $(LDFLAGS) $(LIBFLAGS)

$(foreach lib, $(_DEP_STATIC_LIBS) $(_INJECT_LIBRARY), $(eval $(call generate_lib, $(lib))))

$(ST_LIB): $(OBJS)
	@echo "	[AR]  $@ <= $^"
	@$(AR) rcs $@ $^

$(SH_LIB): $(OBJS) $(_DEP_STATIC_LIBS)
	@echo "	[LD]  $@ <= $^"
	@$(CC) $(LIBFLAGS) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.c.o: %.c
	@echo "	[CC]  $@"
	@$(CC) $(INCFLAGS) $(CFLAGS) $(PIC_OPTION) $< -c -o $@

$(OBJDIR)/%.S.o: %.S
	@echo "	[CC]  $@"
	@$(CC) $(INCFLAGS) $(CFLAGS) $(PIC_OPTION) $< -c -o $@

$(OBJDIR)/%.cpp.o: %.cpp
	@echo "	[CXX] $@"
	@$(CXX) $(INCFLAGS) $(CXXFLAGS) $(PIC_OPTION) $< -c -o $@

clean cleansub:
	@rm -f $(OBJDIR)/*.o $(EXEC) $(ST_LIB) $(OBJDIR)/*.o.d $(SH_LIB) $(OBJDIR)/*.lock

-include $(DEV_ROOT_DIR)/build/SourceDrop.mk
-include $(DEV_ROOT_DIR)/build/Install.mk

