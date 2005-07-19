################################
#
# makefile for flip
#

SHELL= /bin/sh
PERL= /usr/bin/perl
SUDO= /usr/bin/sudo

install: check-files check-templates create-tmp ruby-check
	@echo '==> intalling flip ...'


ruby-check:
	@echo '==> checking ruby support'
	@if `pkg_info | grep ruby > /dev/null 2>&1`; then \
		echo '==>  ruby found'; \
	else \
		echo '==>  ruby not found'; \
		exit 1; \
	fi
	@if `pkg_info | grep ruby-ncurses > /dev/null 2>&1`; then \
		echo '==>  ruby-ncurses found'; \
	else \
		echo '==>  ruby-ncurses not found'; \
		exit 1; \
	fi

check-files:
	@echo '==> checking for programm files'
	@if `ls | grep ap.rb > /dev/null 2>&1`; then \
		echo '==>  ap.rb found'; \
	else \
		echo '==>  ap.rb not found'; \
		exit 1; \
	fi
	@if `ls | grep flip.rb > /dev/null 2>&1`; then \
		echo '==>  flip.rb found'; \
	else \
		echo '==>  flip.rb not found'; \
		exit 1; \
	fi
	@if `ls | grep flipd.rb > /dev/null 2>&1`; then \
		echo '==>  flipd.rb found'; \
	else \
		echo '==>  flipd.rb not found'; \
		exit 1; \
	fi
	@if `ls | grep gui.rb > /dev/null 2>&1`; then \
		echo '==>  gui.rb found'; \
	else \
		echo '==>  gui.rb not found'; \
		exit 1; \
	fi

check-templates:
	@echo '==> checking for templates'
	@if `ls | grep config > /dev/null 2>&1`; then \
		echo '==>  config dir available'; \
	else \
		echo '==>  config dir missing'; \
		exit 1; \
	fi
	@if `ls config | grep templates > /dev/null 2>&1`; then \
		echo '==>  templates dir available'; \
	else \
		echo '==>  templates dir missing'; \
		exit 1; \
	fi
	@if `ls config/templates | grep etc > /dev/null 2>&1`; then \
		echo '==>  etc dir available'; \
	else \
		echo '==>  etc dir missing'; \
		exit 1; \
	fi
	@if `ls config/templates | grep root > /dev/null 2>&1`; then \
		echo '==>  root dir available'; \
	else \
		echo '==>  root dir missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep bridgename.bridge0 > /dev/null 2>&1`; then \
		echo '==>  bridgename.bridge0 found'; \
	else \
		echo '==>  bridgename.bridge0 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep bridgename.bridge1 > /dev/null 2>&1`; then \
		echo '==>  bridgename.bridge1 found'; \
	else \
		echo '==>  bridgename.bridge1 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep bridgename.bridge2 > /dev/null 2>&1`; then \
		echo '==>  bridgename.bridge2 found'; \
	else \
		echo '==>  bridgename.bridge2 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep hostname.ath0 > /dev/null 2>&1`; then \
		echo '==>  hostname.ath0 found'; \
	else \
		echo '==>  hostname.ath0 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep hostname.ath1 > /dev/null 2>&1`; then \
		echo '==>  hostname.ath1 found'; \
	else \
		echo '==>  hostname.ath0 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep hostname.sis0 > /dev/null 2>&1`; then \
		echo '==>  hostname.sis0 found'; \
	else \
		echo '==>  hostname.sis0 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep hostname.wi0 > /dev/null 2>&1`; then \
		echo '==>  hostname.wi0 found'; \
	else \
		echo '==>  hostname.wi0 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep hostname.vlan0 > /dev/null 2>&1`; then \
		echo '==>  hostname.vlan0 found'; \
	else \
		echo '==>  hostname.vlan0 missing'; \
		exit 1; \
	fi
	@if `ls config/templates/etc | grep rc.local > /dev/null 2>&1`; then \
		echo '==>  rc.local found'; \
	else \
		echo '==>  rc.local missing'; \
		exit 1; \
	fi

create-tmp:
	@echo '==> checking for tmp dir'
	@if `ls | grep tmp > /dev/null`; then \
		echo '==>  old tmp dir found moving it ....'; \
		mv tmp tmp.old ; \
		echo '==>  creating tmp dir'; \
		mkdir tmp ; \
	else \
		echo '==>  creating tmp dir'; \
		mkdir tmp ; \
	fi
