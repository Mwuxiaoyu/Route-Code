
curdir = $(shell pwd)
all:

	$(MAKE) -C $(curdir)/openssl-0.9.8x \
		MAKEDEPPROG="$(CROSS_COMPILE)gcc" \
		OPENWRT_OPTIMIZATION_FLAGS="$(CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		depend
	$(_SINGLE)$(MAKE) -C $(curdir)/openssl-0.9.8x \
		CC="$(CROSS_COMPILE)gcc" \
		AR="$(CROSS_COMPILE)ar r" \
		RANLIB="$(CROSS_COMPILE)ranlib" \
		OPENWRT_OPTIMIZATION_FLAGS="$(CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		all
	$(MAKE) -C $(curdir)/openssl-0.9.8x \
		CC="$(CROSS_COMPILE)gcc" \
		AR="$(CROSS_COMPILE)ar r" \
		RANLIB="$(CROSS_COMPILE)ranlib" \
		OPENWRT_OPTIMIZATION_FLAGS="$(CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		build-shared
	# Work around openssl build bug to link libssl.so with libcrypto.so.
	-rm $(curdir)/openssl-0.9.8x/libssl.so.*.*.*
	$(MAKE) -C $(curdir)/openssl-0.9.8x \
		CC="$(CROSS_COMPILE)gcc" \
		OPENWRT_OPTIMIZATION_FLAGS="$(CFLAGS)" \
		$(OPENSSL_MAKEFLAGS) \
		do_linux-shared
	$(MAKE) -C $(curdir)/openssl-0.9.8x \
		INSTALL_PREFIX="$(curdir)/ipkg-install" \
		$(OPENSSL_MAKEFLAGS) \
		install

romfs:
	cp -fpR $(curdir)/ipkg-install/usr/lib/*.so*  ../../romfs/lib
	cp -fpR $(curdir)/ipkg-install/usr/lib/*.so  ../../romfs/lib

clean:
	$(MAKE) -C $(curdir)/openssl-0.9.8x clean
	rm -rf $(curdir)/ipkg-install


