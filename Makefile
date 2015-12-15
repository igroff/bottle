.PHONY: build install start

build:
	# do nothing, because there is no build

install:
	./install $(LOGNAME)

start:
	start bottle
	# things are all managed by upstart, but we need something for
	# starphleet to bite onto
	sleep 999999
