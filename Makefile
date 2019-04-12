export V?=0

export CROSS_COMPILE = aarch64-linux-gnu-
export TEEC_EXPORT = $(shell pwd)/../../optee_client/out/export/usr/
export PLATFORM = vexpress-qemu_v8_virt
export TA_DEV_KIT_DIR = $(shell pwd)/../../optee_os/out/arm/export-ta_arm64/

# If _HOST or _TA specific compilers are not specified, then use CROSS_COMPILE
export HOST_CROSS_COMPILE ?= $(CROSS_COMPILE)
export TA_CROSS_COMPILE ?= $(CROSS_COMPILE)

.PHONY: all
all:
	$(MAKE) -C host --no-builtin-variables
	$(MAKE) -C ta

.PHONY: clean
clean:
	$(MAKE) -C host clean
	$(MAKE) -C ta clean
