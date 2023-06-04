###########################################################
#
# MAKEFILE FOR C/C++ PROJECT
# Author: swm8023 <swm8023@gmail.com>
# Date:   2014/01/30
#
###########################################################

.PHONY: all clean
all: 


prefix=./lib/



# annotation when release version
#DEBUG       := y
#TARGET_PROG := baidu_tts
#TARGET_LIBS :=libmycommon.a
TARGET_LIBS :=libmycommon.so

# project directory	
DEBUG_DIR   := ./Debug
RELEASE_DIR := ./Release
BIN_DIR     := $(if $(DEBUG), $(DEBUG_DIR), $(RELEASE_DIR))

# shell command
CC    := gcc
CXX   := g++
RM    := rm -rf
MKDIR := mkdir -p
SED   := sed
MV    := mv
AR    := ar

# init sources & objects & depends
sources_all := $(shell find . -name "*.c" -o -name "*.cpp" -o -name "*.h")
sources_c   := $(filter %.c, $(sources_all))
sources_cpp := $(filter %.cpp, $(sources_all))
sources_h   := $(filter %.h, $(sources_all))
objs        := $(addprefix $(BIN_DIR)/,$(strip $(sources_cpp:.cpp=.o) $(sources_c:.c=.o)))
deps        := $(addprefix $(BIN_DIR)/,$(strip $(sources_cpp:.cpp=.d) $(sources_c:.c=.d)))

# create directory
$(foreach dirname,$(sort $(dir $(sources_c) $(sources_cpp))),\
  $(shell $(MKDIR) $(BIN_DIR)/$(dirname)))

# complie & link variable
CFLAGS     := $(if $(DEBUG),-g -O, -O2) 
CFLAGS     += $(addprefix -I ,$(sort $(dir $(sources_h)))) -I /usr/include/mysql -lhiredis -lpthread -fPIC -shared
CXXFLAGS    = $(CFLAGS)
LDFLAGS    := -crv
LOADLIBES  += #-L/usr/include/mysql
LDLIBS     += #-lcurl

# add vpath
vpath %.h $(sort $(dir $(sources_h)))
vpath %.c $(sort $(dir $(sources_c)))
vpath %.cpp $(sort $(dir $(sources_cpp)))

# generate depend files
# actually generate after object generated, beacasue it only used when next make)
ifneq "$(MAKECMDGOALS)" "clean"
sinclude $(deps)
endif

# make-depend(depend-file,source-file,object-file,cc)
define make-depend
  $(RM) $1;                                     \
  $4 $(CFLAGS) -MM $2 |                         \
  $(SED) 's,\($(notdir $3)\): ,$3: ,' > $1.tmp; \
  $(SED) -e 's/#.*//'                           \
         -e 's/^[^:]*: *//'                     \
         -e 's/ *\\$$//'                        \
         -e '/^$$/ d'                           \
         -e 's/$$/ :/' < $1.tmp >> $1.tmp;      \
  $(MV) $1.tmp $1;
endef

# rules to generate objects file
$(BIN_DIR)/%.o: %.c
	@$(call make-depend,$(patsubst %.o,%.d,$@),$<,$@,$(CC))
	$(CC) $(CFLAGS) -shared -o $@ -c $<

$(BIN_DIR)/%.o: %.cpp
	@$(call make-depend,$(patsubst %.o,%.d,$@),$<,$@,$(CXX))
	$(CXX) $(CXXFLAGS) -shared -o $@ -c $<

# add-target(target,objs,ar)
define add-target
  REAL_TARGET += $(BIN_DIR)/$1
  $(BIN_DIR)/$1: $2
	$3 $(LDFLAGS) $$^ $(LOADLIBES) $(LDLIBS) -o $$@
endef

# add-target(target,objs,ar)
define add-lib
  REAL_LIB += $(BIN_DIR)/$1
  $(BIN_DIR)/$1: $2
	$3  $(LDFLAGS) $$@ $$^ $(LOADLIBES) $(LDLIBS) 
endef

# call add-target
$(foreach targ,$(TARGET_PROG),$(eval $(call add-target,$(targ),$(objs),$(CXX))))

# call add-target
$(foreach targ,$(TARGET_LIBS),$(eval $(call add-lib,$(targ),$(objs),$(AR))))

#all: $(REAL_TARGET) $(REAL_LIB)
#all: $(REAL_TARGET)
all:$(REAL_LIB)

install: $(REAL_LIB)
	install -m 0755 $(REAL_LIB) $(prefix)

#.PHONY: install


clean: 
	$(RM) $(BIN_DIR)
