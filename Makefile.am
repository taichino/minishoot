SUBDIRS = src

ARCHIVE_DIR = $(PACKAGE_NAME)
archive:
	cp -R src $(ARCHIVE_DIR) 
	tar czvf $(PACKAGE_NAME).tar.gz $(ARCHIVE_DIR)
	rm -rf $(ARCHIVE_DIR) 

install_spec: archive
	echo "install_spec"
