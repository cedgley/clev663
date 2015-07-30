#
#
#

OBJ_DIR:=$(CURDIR)/obj
OUT_DIR:=$(CURDIR)/out
SRC_DIR:=$(CURDIR)/linux \
 $(CURDIR)/NXP-Reader-Lib/comps/phOsal/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phTools/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalMfc/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalMfc/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phbalReg/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phhalHw/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phhalHw/src/Rc663/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phhalHw/src/Callback/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phKeyStore/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phKeyStore/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phKeyStore/src/Rc663/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phacDiscLoop/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phacDiscLoop/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI18092mPI/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI18092mPI/src/Sw \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalMifare/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalMifare/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalFelica/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalFelica/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalFelica/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalFelica/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalMfdf/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalMfdf/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p3b/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p3b/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p3a/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p3a/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p4/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p4/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p4a/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p4a/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalMful/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalMful/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI18092mT/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phpalI18092mT/src/Sw/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalT1T/src/ \
 $(CURDIR)/NXP-Reader-Lib/comps/phalT1T/src/Sw/


# Include dirs
INCLUDE_DIR:=-I$(CURDIR)/include \
-I$(CURDIR)/NXP-Reader-Lib/intfs \
-I$(CURDIR)/NXP-Reader-Lib/types \
-I$(CURDIR)/NXP-Reader-Lib/comps/phalMfc/src/Sw \
-I$(CURDIR)/NXP-Reader-Lib/comps/phalMfdf/src/ \
-I$(CURDIR)/NXP-Reader-Lib/comps/phpalI14443p4/src/Sw/ \
-I$(CURDIR)/NXP-Reader-Lib/comps/phbalReg/src/Stub/ \
-I$(CURDIR)/NXP-Reader-Lib/comps/phalMful/src/Sw/ \
-I$(CURDIR)/NXP-Reader-Lib/comps/phpalI18092mT/src/Sw/ \
-I$(CURDIR)/NXP-Reader-Lib/comps/phalT1T/src/

# Library source top level directory
LIB_DIR:=lib
LIBS:=

OBJS-Y:=main.o phhwConfig.o phhalHw.o phTools.o phOsal.o phbalReg.o \
phKeyStore.o phKeyStore_Rc663.o phKeyStore_Sw.o \
phacDiscLoop.o phacDiscLoop_Sw.o phacDiscLoop_Sw_Int.o \
phhalHw_Rc663.o phhalHw_Rc663_Int.o phhalHw_Rc663_Cmd.o phhalHw_Rc663_Wait.o phhalHw_Callback.o \
phalMfc.o phalMfc_Sw.o phalMfc_Int.o phalMful_Sw.o phalMful_Int.o \
phpalMifare.o phpalMifare_Sw.o phpalFelica.o phpalFelica_Sw.o phalFelica_Sw.o \
phalMfdf.o phalMfdf_Sw.o phalMfdf_Int.o phalMfdf_Sw_Int.o \
phpalI14443p3a.o phpalI14443p3a_Sw.o phpalI14443p3b.o phpalI14443p3b_Sw.o \
phpalI14443p4.o phpalI14443p4_Sw.o phpalI14443p4a.o phpalI14443p4a_Sw.o \
phpalI18092mT.o phpalI18092mT_Sw.o phpalI18092mT_Sw_Int.o \
phpalI18092mPI.o phpalI18092mPI_Sw.o phpalI18092mPI_Sw_Int.o \
phalT1T.o phalT1T_Sw.o phbalReg_Stub.o phOsal_Stub.o

OBJS:= ${OBJS-Y:%.o=${OBJ_DIR}/%.o}

#export CFLAGS:=-Wall ${EXTRA_FLAGS} -O0 -g
export CFLAGS:=${EXTRA_FLAGS} -O0

vpath %.c $(SRC_DIR)

.PHONY: all clean rebuild

all:	dirs clev663

clean:
	@echo "Cleaning Up"
	@rm -rf $(OBJ_DIR)/*
	@rm -rf $(OUT_DIR)/*
	@rm clev663

dirs:
	@if test ! -d $(OBJ_DIR); then echo "Creating $(OBJ_DIR)!"; mkdir -p $(OBJ_DIR); fi
	@if test ! -d $(OUT_DIR); then echo "Creating $(OUT_DIR)!"; mkdir -p $(OUT_DIR); fi

rebuild:	clean all

clev663:	$(OBJS)
	@$(CC) -rdynamic -o$@ $(OBJS) -L$(LIB_DIR) $(LIBS)
	@echo "Compilation complete."

$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDE_DIR) -c -MMD -MP -MF"$(@:%.o=%.d)" -o"$@" "$<"

