SUBDIRS = lib cat revwords

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

clean:
	$(MAKE) -C $(CURDIR)/lib clean
	$(MAKE) -C $(CURDIR)/cat clean
	$(MAKE) -C $(CURDIR)/revwords clean
